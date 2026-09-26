// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API UMainMovementComponent : public UCharacterMovementComponent 
{
	GENERATED_BODY()

public:
	UMainMovementComponent();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	float SprintMultiplier = 2.f;

	float DefaultMaxWalkSpeed = 0.f;

	float SpeedModifier = 1.f;

public:
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprinting();

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprinting();

private:
	bool bIsSprinting = false;

private:
	void UpdateSpeed();
};
