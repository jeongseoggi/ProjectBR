// Fill out your copyright notice in the Description page of Project Settings.


#include "BRPlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


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
	}
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
