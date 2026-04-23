// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BRInputData.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FBRInputAction
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* Action = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};


UCLASS()
class PROJECTBR_API UBRInputData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FBRInputAction> AbilityActions;
};
