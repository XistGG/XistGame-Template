// Copyright (c) 2025 Xist.GG LLC

#pragma once

#include "GameFramework/GameModeBase.h"
#include "XistGameGameMode.generated.h"

UCLASS(MinimalAPI)
class AXistGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Set Class Defaults
	AXistGameGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
