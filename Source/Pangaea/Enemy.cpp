// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyAnimInstance.h"
#include "EnemyController.h"
#include "UObject/ConstructorHelpers.h"
#include "Weapon.h"
#include "PangaeaAnimInstance.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = 1000.f; // 시야 범위 설정
	SightConfig->LoseSightRadius = 1200.f; // 시야에서 벗어나는 범위 설정
	SightConfig->PeripheralVisionAngleDegrees = 90.f; // 감지각 범위 설정
	SightConfig->SetMaxAge(2); // 자극 기억 시간

	SightConfig->DetectionByAffiliation.bDetectEnemies = true; // 적 감지 허용
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; // 중립 감지 허용 여부 (예: 플레이어 캐릭터가 중립으로 설정된 경우)
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; // 아군 감지 허용 여부

	AIPerceptionComponent->ConfigureSense(*SightConfig); // 여러 감지를 사용하려면 배열로 저장해서 전달
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation()); // 우선적으로 사용할 감지 설정
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	_Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponClass));
	_Weapon->Holder = this;
	_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocket"));
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	if (_chasedTarget != nullptr && _AnimInstance->State == ECharacterState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MakeAttackDecision(_chasedTarget);
	}
}

void AEnemy::Chase(APawn* targetPawn)
{	
	if (targetPawn != nullptr && _AnimInstance->State == ECharacterState::Locomotion)
	{		
		auto enemyController = Cast<AEnemyController>(GetController());
		if (enemyController == nullptr)
		{
			return;
		}
		if (enemyController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			UE_LOG(LogTemp, Warning, TEXT("Chase start target: %s"), *targetPawn->GetName());
			enemyController->MoveToActor(targetPawn, 90.f);
		}			
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Target is null or not in locomotion state"));
	}
	_chasedTarget = targetPawn;	
}

void AEnemy::DieProcess()
{
	Super::DieProcess();
	_Weapon->Destroy();
}
