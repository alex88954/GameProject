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
	auto* Subsystem = GetEnhancedInputSubsystem();
	if (!Subsystem) return;

	// Apply InputMappingContext
	Subsystem->AddMappingContext(MainInputContext, 0);

	// Get Enhanced Input Component
	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent) return;

	// Bind Actions
	EnhancedInputComponent->BindAction(IAMove, ETriggerEvent::Triggered, this, &AMainPlayerController::Move);
	EnhancedInputComponent->BindAction(IAJump, ETriggerEvent::Triggered, this, &AMainPlayerController::Jump);
	EnhancedInputComponent->BindAction(IASprint, ETriggerEvent::Triggered, this, &AMainPlayerController::Sprint);

	EnhancedInputComponent->BindAction(IARightClickLook, ETriggerEvent::Triggered, this, &AMainPlayerController::ActivateLook);

	EnhancedInputComponent->BindAction(IALook, ETriggerEvent::Triggered, this, &AMainPlayerController::Look);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetMouseCursorVisible(true);
}

UEnhancedInputLocalPlayerSubsystem* AMainPlayerController::GetEnhancedInputSubsystem()
{
	UEnhancedInputLocalPlayerSubsystem* EnhancedPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!EnhancedPlayerSubsystem) return nullptr;
	return EnhancedPlayerSubsystem;
}

void AMainPlayerController::SetMouseCursorVisible(bool bIsVisible)
{
	bShowMouseCursor = bIsVisible;

	if (bIsVisible)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}
	else
	{
		FInputModeGameOnly InputMode;
		InputMode.SetConsumeCaptureMouseDown(false);
		SetInputMode(InputMode);
	}
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

void AMainPlayerController::ActivateLook(const FInputActionValue& Input)
{
	auto* Subsystem = GetEnhancedInputSubsystem();
	if (!Subsystem) return;

	const bool bWantsToLook = Input.Get<bool>();

	if (bWantsToLook)
	{
		Subsystem->AddMappingContext(AimMappingContext, 1);
		SetMouseCursorVisible(false);
	}
	else
	{
		Subsystem->RemoveMappingContext(AimMappingContext);
		SetMouseCursorVisible(true);
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
		OnSprintStarted.Broadcast();
	}

	if (!bIsSprinting)
	{
		CharacterMovement->StopSprinting();
		OnSprintEnded.Broadcast();
	}
}
