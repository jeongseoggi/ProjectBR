// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_ComboWindow.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"

void UANS_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EventReference)
{
	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(MeshComp->GetOwner()))
	{
		UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
		ASC->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("Event.Combo.WindowOpen"), nullptr);
	}
}

void UANS_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	
}
