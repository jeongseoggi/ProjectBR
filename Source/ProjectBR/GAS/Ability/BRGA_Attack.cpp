// Fill out your copyright notice in the Description page of Project Settings.


#include "BRGA_Attack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "ProjectBR/CoreFramework/Character/BRPlayerCharacter.h"

void UBRGA_Attack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	
	CurrentComboIndex = 1;
	bIsComboWindowOpen = false;
	bComboBuffer = false;
	
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, NAME_None, ComboMontage, 1.0f, FName("Attack1"));
	
	if (MontageTask)
	{
		MontageTask->OnCompleted.AddDynamic(this, &UBRGA_Attack::OnAttackCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UBRGA_Attack::OnAttackCompleted);
		MontageTask->ReadyForActivation();
	}
	
	UAbilityTask_WaitGameplayEvent* EventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, FGameplayTag::RequestGameplayTag("Event.Combo.WindowOpen"));
	
	if (EventTask)
	{
		EventTask->EventReceived.AddDynamic(this, &UBRGA_Attack::OnComboWindowOpened);
		EventTask->ReadyForActivation();
	}
	
	// UAbilityTask_WaitInputPress* InputTask = UAbilityTask_WaitInputPress::WaitInputPress(this);
	// if (InputTask)
	// {
	// 	InputTask->OnPress.AddDynamic(this, &UBRGA_Attack::OnComboInputReceived);
	// 	InputTask->ReadyForActivation();
	// }
	
	UAbilityTask_WaitGameplayEvent* InputEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, FGameplayTag::RequestGameplayTag("Event.Combo.InputPressed"));

	if (InputEventTask)
	{
		// OnComboInputReceived의 매개변수를 (FGameplayEventData Payload)로 변경해야 함
		InputEventTask->EventReceived.AddDynamic(this, &UBRGA_Attack::OnComboInputReceived);
		InputEventTask->ReadyForActivation();
	}
}

void UBRGA_Attack::ExecuteComboJump()
{
	if (CurrentComboIndex >= MaxComboCount) return; 

	CurrentComboIndex++;
	FName NextSection = FName(*FString::Printf(TEXT("Attack%d"), CurrentComboIndex));

	// 몽타주 섹션 이동
	MontageJumpToSection(NextSection);
    
	// 상태 초기화 (다음 타수를 위해 창을 닫고 예약 취소)
	bIsComboWindowOpen = false;
	bComboBuffer = false;

	// 중요: 다음 타수(예: 2타 -> 3타)에서 누를 마우스 클릭을 위해 태스크를 새로 만듭니다.
	CreateInputWaitTask();
    
	UE_LOG(LogTemp, Warning, TEXT("Jumped to Section: %s"), *NextSection.ToString());
}

void UBRGA_Attack::OnComboInputReceived(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Input Detected via Event!"));
    
	if (bIsComboWindowOpen)
	{
		// 입력 창이 열려있다면 즉시 다음 타수로 점프
		ExecuteComboJump();
	}
	else
	{
		// 아직 창이 안 열렸다면 '예약' 설정
		bComboBuffer = true;
        
		// 중요: 태스크는 1회성이므로 다음 입력을 또 받을 수 있게 새로 생성해둡니다.
		CreateInputWaitTask();
	}
}

void UBRGA_Attack::OnComboWindowOpened(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Window Opened via GAS Event!"));
	bIsComboWindowOpen = true;

	if (bComboBuffer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Executing Buffered Jump!"));
		ExecuteComboJump();
	}
}

void UBRGA_Attack::OnAttackCompleted()
{
	CurrentComboIndex = 1;
	bIsComboWindowOpen = false;
	bComboBuffer = false;
	
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBRGA_Attack::CreateInputWaitTask()
{
	if (CurrentInputTask != nullptr) 
	{
		return;
	}

	CurrentInputTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, FGameplayTag::RequestGameplayTag("Event.Combo.InputPressed"));
    
	if (CurrentInputTask)
	{
		CurrentInputTask->EventReceived.AddDynamic(this, &UBRGA_Attack::OnComboInputReceived);
		CurrentInputTask->ReadyForActivation();
	}
}

