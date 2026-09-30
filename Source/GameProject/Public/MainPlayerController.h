// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include <EnhancedInputSubsystems.h>
#include "InputAction.h"
#include "InputActionValue.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSprintEnded);

UCLASS(Abstract)
class GAMEPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

protected:
	virtual UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem();

protected:
	virtual void SetMouseCursorVisible(bool bIsVisible);

protected:
	class AMainCharacter* Character = nullptr;
	class UMainMovementComponent* CharacterMovement = nullptr;

public:
	UPROPERTY(BlueprintAssignable, Category = "Sprint Delegate")
	FOnSprintStarted OnSprintStarted;
	UPROPERTY(BlueprintAssignable, Category = "Sprint Delegate")
	FOnSprintEnded OnSprintEnded;

protected:
	UPROPERTY(EditAnywhere, Category = "Input: Mapping Context")
	UInputMappingContext* MainInputContext;

	UPROPERTY(EditAnywhere, Category = "Input: Mapping Context")
	UInputMappingContext* AimMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input: Movement")
	UInputAction* IAMove;

	UPROPERTY(EditAnywhere, Category = "Input: Movement")
	UInputAction* IAJump;

	UPROPERTY(EditAnywhere, Category = "Input: Movement")
	UInputAction* IASprint;

	UPROPERTY(EditAnywhere, Category = "Input: Aim")
	UInputAction* IARightClickLook;

	UPROPERTY(EditAnywhere, Category = "Input: Aim")
	UInputAction* IALook;

protected:
	void Move(const struct FInputActionValue& Input);
	void Look(const struct FInputActionValue& Input);
	void ActivateLook(const struct FInputActionValue& Input);
	void Jump(const struct FInputActionValue& Input);
	void Sprint(const struct FInputActionValue& Input);
};
