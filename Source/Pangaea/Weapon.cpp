// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "GameFramework/Character.h"
#include "PlayerAvatar.h"


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
	auto character = Cast<ACharacter>(OtherActor);
	if (character == nullptr)
	{
		return;
	}
	if (Holder == nullptr)
	{

		Holder = Cast<APlayerAvatar>(character);
		if (Holder == nullptr)
		{
			return;
		}

		TArray<AActor*> attachedActors;
		Holder->GetAttachedActors(attachedActors);

		for (int i = 0; i < attachedActors.Num(); i++)
		{
			AWeapon* weapon = Cast<AWeapon>(attachedActors[i]);
			weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

			auto location = weapon->GetActorLocation();
			location.Z = 50.0f;
			weapon->SetActorLocationAndRotation(location, FQuat::Identity);

			weapon->Holder = nullptr;
		}

		AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
	}
	else if (IsWithinAttackRange(0.0f, OtherActor))
	{
		// 공격 로직 구현
	}
}

