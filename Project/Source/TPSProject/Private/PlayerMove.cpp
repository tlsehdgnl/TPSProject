// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	moveComp->MaxWalkSpeed = walkSpeed;

}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PlayerMove();

}
void UPlayerMove::SetupInputBinding(UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(iaMouseLook, ETriggerEvent::Triggered, this, &UPlayerMove::Look);
	PlayerInputComponent->BindAction(iaMove, ETriggerEvent::Triggered, this, &UPlayerMove::Move);
	PlayerInputComponent->BindAction(ia_Run, ETriggerEvent::Started, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(ia_Run, ETriggerEvent::Completed, this, &UPlayerMove::InputRun);
	PlayerInputComponent->BindAction(iaJump, ETriggerEvent::Started, this, &UPlayerMove::InputJump);
	PlayerInputComponent->BindAction(iaJump, ETriggerEvent::Completed, this, &UPlayerMove::InputStopJump);
}

void UPlayerMove::Look(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	me->AddControllerYawInput(value.X);
	me->AddControllerPitchInput(value.Y);
}

void UPlayerMove::Move(const struct FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	direction.X = value.X;
	direction.Y = value.Y;
}

void UPlayerMove::InputRun(const FInputActionValue& inputValue)
{
	//auto movement = GetCharacterMovement();
	if (moveComp->MaxWalkSpeed > walkSpeed) {
		moveComp->MaxWalkSpeed = walkSpeed;
	}
	else {
		moveComp->MaxWalkSpeed = runSpeed;
	}
}

void UPlayerMove::InputJump(const FInputActionValue& inputValue)
{
	me->Jump();
}

void UPlayerMove::InputStopJump(const FInputActionValue& inputValue)
{
	me->StopJumping();
}

void UPlayerMove::PlayerMove()
{
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);
	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;
}