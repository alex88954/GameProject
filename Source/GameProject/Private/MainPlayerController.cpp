// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "MainCharacter.h"
#include "MainMovementComponent.h"

void AMainPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	Character = Cast<AMainCharacter>(InPawn);
	if (Character)
	{
		CharacterMovement = Cast<UMainMovementComponent>(Character->GetCharacterMovement());
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Get Enhanced Input subsystem
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!EnhancedInputSubSystem) return;

	// Apply InputMappingContext
	EnhancedInputSubSystem->AddMappingContext(MainInputContext, 0);

	// Get Enhanced Input Component
	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent) return;

	// Bind Actions
	EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &AMainPlayerController::Move);
	EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);
	EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Triggered, this, &AMainPlayerController::Jump);
	EnhancedInputComponent->BindAction(IASprint, ETriggerEvent::Triggered, this, &AMainPlayerController::Sprint);

}

void AMainPlayerController::Move(const FInputActionValue& Input)
{
	if (!Character) return;

	const FVector2D Movement = Input.Get<FVector2D>();

	// Find controller forward direction
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Transform FRotator into Vector
	// Get Forward Vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Get Right Vector
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// Add Movement
	if (Movement.X)
	{
		Character->AddMovementInput(ForwardDirection, Movement.X);
	}
	if (Movement.Y)
	{
		Character->AddMovementInput(RightDirection, Movement.Y);
	}
}

void AMainPlayerController::Look(const FInputActionValue& Input)
{
	if (!Character) return;

	const FVector2D Look = Input.Get<FVector2D>();

	if (Look.X)
	{
		AddYawInput(Look.X);
	}
	if (Look.Y)
	{
		AddPitchInput(Look.Y);
	}
}

void AMainPlayerController::Jump(const FInputActionValue& Input)
{
	if (!Character) return;
	Character->Jump();
}

void AMainPlayerController::Sprint(const FInputActionValue& Input)
{
	if (!Character || !CharacterMovement) return;

	const bool bIsSprinting = Input.Get<bool>();

	if (bIsSprinting) 
	{
		CharacterMovement->StartSprinting();
	}

	if (!bIsSprinting)
	{
		CharacterMovement->StopSprinting();
	}
}
