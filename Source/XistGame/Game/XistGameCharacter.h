// Copyright (c) 2025 Xist.GG LLC

#pragma once

#include "GameFramework/Character.h"
#include "XistGameCharacter.generated.h"

/**
 * AXistGameCharacter
 * 
 * This is the primary character class used in this project.
 * 
 * @see AXistGameGameMode
 */
UCLASS(Blueprintable, Config=XistGame)
class XISTGAME_API AXistGameCharacter
	: public ACharacter
{
	GENERATED_BODY()

public:
	// Set Class Defaults
	AXistGameCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** @return TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/** @return CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;
};
