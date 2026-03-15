// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAvatar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "PlayerAvatarAnimInstance.h"
#include "PangaeaAnimInstance.h"

// Sets default values
APlayerAvatar::APlayerAvatar()
{
	//카메라 스프링 암 생성
	_springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_springArmComponent->SetupAttachment(RootComponent);
	_springArmComponent->SetUsingAbsoluteRotation(true);	//회전 상속 끊기. 주로 **쿼터뷰(Top-down)**나 디아블로 스타일의 게임을 만들 때 필수적
	_springArmComponent->TargetArmLength = 800.f;	//카메라와 캐릭터 간의 거리
	_springArmComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	_springArmComponent->bDoCollisionTest = false;	//카메라가 월드 오브젝트와 충돌하지 않음

	//카메라 생성
	_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_cameraComponent->SetupAttachment(_springArmComponent, USpringArmComponent::SocketName);
	_cameraComponent->bUsePawnControlRotation = false;	//스프링 암의 회전 사용 안함

	//캐릭터 무브먼트 설정
	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true; //움직이는 방향으로 캐릭터 회전
	characterMovement->RotationRate = FRotator(0.f, 640.f, 0.f);
	characterMovement->bConstrainToPlane = true; //2D 게임처럼 평면에서만 이동
	characterMovement->bSnapToPlaneAtStart = true; //게임 시작 시 평면에 고정

	//컨트롤러 회전에 따라 캐릭터 회전 안함
	bUseControllerRotationPitch = false;	
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	_stimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimuli Source"));
	_stimuliSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());  // 시각 감지에 대한 자극원으로 등록
	_stimuliSourceComponent->RegisterWithPerceptionSystem(); // AI 지각 시스템에 등록
};

bool APlayerAvatar::IsAttacking()
{
	return false;
}
