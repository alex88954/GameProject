// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMovementComponent.h"

UMainMovementComponent::UMainMovementComponent()
{
	DefaultMaxWalkSpeed = MaxWalkSpeed;
}

void UMainMovementComponent::StartSprinting()
{
	bIsSprinting = true;
	UpdateSpeed();
}

void UMainMovementComponent::StopSprinting()
{
	bIsSprinting = false;
	UpdateSpeed();
}

void UMainMovementComponent::UpdateSpeed()
{
	float speed = DefaultMaxWalkSpeed * SpeedModifier;
	if (bIsSprinting)
	{
		speed *= SprintMultiplier;
	}
	MaxWalkSpeed = speed;
}
