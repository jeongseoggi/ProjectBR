// Fill out your copyright notice in the Description page of Project Settings.


#include "BRAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectBR/CoreFramework/Character/BRPlayerCharacter.h"

void UBRAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	OwnerCharacter = Cast<ABRPlayerCharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerCharacterMovementComp = OwnerCharacter->GetCharacterMovement();
	}
	
}

void UBRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (IsValid(OwnerCharacterMovementComp))
	{
		Velocity = OwnerCharacterMovementComp->Velocity;
		GroundSpeed = FVector(Velocity.X, Velocity.Y, 0).Size();
		//bShouldMove = ((OwnerCharacterMovementComp->GetCurrentAcceleration().IsNearlyZero()) == false && (3.f < GroundSpeed));
		bHasAcceleration = !OwnerCharacterMovementComp->GetCurrentAcceleration().IsNearlyZero();
		bShouldMove = bHasAcceleration;
		bIsFalling = OwnerCharacterMovementComp->IsFalling();
		Direction = CalculateDirection(Velocity, OwnerCharacter->GetActorRotation());
	}
	
}
