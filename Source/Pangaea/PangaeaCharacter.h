// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PangaeaCharacter.generated.h"

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class APangaeaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APangaeaCharacter();

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	int HealthPoints = 100;

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float Strength = 5;

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float Armer = 1;

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float AttackRange = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Pangaea Character Params")
	float AttackInterval = 3.0f;

	/** Initialization */
	virtual void BeginPlay() override;

	/** Update */
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Pangaea|Character", meta = (DisplayName = "Get HP"))
	int GetHealthPoints();

	UFUNCTION(BlueprintCallable, Category = "Pangaea|Character")
	bool IsKilled();

	UFUNCTION(BlueprintCallable, Category = "Pangaea|PlayerCharacter")
	bool CanAttack();
	
	bool IsAttacking();
	virtual void Attack();
	virtual void Hit(int damage);
	virtual void DieProcess();

	UFUNCTION(NetMulticast, Reliable)
	void AttackBroadcastRpc();

	UFUNCTION()
	void OnHealthPointsChanged();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnHealthPointsChanged)
	int _HealthPoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UUserWidget* HealthBarWidget;

	float _AttackCountingDown;
	class UPangaeaAnimInstance* _AnimInstance;
};

