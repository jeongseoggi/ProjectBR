// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BRPlayerController.generated.h"

/**
 * 
 */
class UCameraComponent;

UCLASS()
class PROJECTBR_API ABRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AActor* FindBestTarget(UCameraComponent* Camera);
	FORCEINLINE bool IsLockOn() const {return bIsLockOn;}
	void ToggleLockOn();
	
protected:
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LockOnRange = 5000.0f;
	
#pragma region LockOnVar
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsLockOn;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> CurrentTarget;
#pragma endregion
};
