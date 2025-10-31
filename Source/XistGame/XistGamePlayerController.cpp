// Copyright (c) 2025 Xist.GG LLC

#include "XistGamePlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "XistGameLog.h"
#include "Engine/LocalPlayer.h"

// Set class defaults
AXistGamePlayerController::AXistGamePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	ShortPressThreshold = 0.2f;  // 200 ms
}

void AXistGamePlayerController::PostInitProperties()
{
	Super::PostInitProperties();

	ApplyIniSettings();
}

void AXistGamePlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Help the dev understand if they've misconfigured a required setting
	ensureMsgf(FXCursor != nullptr, TEXT("%hs: You didn't configure a valid FXCursor"), __FUNCTION__);
	ensureMsgf(DefaultMappingContext != nullptr, TEXT("%hs: You didn't configure a valid DefaultMappingContext"), __FUNCTION__);
	ensureMsgf(SetDestinationClickAction != nullptr, TEXT("%hs: You didn't configure a valid SetDestinationClickAction"), __FUNCTION__);
	ensureMsgf(SetDestinationTouchAction != nullptr, TEXT("%hs: You didn't configure a valid SetDestinationTouchAction"), __FUNCTION__);
}

void AXistGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AXistGamePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AXistGamePlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AXistGamePlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AXistGamePlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &AXistGamePlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &AXistGamePlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &AXistGamePlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &AXistGamePlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogXistGame, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AXistGamePlayerController::OnInputStarted()
{
	StopMovement();
}

// Triggered every frame when the input is held down
void AXistGamePlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AXistGamePlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AXistGamePlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AXistGamePlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AXistGamePlayerController::ApplyIniSettings()
{
	if (FXCursor == nullptr
		&& not FXCursorPath.IsEmpty())
	{
		FXCursor = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), nullptr,
			FXCursorPath, nullptr, LOAD_None, nullptr));
		UE_CLOG(FXCursor == nullptr, LogXistGame, Error, TEXT("%hs: FXCursorPath [%s] is not valid"), __FUNCTION__, *FXCursorPath);
	}

	if (DefaultMappingContext == nullptr
		&& not IMCPath.IsEmpty())
	{
		DefaultMappingContext = Cast<UInputMappingContext>(StaticLoadObject(UInputMappingContext::StaticClass(), nullptr,
			IMCPath, nullptr, LOAD_None, nullptr));
		UE_CLOG(DefaultMappingContext == nullptr, LogXistGame, Error, TEXT("%hs: IMCPath [%s] is not valid"), __FUNCTION__, *IMCPath);
	}

	if (SetDestinationClickAction == nullptr
		&& not SetDestClickActionPath.IsEmpty())
	{
		SetDestinationClickAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr,
			SetDestClickActionPath, nullptr, LOAD_None, nullptr));
		UE_CLOG(SetDestinationClickAction == nullptr, LogXistGame, Error, TEXT("%hs: SetDestClickActionPath [%s] is not valid"), __FUNCTION__, *SetDestClickActionPath);
	}

	if (SetDestinationTouchAction == nullptr
		&& not SetDestTouchActionPath.IsEmpty())
	{
		SetDestinationTouchAction = Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr,
			SetDestTouchActionPath, nullptr, LOAD_None, nullptr));
		UE_CLOG(SetDestinationTouchAction == nullptr, LogXistGame, Error, TEXT("%hs: SetDestTouchActionPath [%s] is not valid"), __FUNCTION__, *SetDestTouchActionPath);
	}
}
