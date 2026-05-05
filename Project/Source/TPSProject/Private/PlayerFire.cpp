#include "PlayerFire.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyFSM.h"
#include "Camera/CameraComponent.h"
#include "CharacterAnim.h"

UPlayerFire::UPlayerFire()
{
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	tpsCamComp = me->tpsCamComp;
	gunMeshComp = me->gunMeshComp;
	sniperGunComp = me->sniperGunComp;

	ChangeToRifleGun(FInputActionValue());

	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	crosshairUI->AddToViewport();
}

void UPlayerFire::SetupInputBinding(UEnhancedInputComponent* PlayerInputComponent)
{
	//Started에서 Triggered로 변경하여 연사 가능하게 수정
	PlayerInputComponent->BindAction(iaFire, ETriggerEvent::Triggered, this, &UPlayerFire::InputFire);
	PlayerInputComponent->BindAction(iaGrenadeGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(iaRifleGun, ETriggerEvent::Started, this, &UPlayerFire::ChangeToRifleGun);
	PlayerInputComponent->BindAction(iaSniper, ETriggerEvent::Started, this, &UPlayerFire::SniperAim);
	PlayerInputComponent->BindAction(iaSniper, ETriggerEvent::Completed, this, &UPlayerFire::SniperAim);
}

void UPlayerFire::InputFire(const struct FInputActionValue& inputValue)
{
	float now = GetWorld()->GetTimeSeconds();

	// 쿨타임 체크
	if (now - lastFireTime < fireCooldown)
	{
		return;  // 아직 쿨타임 안 됨
	}

	lastFireTime = now;  // 발사 시간 갱신

	// 총소리, 카메라 흔들림
	UGameplayStatics::PlaySound2D(GetWorld(), bulletSound);

	auto controller = GetWorld()->GetFirstPlayerController();
	controller->PlayerCameraManager->StartCameraShake(cameraShake);

	// 공격 애니메이션
	auto anim = Cast<UCharacterAnim>(me->GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();

	// 유탄 발사
	if (bUsingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
		return;
	}

	// 일반 총
	FVector startPos = tpsCamComp->GetComponentLocation();
	FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;

	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(me);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

	if (bHit)
	{
		FTransform bulletTrans;
		bulletTrans.SetLocation(hitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);

		auto hitComp = hitInfo.GetComponent();

		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			FVector dir = (endPos - startPos).GetSafeNormal();
			FVector force = dir * hitComp->GetMass() * 500000;
			hitComp->AddForceAtLocation(force, hitInfo.ImpactPoint);
		}

		// Enemy FSM 데미지 처리
		auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
		if (enemy)
		{
			UEnemyFSM* enemyFSM = Cast<UEnemyFSM>(enemy);
			enemyFSM->OnDamageProcess();
		}
	}
}

void UPlayerFire::ChangeToGrenadeGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = true;
	gunMeshComp->SetVisibility(true);
	sniperGunComp->SetVisibility(false);

	me->OnUsingGrenade(bUsingGrenadeGun);
}

void UPlayerFire::ChangeToRifleGun(const struct FInputActionValue& inputValue)
{
	bUsingGrenadeGun = false;
	gunMeshComp->SetVisibility(false);
	sniperGunComp->SetVisibility(true);

	me->OnUsingGrenade(bUsingGrenadeGun);
}

void UPlayerFire::SniperAim(const struct FInputActionValue& inputValue)
{
	if (bUsingGrenadeGun)
		return;

	if (bSniperAim)
	{
		bSniperAim = false;
		sniperUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
		crosshairUI->AddToViewport();
	}
	else
	{
		bSniperAim = true;
		sniperUI->AddToViewport();
		tpsCamComp->SetFieldOfView(45.0f);
		crosshairUI->RemoveFromParent();
	}
}
