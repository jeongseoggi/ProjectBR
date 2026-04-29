
#include "WeaponComponent.h"

#include "ProjectBR/CoreFramework/Character/BRPlayerCharacter.h"
#include "ProjectBR/Weapon/WeaponActor.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnAndAttachWeapon();
}

void UWeaponComponent::SpawnAndAttachWeapon()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	WeaponActorInstance = GetWorld()->SpawnActor<AWeaponActor>(WeaponActorClass, SpawnParams);
	
	ABRPlayerCharacter* OwnerCharacter = Cast<ABRPlayerCharacter>(GetOwner());
	check(OwnerCharacter);
	
	WeaponActorInstance->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
} 


