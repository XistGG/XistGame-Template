// Copyright (c) 2025 Xist.GG LLC

#pragma once

#include "GameFramework/GameModeBase.h"
#include "XistGameGameMode.generated.h"

/**
 * AXistGameGameMode
 *
 * Blueprint classes are assigned as game defaults via Config INI.
 *
 * @see Config/DefaultXistGame.ini
 */
UCLASS(Blueprintable, Config=XistGame)
class XISTGAME_API AXistGameGameMode
	: public AGameModeBase
{
	GENERATED_BODY()

public:
	// Set Class Defaults
	AXistGameGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~Begin UObject interface
	virtual void PostInitProperties() override;
	//~End UObject interface

protected:
	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<APawn> OverridePawnClass;

	UPROPERTY(Config, EditDefaultsOnly)
	TSubclassOf<APlayerController> OverridePlayerControllerClass;
};
