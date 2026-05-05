// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (PlayerComponent), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerMove();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaMouseLook;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaMove;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* ia_Run;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* iaJump;

	FVector direction;

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float walkSpeed = 800;

	UPROPERTY(EditAnywhere, Category = PlayerSetting)
	float runSpeed = 1600;

	void Look(const struct FInputActionValue& inputValue);
	void Move(const struct FInputActionValue& inputValue);
	void InputRun(const struct FInputActionValue& inputValue);
	void InputJump(const struct FInputActionValue& inputValue);
	void InputStopJump(const struct FInputActionValue& inputValue);

	void PlayerMove();
};
