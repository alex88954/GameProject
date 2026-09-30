// Implementation for UCameraMovementComponent
// Provides definitions to resolve linker errors and minimal runtime behavior.

#include "CameraMovementComponent.h"
#include "GameFramework/Actor.h"
#include "MainPlayerController.h"
#include "MainMovementComponent.h"
#include "Camera/CameraComponent.h"

/// <summary>
/// When the player starts sprinting the camera punches to the Target Punch FOV
/// then settles to an FOV based on the current speed capped at Max Speed FOV.
/// </summary>

UCameraMovementComponent::UCameraMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* PawnOwner = Cast<APawn>(GetOwner());
	if (!PawnOwner) return;

	AMainPlayerController* PlayerController = Cast<AMainPlayerController>(PawnOwner->GetController());
	if (PlayerController)
	{
		PlayerController->OnSprintStarted.AddDynamic(this, &UCameraMovementComponent::HandleSprintStarted);
		PlayerController->OnSprintEnded.AddDynamic(this, &UCameraMovementComponent::HandleSprintEnded);
	}

	Camera = GetOwner()->GetComponentByClass<UCameraComponent>();
	Movement = GetOwner()->GetComponentByClass<UMainMovementComponent>();

	TargetFOV = BaseFOV;
}

void UCameraMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Camera || !Movement) return;

	// Set the timer only once
	if (bIsPunching && !bPunchReached &&
		FMath::IsNearlyEqual(Camera->FieldOfView, PunchFOV, PunchCompleteTolerance))
	{
		bPunchReached = true;
		if(PunchDuration > 0.f)
		{
			GetWorld()->GetTimerManager().SetTimer(PunchTimer, this, &UCameraMovementComponent::SetPunchingFalse, PunchDuration, false);
		}
		else 
		{
			SetPunchingFalse(); // Allow timer to be 0 for instant settle
		}
	}


	const float WalkSpeed = Movement->DefaultMaxWalkSpeed * Movement->SpeedModifier;
	const float MaxSpeed = WalkSpeed * Movement->SprintMultiplier;
	const float CurrentSpeed = Movement->Velocity.Size2D();

	// Current Speed to FOV
	FVector2D InputRange(0, MaxSpeed);
	FVector2D OutPutRange(BaseFOV, MaxSpeedFOV);

	const float SpeedBasedFOV = FMath::GetMappedRangeValueClamped(InputRange, OutPutRange, CurrentSpeed);

	// If sprinting and not punching target speed based FOV
	if (!bIsPunching && Movement->GetIsSprinting())
	{
		TargetFOV = SpeedBasedFOV;
	}

	// Set new FOV
	const float CameraFOV = Camera->FieldOfView;

	const float InterpSpeed = bIsPunching ? PunchInterpSpeed : SettleInterpSpeed;
	const float NewFOV = bIsPunching ? FMath::FInterpConstantTo(CameraFOV, TargetFOV, DeltaTime, InterpSpeed) : FMath::FInterpTo(CameraFOV, TargetFOV, DeltaTime, InterpSpeed);
	Camera->SetFieldOfView(NewFOV);
}

void UCameraMovementComponent::HandleSprintStarted()
{
	TargetFOV = PunchFOV;
	bIsPunching = true;
	bPunchReached = false;
}

void UCameraMovementComponent::HandleSprintEnded()
{
	GetWorld()->GetTimerManager().ClearTimer(PunchTimer);

	TargetFOV = BaseFOV;
	bIsPunching = false;
	bPunchReached = false;
}

void UCameraMovementComponent::SetPunchingFalse()
{
	bIsPunching = false;
	bPunchReached = false;
	TargetFOV = BaseFOV;
}
