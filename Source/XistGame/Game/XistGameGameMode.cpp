// Copyright (c) 2025 Xist.GG LLC

#include "XistGameGameMode.h"
#include "XistGamePlayerController.h"
#include "XistGameCharacter.h"

// Set Class Defaults
AXistGameGameMode::AXistGameGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
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
