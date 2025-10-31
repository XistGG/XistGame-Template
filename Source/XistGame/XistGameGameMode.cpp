// Copyright (c) 2025 Xist.GG LLC

#include "XistGameGameMode.h"
#include "XistGamePlayerController.h"
#include "XistGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

// Set Class Defaults
AXistGameGameMode::AXistGameGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultPawnClass = AXistGameCharacter::StaticClass();
	PlayerControllerClass = AXistGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character if possible
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_Character"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller if possible
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_PlayerController"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}
