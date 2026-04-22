// Fill out your copyright notice in the Description page of Project Settings.


#include "BRPlayerState.h"

#include "AbilitySystemComponent.h"
#include "ProjectBR/GAS/AttributeSet/BRAttributeSet.h"
#include "ProjectBR/GAS/Component/BRAbilitySystemComponent.h"

ABRPlayerState::ABRPlayerState()
{
	ASC = CreateDefaultSubobject<UBRAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UBRAttributeSet>(FName("AttributeSet"));
}

UAbilitySystemComponent* ABRPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}
