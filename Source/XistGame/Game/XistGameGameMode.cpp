// Copyright (c) 2025 Xist.GG LLC

#include "XistGameGameMode.h"
#include "XistGameCharacter.h"
#include "XistGamePlayerController.h"

// Set Class Defaults
AXistGameGameMode::AXistGameGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Never tick actors unless it's absolutely required
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Enable use of XistGame classes
	DefaultPawnClass = AXistGameCharacter::StaticClass();
	PlayerControllerClass = AXistGamePlayerController::StaticClass();
}

void AXistGameGameMode::PostInitProperties()
{
	Super::PostInitProperties();

	if (OverridePawnClass)
	{
		// Override default class based on INI
		DefaultPawnClass = OverridePawnClass;
	}

	if (OverridePlayerControllerClass)
	{
		// Override default class based on INI
		PlayerControllerClass = OverridePlayerControllerClass;
	}
}
