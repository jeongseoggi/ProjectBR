// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BRGameplayAbility.h"
#include "BRGA_Attack.generated.h"

class UAbilityTask_WaitGameplayEvent;
/**
 * 
 */
UCLASS()
class PROJECTBR_API UBRGA_Attack : public UBRGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void ExecuteComboJump();
	
	UFUNCTION()
	void OnComboInputReceived(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnComboWindowOpened(FGameplayEventData Payload);
	
	UFUNCTION()
	void OnAttackCompleted();
	
	UFUNCTION()
	void CreateInputWaitTask();
	
protected:
	int32 CurrentComboIndex;
	
	bool bIsComboWindowOpen;
	
	bool bComboBuffer;
	
	int32 MaxComboCount = 3;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ComboMontage;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilityTask_WaitGameplayEvent> CurrentInputTask;
};