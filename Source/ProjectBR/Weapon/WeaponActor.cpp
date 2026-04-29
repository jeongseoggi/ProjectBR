#include "WeaponActor.h"


AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>("SceneComp");
	SetRootComponent(SceneComp);
	
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("SM");
	WeaponMesh->SetupAttachment(SceneComp);
}

void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}
