// Copyright (c) 2025 Xist.GG LLC

#include "XistGamePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "XistGameCharacter.h"
#include "XistGameLog.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"

// Set class defaults
AXistGamePlayerController::AXistGamePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// PlayerController explicitly requires ticking for inputs to work
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

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
	ensureMsgf(CameraZoomAction != nullptr, TEXT("%hs: You didn't configure a valid CameraZoomAction"), __FUNCTION__);
	ensureMsgf(SetDestinationClickAction != nullptr, TEXT("%hs: You didn't configure a valid SetDestinationClickAction"), __FUNCTION__);
	ensureMsgf(SetDestinationTouchAction != nullptr, TEXT("%hs: You didn't configure a valid SetDestinationTouchAction"), __FUNCTION__);
}

void AXistGamePlayerController::OnCameraZoom(const FInputActionInstance& InputActionInstance)
{
	const float RawInputValue = InputActionInstance.GetValue().Get<float>();
	const float NormalizedInputValue = FMath::Clamp(RawInputValue, -1.f, 1.f);

	// This project must use AXistGameCharacter as a base class for its Player Pawn
	const AXistGameCharacter* XistGameCharacter = GetPawn<AXistGameCharacter>();
	check(XistGameCharacter);

	USpringArmComponent* SpringArm = XistGameCharacter->GetCameraBoom();
	check(SpringArm);

	constexpr double ZoomStep = 500.;
	constexpr double MinLength = 500.;
	constexpr double MaxLength = 10000.;

	const double NewValue = SpringArm->TargetArmLength + (NormalizedInputValue * ZoomStep);
	SpringArm->TargetArmLength = FMath::Clamp(NewValue, MinLength, MaxLength);}

void AXistGamePlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Enhanced Input subsystem is required by this project
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	// Enhanced Input component is required as a base for our input component
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	// Add Input Mapping Context
	Subsystem->AddMappingContext(DefaultMappingContext, 0);

	// Set up action bindings:

	// Setup mouse input events
	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ThisClass::OnInputStarted);
	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ThisClass::OnSetDestinationTriggered);
	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ThisClass::OnSetDestinationReleased);
	EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ThisClass::OnSetDestinationReleased);

	// Setup touch input events
	EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ThisClass::OnInputStarted);
	EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ThisClass::OnTouchTriggered);
	EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ThisClass::OnTouchReleased);
	EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ThisClass::OnTouchReleased);

	// Setup Camera Zoom events
	EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &ThisClass::OnCameraZoom);
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

/**
 * If the CurrentValue is not already set by BP, return the default value from the INI
 * @tparam T UObject class
 * @param ObjectFName Name of the sub-object to report in error log messages
 * @param CurrentValue Pointer to current value (or nullptr)
 * @param ConfigPath Path read from INI, represents default value if current is nullptr
 * @return 
 */
template<class T>
T* GetWithIniDefault(const FName& ObjectFName, T* CurrentValue, const FString& ConfigPath)
{
	T* Result = CurrentValue;  // possibly nullptr
	if (not Result && not ConfigPath.IsEmpty())
	{
		Result = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, ConfigPath,
			nullptr, LOAD_None, nullptr));

		UE_CLOG(Result == nullptr, LogXistGame, Error, TEXT("%hs: %s [%s] is not valid"),
			__FUNCTION__, *ObjectFName.ToString(), *ConfigPath);
	}
	return Result;  // possibly nullptr
}

void AXistGamePlayerController::ApplyIniSettings()
{
	FXCursor = GetWithIniDefault<UNiagaraSystem>("FXCursor", FXCursor, FXCursorPath);

	DefaultMappingContext = GetWithIniDefault<UInputMappingContext>("DefaultMappingContext", DefaultMappingContext, IMCPath);

	CameraZoomAction = GetWithIniDefault<UInputAction>("CameraZoomAction", CameraZoomAction, CameraZoomActionPath);
	SetDestinationClickAction = GetWithIniDefault<UInputAction>("SetDestinationClickAction", SetDestinationClickAction, SetDestClickActionPath);
	SetDestinationTouchAction = GetWithIniDefault<UInputAction>("SetDestinationTouchAction", SetDestinationTouchAction, SetDestTouchActionPath);
}
