// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


class AWeaponActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTBR_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();
	FORCEINLINE AWeaponActor* GetWeaponActor() const {return WeaponActorInstance;}

protected:
	virtual void BeginPlay() override;
	void SpawnAndAttachWeapon();
	
private:
	
public:
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeaponActor> WeaponActorClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AWeaponActor> WeaponActorInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Attach")
	FName SocketName;
private:
};
