// Fill out your copyright notice in the Description page of Project Settings.


#include "BRPlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ProjectBR/CoreFramework/Input/BRInputComponent.h"
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
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABRPlayerCharacter::Look);
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
	FVector2D MoveVec = Value.Get<FVector2D>();
	
	AddMovementInput(GetActorForwardVector(), MoveVec.X);
	AddMovementInput(GetActorRightVector(), MoveVec.Y);
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
