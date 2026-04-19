// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaGameMode.h"
#include "GameFramework/PlayerController.h"
#include "PangaeaGameState.h"

APangaeaGameMode::APangaeaGameMode()
{
	// stub
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_PlayerAvatar"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}	

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_PangaeaPlayerController"));
	if (PlayerControllerBPClass.Class != nullptr)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	GameStateClass = APangaeaGameState::StaticClass();
}

APangaeaGameMode::~APangaeaGameMode()
{
	AProjectile* Fireball = nullptr;
	while (!_FireballPool.IsEmpty() && _FireballPool.Dequeue(Fireball))
	{
		if (Fireball)
		{
			Fireball->Destroy();
		}
	}
	_FireballPool.Empty();
}

AProjectile* APangaeaGameMode::SpawnOrGetFireball(UClass* ProjectileClass)
{
	AProjectile* Fireball = nullptr;
	if (_FireballPool.IsEmpty())
	{
		Fireball = Cast<AProjectile>(GetWorld()->SpawnActor(ProjectileClass));
	}
	else
	{
		_FireballPool.Dequeue(Fireball);
		Fireball->Reset();
	}
	return Fireball;
}

void APangaeaGameMode::RecycleFireball(AProjectile* Projectile)
{
	if (Projectile == nullptr)
	{
		return;
	}

	Projectile->SetActorHiddenInGame(true);
	Projectile->SetActorEnableCollision(false);
	Projectile->SetActorTickEnabled(false);
	_FireballPool.Enqueue(Projectile);
}
