// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyAnimInstance.h"
#include "EnemyController.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	_HealthPoints = HealthPoints;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	auto animInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->Speed = GetCharacterMovement()->Velocity.Size2D();

	if (_AttackCountDown == AttackInterval)
	{
		animInst->State = EEnemyState::Attack;
	}

	if (_AttackCountDown > 0.0f)
	{
		_AttackCountDown -= DeltaTime;
	}

	if (_chasedTarget != nullptr && animInst->State == EEnemyState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MakeAttackDecision(_chasedTarget);
	}
}

int AEnemy::GetHealthPoints()
{
	return _HealthPoints;
}

bool AEnemy::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}

bool AEnemy::CanAttack()
{	
	auto enemyAnimInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	return (_AttackCountDown <= 0.0f) && (enemyAnimInst->State == EEnemyState::Locomotion);
}

void AEnemy::Chase(APawn* targetPawn)
{	
	auto enemyAnimInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (targetPawn != nullptr && enemyAnimInst->State == EEnemyState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MoveToActor(targetPawn, 90.f);
	}
	_chasedTarget = targetPawn;	
}

void AEnemy::Attack()
{
	GetController()->StopMovement();
	_AttackCountDown = AttackInterval;
}

void AEnemy::Hit(int damage)
{
	_HealthPoints -= damage;

	auto enemyAnimInst = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	enemyAnimInst->State = EEnemyState::Hit;

	if (IsKilled())
	{
		DieProcess();
	}
}

void AEnemy::DieProcess()
{
	Destroy();
}

