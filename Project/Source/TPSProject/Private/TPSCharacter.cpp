// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMove.h"
#include "PlayerFire.h"
#include "TPSProject.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*ConstructorHelpers::FObjectFinder<USkeletalMesh>
		TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}*/

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 500;

	tpsCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	JumpMaxCount = 10;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("HandGrip_R"));

	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	sniperGunComp->SetupAttachment(GetMesh(), TEXT("HandGrip_R"));

	//playerMove = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
	//playerFire = CreateDefaultSubobject<UPlayerFire>(TEXT("PlayerFire"));
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* pc = Cast<APlayerController>(Controller);
	if (pc != nullptr) {
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsys != nullptr) {
			subsys->AddMappingContext(imcDefault, 0);
			subsys->AddMappingContext(imcMouseLook, 0);
		}
	}
	
	 hp = initialHP;
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// hpTimer는 실제 HP가 아니라
	// '시간 경과를 측정하기 위한 타이머 변수'로 사용
	hpTimer += DeltaTime;

	// 1초가 지났을 경우
	if (hpTimer >= 1.0f)
	{
		// 매 1초마다 HP를 1씩 감소
		hp--;

		// 타이머 초기화 (다음 1초 측정을 위해)
		hpTimer = 0.0f;

		PRINT_LOG(TEXT("HP decreased: %d"), hp);

		// HP가 0 이하가 되면 게임 오버 처리
		if (hp <= 0)
		{
			PRINT_LOG(TEXT("Game Over"));
			OnGameOver();
		}
	}
}



// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr) {

		onInputBindingDelegate.Broadcast(enhancedInputComponent);
		//playerMove->SetupInputBinding(enhancedInputComponent);
		//playerFire->SetupInputBinding(enhancedInputComponent);
	}

}

void ATPSCharacter::OnHitEvent() 
{ 
	PRINT_LOG(TEXT("Damaged !!!!!")); 
	hp--; 
	if (hp <= 0) 
	{ 
		PRINT_LOG(TEXT("Game Over")); 
		OnGameOver(); 
	} 
}


void ATPSCharacter::OnGameOver_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}