// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

UCLASS(ClassGroup = (PlayerComponent), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:

	UPlayerFire();

protected:

	virtual void BeginPlay() override;

public:

	virtual void SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class UCameraComponent* tpsCamComp;

	UPROPERTY()
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY()
	class UStaticMeshComponent* sniperGunComp;

	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditAnywhere, Category = BulletEffect)
	class UParticleSystem* bulletEffectFactory;

	UPROPERTY(EditAnywhere, Category = SniperUI)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	UPROPERTY()
	class UUserWidget* sniperUI;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	UPROPERTY()
	class UUserWidget* crosshairUI;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaFire;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaGrenadeGun;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaRifleGun;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaSniper;

	bool bSniperAim = false;
	bool bUsingGrenadeGun = true;

	UPROPERTY(EditDefaultsOnly, Category = CameraMotion)
	TSubclassOf<class UCameraShakeBase>cameraShake;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundBase* bulletSound;

	// 0.1초 연사 쿨타임
	UPROPERTY(EditAnywhere, Category = Fire)
	float fireCooldown = 0.1f;

	float lastFireTime = 0.0f;

	void InputFire(const struct FInputActionValue& inputValue);
	void ChangeToGrenadeGun(const struct FInputActionValue& inputValue);
	void ChangeToRifleGun(const struct FInputActionValue& inputValue);
	void SniperAim(const struct FInputActionValue& inputValue);
};
