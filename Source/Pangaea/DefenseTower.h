// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DefenseTower.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API ADefenseTower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefenseTower();

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	int HealthPoints = 500;

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	int ShellDefense = 3;

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float AttackRange = 156.0f;

	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float ReloadInterval = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int _HealthPoints;
	float _ReloadCountingDown;	

	class APlayerAvatar* _Target = nullptr;
	UClass* _FireballClass = nullptr;

	class APangaeaGameMode* _GameMode = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Defense Tower", meta = (DisplayName = "GetHP"))
	int GetHealthPoints();

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Defense Tower")
	bool IsDestroyed();

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Defense Tower")
	bool CanFire();

	void Fire();
	void Hit(int damage);

protected:
	void DestroyProcess();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void OnMeshBeginOverlap(AActor* OtherActor);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* _SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* _MeshComponent;

public:
	FORCEINLINE USphereComponent* GetSphereComponent() const
	{
		return _SphereComponent;
	}

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return _MeshComponent;
	}

	

	
};
