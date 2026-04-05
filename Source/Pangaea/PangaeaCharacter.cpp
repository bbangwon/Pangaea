// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "PangaeaAnimInstance.h"
#include "Net/UnrealNetwork.h"

APangaeaCharacter::APangaeaCharacter()
{	
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void APangaeaCharacter::BeginPlay()
{
	Super::BeginPlay();
	_AnimInstance = Cast<UPangaeaAnimInstance>(GetMesh()->GetAnimInstance());
	_HealthPoints = HealthPoints;
}

void APangaeaCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

	_AnimInstance->Speed = GetCharacterMovement()->Velocity.Size2D();
	if (_AttackCountingDown == AttackInterval)
	{
		_AnimInstance->State = ECharacterState::Attack;
	}

	if (_AttackCountingDown > 0.0f)
	{
		_AttackCountingDown -= DeltaTime;
	}
}

int APangaeaCharacter::GetHealthPoints()
{
	return _HealthPoints;
}

bool APangaeaCharacter::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}

bool APangaeaCharacter::IsAttacking()
{
	return _AnimInstance->State == ECharacterState::Attack;
}

bool APangaeaCharacter::CanAttack()
{
	return (_AttackCountingDown <= 0.0f && _AnimInstance->State == ECharacterState::Locomotion);
}

void APangaeaCharacter::Attack()
{
	_AttackCountingDown = AttackInterval;
}

void APangaeaCharacter::Hit(int damage)
{	
	_HealthPoints -= damage;
	_AnimInstance->State = ECharacterState::Hit;

	UE_LOG(LogTemp, Warning, TEXT("Character hit! Current HP: %d"), _HealthPoints);

	if (IsKilled())
	{
		DieProcess();
	}
}

void APangaeaCharacter::DieProcess()
{
	Destroy();
}

void APangaeaCharacter::AttackBroadcastRpc_Implementation()
{
	Attack();
}

void APangaeaCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APangaeaCharacter, _HealthPoints);
}

