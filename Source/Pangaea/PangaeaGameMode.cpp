// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaGameMode.h"

APangaeaGameMode::APangaeaGameMode()
{
	// stub
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_PlayerAvatar"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}