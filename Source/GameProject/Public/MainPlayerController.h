// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	protected:
	TObjectPtr<class UInputMappingContext> MainInputContext = nullptr;
};
