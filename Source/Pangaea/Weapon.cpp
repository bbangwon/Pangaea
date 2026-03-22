// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "PlayerAvatar.h"
#include "DefenseTower.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(_StaticMesh);	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBeginOverlap);
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Holder == nullptr)
	{
		auto rot = FRotator(0.0f, 300.f * DeltaTime, 0.0f);
		AddActorLocalRotation(rot);
	}

}

bool AWeapon::IsWithinAttackRange(float AttackRange, AActor* Target)
{
	return false;
}

void AWeapon::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	auto Character = Cast<APangaeaCharacter>(OtherActor);
	if (Character == nullptr)
	{
		return;
	}
	if (Holder == nullptr)
	{
		auto PlayerAvatar = Cast<APlayerAvatar>(Character);
		if (PlayerAvatar != nullptr)
		{
			Holder = PlayerAvatar;			
			PlayerAvatar->DropWeapon();
			PlayerAvatar->AttactWeapon(this);
		}
		else
		{
			return;
		}		
	}
	else if(Character != Holder && 
		IsWithinAttackRange(0.0f, OtherActor) &&
		Character->CanBeDamaged() &&
		Holder->IsAttacking())
	{
		//적이 공격
		Character->Hit(Holder->Strength);
		if (Character->IsA(APlayerAvatar::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Hit PlayerAvatar"));
			UE_LOG(LogTemp, Log, TEXT("Hit PlayerAvatar"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Hit Enemy"));
			UE_LOG(LogTemp, Log, TEXT("Hit Enemy"));
		}
	}
	else if (Holder != nullptr &&
		Holder->IsA(APangaeaCharacter::StaticClass()) &&
		Holder->IsAttacking())
	{
		auto tower = Cast<ADefenseTower>(OtherActor);
		if (tower != nullptr &&
			tower->CanBeDamaged() &&
			IsWithinAttackRange(0.0f, tower))
		{
			tower->Hit(Strength);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Hit DefenseTower"));
		}
	}
}

