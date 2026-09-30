// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEPROJECT_API UCameraMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCameraMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOV", meta = (ClampMin = "1.0", ClampMax = "170"))
	float BaseFOV = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOV", meta = (ClampMin = "1.0", ClampMax = "170"))
	float PunchFOV = 110.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOV", meta = (ClampMin = "1.0", ClampMax = "170"))
	float MaxSpeedFOV = 100.f;

	/** Value for constant speed interpolation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOV", meta = (ClampMin = "0.0"))
	float PunchInterpSpeed = 130.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOV", meta = (ClampMin = "0.0"))
	float SettleInterpSpeed = 5.f;

protected:
	bool bPunchReached = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOV", meta = (ClampMin = "0.01"))
	float PunchCompleteTolerance = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FOV", meta = (ClampMin = "0.0"))
	float PunchDuration = 0.5f;


public:
	UPROPERTY()
	class UCameraComponent* Camera;
	UPROPERTY()
	class UMainMovementComponent* Movement;

protected:
	bool bIsPunching;
	float TargetFOV;

protected:
	FTimerHandle PunchTimer;
	
protected:

	UFUNCTION()
	void HandleSprintStarted();

	UFUNCTION()
	void HandleSprintEnded();

protected:
	void SetPunchingFalse();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsPunching() const { return bIsPunching; }
};
