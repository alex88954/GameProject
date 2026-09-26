// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "MainCharacter.h"
#include "MainMovementComponent.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GAMEPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetPawn(APawn* InPawn) override;
protected:
	virtual void SetupInputComponent() override;

protected:
	AMainCharacter* Character;
	UMainMovementComponent* CharacterMovement;

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* MainInputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IAMove;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IALook;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IAJump;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IASprint;

protected:
	void Move(const struct FInputActionValue& Input);
	void Look(const struct FInputActionValue& Input);
	void Jump(const struct FInputActionValue& Input);
	void Sprint(const struct FInputActionValue& Input);
};
