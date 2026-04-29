// Fill out your copyright notice in the Description page of Project Settings.


#include "BRPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectBR/CoreFramework/Component/WeaponComponent.h"
#include "ProjectBR/CoreFramework/Input/BRInputComponent.h"
#include "ProjectBR/CoreFramework/PlayerController/BRPlayerController.h"
#include "ProjectBR/CoreFramework/PlayerState/BRPlayerState.h"
#include "ProjectBR/GAS/Ability/BRGameplayAbility.h"
#include "ProjectBR/GAS/Component/BRAbilitySystemComponent.h"


ABRPlayerCharacter::ABRPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
}

void ABRPlayerCharacter::SetLockOnState(bool bNewState)
{
	bIsStrafing = bNewState;
	
	bUseControllerRotationYaw = bNewState;
	GetCharacterMovement()->bOrientRotationToMovement = !bNewState;
	
	if (SpringArm)
	{
		SpringArm->TargetArmLength = bNewState ? 450.f : 300.f;
	}
}

void ABRPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent = GetAbilitySystemComponent();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GiveDefaultAbilites();
	}
}

void ABRPlayerCharacter::GiveDefaultAbilites()
{
	check(AbilitySystemComponent);
	
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultAbilities)
	{
		if (AbilitySystemComponent->FindAbilitySpecFromClass(AbilityClass))
		{
			continue;
		}
		
		FGameplayAbilitySpec Spec(AbilityClass, 1, INDEX_NONE, this);
		
		if (const UBRGameplayAbility* BRAbility = Cast<UBRGameplayAbility>(AbilityClass->GetDefaultObject()))
		{
			if (BRAbility->StartupInputTag.IsValid())
			{
				Spec.DynamicAbilityTags.AddTag(BRAbility->StartupInputTag);
			}
		}
		
		AbilitySystemComponent->GiveAbility(Spec);
	}
	
}

void ABRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	APlayerController* PC = Cast<APlayerController>(GetController());
	check(PC);
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		if (DefaultMappingContext)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABRPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABRPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ABRPlayerCharacter::LockOn);
	}
	
	UBRInputComponent* BRInputComp = CastChecked<UBRInputComponent>(PlayerInputComponent);

	TArray<uint32> BindHandles;
	BRInputComp->BindAbilityActions(
		InputData,
		this,
		&ThisClass::Input_AbilityInputTagPressed,
		&ThisClass::Input_AbilityInputTagReleased,
		BindHandles
	);
}

UAbilitySystemComponent* ABRPlayerCharacter::GetAbilitySystemComponent() const
{
	if (ABRPlayerState* PS = Cast<ABRPlayerState>(GetPlayerState()))
	{
		return PS->GetAbilitySystemComponent();
	}
	
	return nullptr;
}

void ABRPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABRPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVec = Value.Get<FVector2D>();
	
	if (Controller)
	{
		AddControllerYawInput(LookVec.X);
		AddControllerPitchInput(-LookVec.Y);
	}
}

void ABRPlayerCharacter::LockOn(const FInputActionValue& Value)
{
	// ABRPlayerController* PC = Cast<ABRPlayerController>(GetController());
	// if (!PC) return;
	//
	// PC->ToggleLockOn(); 
	//
	// if (PC->IsLockOn())
	// {
	// 	GetCharacterMovement()->bOrientRotationToMovement = false;
	// 	bUseControllerRotationYaw = true;
	// }
	// else
	// {
	// 	GetCharacterMovement()->bOrientRotationToMovement = true;
	// 	bUseControllerRotationYaw = false;
	// }
	
	ABRPlayerController* PC = Cast<ABRPlayerController>(GetController());
	if (PC)
	{
		PC->ToggleLockOn();
	}
}

void ABRPlayerCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UBRAbilitySystemComponent* ASC = Cast<UBRAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void ABRPlayerCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UBRAbilitySystemComponent* ASC = Cast<UBRAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}
