// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BRPlayerCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class USpringArmComponent;

UCLASS()
class PROJECTBR_API ABRPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABRPlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
#pragma region Default
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;
#pragma endregion
	
#pragma region Input
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
#pragma endregion
};
