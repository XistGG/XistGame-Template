// Copyright (c) 2025 Xist.GG LLC

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "XistPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

UCLASS()
class AXistPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Set class defaults
	AXistPlayerController();

protected:
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** IMC */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Click to Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Touch to Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

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

private:
	FVector CachedDestination;

	bool bIsTouch {false}; // Is it a touch device
	float FollowTime {0.f}; // For how long it has been pressed
};


