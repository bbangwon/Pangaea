// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "PlayerAvatar.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	_LifeCountingDown = Lifespan;
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_LifeCountingDown > 0.0f)
	{
		_LifeCountingDown -= DeltaTime;
	}
	else
	{
		PrimaryActorTick.bCanEverTick = false;
		Destroy();
	}

	FVector CurrentLocation = GetActorLocation();
	FVector Vel = GetActorRotation().RotateVector(FVector::ForwardVector) * Speed * DeltaTime;
	FVector NextLocation = CurrentLocation + Vel;
	SetActorLocation(NextLocation);

	FHitResult HitResult;
	FCollisionObjectQueryParams ObjCollisionQueryParams;
	ObjCollisionQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	if (GetWorld()->LineTraceSingleByObjectType(HitResult, CurrentLocation, NextLocation, ObjCollisionQueryParams))
	{
		auto PlayerAvatar = Cast<APlayerAvatar>(HitResult.GetActor());
		if (PlayerAvatar != nullptr)
		{
			PlayerAvatar->Hit(Damage);
			PrimaryActorTick.bCanEverTick = false;
			Destroy();
		}
	}
}

