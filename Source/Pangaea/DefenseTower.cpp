// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseTower.h"
#include "PlayerAvatar.h"
#include "Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Weapon.h"
#include "PangaeaCharacter.h"
#include "PangaeaGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADefenseTower::ADefenseTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SetRootComponent(_SphereComponent);

	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	_MeshComponent->SetupAttachment(_SphereComponent);
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(TEXT("Blueprint'/Game/TopDown/Blueprints/BP_Fireball.BP_Fireball'"));
	if (blueprint_finder.Succeeded())
	{
		_FireballClass = blueprint_finder.Object->GeneratedClass;
	}	
}

// Called when the game starts or when spawned
void ADefenseTower::BeginPlay()
{
	Super::BeginPlay();

	// 이벤트 함수 등록
	_SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADefenseTower::OnBeginOverlap);
	_SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ADefenseTower::OnEndOverlap);

	_GameMode = Cast<APangaeaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	_HealthPoints = HealthPoints;

}

// Called every frame
void ADefenseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (_Target != nullptr)
	{
		if (CanFire())
		{
			Fire();
			_ReloadCountingDown = ReloadInterval;
		}
	}
	if (ReloadInterval > 0.0f)
	{
		_ReloadCountingDown -= DeltaTime;
	}
}

int ADefenseTower::GetHealthPoints()
{
	return _HealthPoints;
}

bool ADefenseTower::IsDestroyed()
{
	return (_HealthPoints <= 0.0f);
}

bool ADefenseTower::CanFire()
{
	return false;
	//return (_ReloadCountingDown <= 0.0f);
}

void ADefenseTower::Fire()
{
	
	auto Fireball = _GameMode->SpawnOrGetFireball(_FireballClass); //Cast<AProjectile>(GetWorld()->SpawnActor(_FireballClass));
	FVector StartLocation = GetActorLocation();
	StartLocation.Z += 100.0f; // 발사 위치를 타워보다 약간 위로 설정
	FVector TargetLocation = _Target->GetActorLocation();
	TargetLocation.Z = StartLocation.Z; // 타겟 위치의 Z값을 발사 위치와 동일하게 설정하여 수평으로 발사
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);
	Fireball->SetActorLocation(StartLocation);
	Fireball->SetActorRotation(Rotation);
}



void ADefenseTower::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerAvatar* Player = Cast<APlayerAvatar>(OtherActor);
	if (Player)
	{
		_Target = Player;
	}
}

void ADefenseTower::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (_Target != nullptr && OtherActor == _Target)
	{
		_Target = nullptr;
	}
}

void ADefenseTower::OnMeshBeginOverlap(AActor* OtherActor)
{
	AWeapon* weapon = Cast<AWeapon>(OtherActor);
	if (weapon == nullptr || weapon->Holder == nullptr)
	{
		return;
	}

	APlayerAvatar* playerAvatar = Cast<APlayerAvatar>(weapon->Holder);
	if (playerAvatar != nullptr && 
		playerAvatar->IsAttacking() &&
		CanBeDamaged())
	{
		Hit(playerAvatar->Strength);
	}
}

void ADefenseTower::Hit(int damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Defense Tower is hit! Damage: %d"), damage);
	_HealthPoints -= damage;

	UE_LOG(LogTemp, Warning, TEXT("Defense Tower is hit! Current HP: %d"), _HealthPoints);

	if (IsDestroyed())
	{
		DestroyProcess();
	}
}

void ADefenseTower::DestroyProcess()
{
	Destroy();
}

