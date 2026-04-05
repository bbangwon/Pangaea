// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Enemy.h"

void AEnemyController::MakeAttackDecision(APawn* targetPawn)
{
	auto controlledCharacter = Cast<AEnemy>(GetPawn());
	if (targetPawn == nullptr || controlledCharacter == nullptr)
	{
		return;
	}

	auto dist = FVector::Dist2D(targetPawn->GetActorLocation(), controlledCharacter->GetTargetLocation());

	if (dist <= controlledCharacter->AttackRange && controlledCharacter->CanAttack())
	{
		StopMovement();
		controlledCharacter->AttackBroadcastRpc();
	}
	else if (dist > controlledCharacter->AttackRange)
	{
		controlledCharacter->Chase(targetPawn);
	}
}
