// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BRGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBR_API UBRGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UBRGameplayAbility();
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
	
	
};
