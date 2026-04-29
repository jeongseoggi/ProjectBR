// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BRPlayerCharacter.generated.h"

class UWeaponComponent;
struct FGameplayTag;
class UGameplayAbility;
class UBRInputData;
struct FInputActionValue;
class UInputAction;
class UCameraComponent;
class UInputMappingContext;
class USpringArmComponent;

UCLASS()
class PROJECTBR_API ABRPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABRPlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	void SetLockOnState(bool bNewState);
	
protected:
	virtual void BeginPlay() override;
	
	void GiveDefaultAbilites();
	
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void LockOn(const FInputActionValue& Value);
	
	UFUNCTION()
	void Attack(const FInputActionValue& Value);
	
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
private:

#pragma region Default
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWeaponComponent> WeaponComponent;
#pragma endregion
	
#pragma region Input
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> SprintAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> LockOnAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> AttackAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBRInputData> InputData;
#pragma endregion
	
#pragma region Abilities
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
#pragma endregion
	
#pragma region LockOn
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsStrafing;
#pragma endregion
};
