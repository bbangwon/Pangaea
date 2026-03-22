// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PangaeaCharacter.h"
#include "PlayerAvatar.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API APlayerAvatar : public APangaeaCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerAvatar();

public:	
	void DropWeapon();
	void AttactWeapon(class AWeapon* Weapon);

public:
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const 
	{
		return _springArmComponent;
	}

	FORCEINLINE UCameraComponent* GetCameraComponent() const 
	{
		return _cameraComponent;
	}

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* _springArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* _cameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI Perception", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* _stimuliSourceComponent;
};
