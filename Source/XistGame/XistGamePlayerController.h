// Copyright (c) 2025 Xist.GG LLC

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "XistGamePlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

/**
 * AXistGamePlayerController
 * 
 * Blueprint classes are assigned as game defaults via Config INI.
 *
 * @see Config/DefaultXistGame.ini
 */
UCLASS(Blueprintable, Config=XistGame)
class AXistGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Set class defaults
	AXistGamePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin UObject interface
	virtual void PostInitProperties() override;
	//~End UObject interface

protected:
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UNiagaraSystem> FXCursor;

	/** IMC */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	/** Click to Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> SetDestinationClickAction;

	/** Touch to Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> SetDestinationTouchAction;

	//~Begin APlayerController interface
	virtual void SetupInputComponent() override;
	//~End of APlayerController interface

	//~Begin AActor interface
	virtual void BeginPlay() override;
	//~End of AActor interface

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

protected:
	UPROPERTY(Config, VisibleAnywhere)
	FString FXCursorPath;

	UPROPERTY(Config, VisibleAnywhere)
	FString IMCPath;

	UPROPERTY(Config, VisibleAnywhere)
	FString SetDestClickActionPath;

	UPROPERTY(Config, VisibleAnywhere)
	FString SetDestTouchActionPath;

	void ApplyIniSettings();

private:
	FVector CachedDestination;

	bool bIsTouch {false}; // Is it a touch device
	float FollowTime {0.f}; // For how long it has been pressed
};


