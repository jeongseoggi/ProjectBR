// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BRAnimInstance.generated.h"

class UCharacterMovementComponent;
class ABRPlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECTBR_API UBRAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	void NativeInitializeAnimation() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
	
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ABRPlayerCharacter> OwnerCharacter;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float GroundSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShouldMove;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasAcceleration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsFalling;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Direction;
};
