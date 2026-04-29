// Fill out your copyright notice in the Description page of Project Settings.


#include "BRPlayerController.h"

#include "AudioMixerBlueprintLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectBR/ProjectBR.h"
#include "ProjectBR/CoreFramework/Character/BRPlayerCharacter.h"

AActor* ABRPlayerController::FindBestTarget(UCameraComponent* Camera)
{
	if (!Camera) return nullptr;
	
	TArray<FHitResult> OutHits;
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * LockOnRange);
	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bDebugQuery = true;
	
	DrawDebugSphere(GetWorld(), Start, 500.f, 12, FColor::Green, false, 2.0f);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
	
	if (GetPawn())
	{
		Params.AddIgnoredActor(GetPawn());
	}
	
	GetWorld()->SweepMultiByChannel(OutHits, Start, End, FQuat::Identity, LOCK_ON, FCollisionShape::MakeSphere(500.f), Params);
	
	AActor* BestTarget = nullptr;
	float MinDistance = MAX_FLT;

	for (auto& Hit : OutHits)
	{
		AActor* Candidate = Hit.GetActor();
		if (!Candidate) continue;
		
		FVector DirToTarget = (Candidate->GetActorLocation() - Camera->GetComponentLocation()).GetSafeNormal();
		float DotProduct = FVector::DotProduct(Camera->GetForwardVector(), DirToTarget);
		
		if (DotProduct < 0.5f) continue; 
		
		float Distance = FVector::Dist(GetPawn()->GetActorLocation(), Candidate->GetActorLocation());
		
		float Score = Distance * (2.0f - DotProduct);
		
		if (Score < MinDistance)
		{
			MinDistance = Score;
			BestTarget = Candidate;
		}
	}
	
	return BestTarget;
}

void ABRPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (bIsLockOn)
	{
		if (!CurrentTarget.IsValid())
		{
			ToggleLockOn();
			return;
		}
		
		if (GetPawn())
		{
			FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
			FVector LookAtLocation = CurrentTarget->GetActorLocation() - FVector(0, 0, 50.f);
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, LookAtLocation);
			FRotator CurrentControlRotation = GetControlRotation();
			FRotator TargetRotation = FMath::RInterpTo(CurrentControlRotation, LookAtRotation, DeltaSeconds, 5.0f);
		
			SetControlRotation(TargetRotation);
		}
	}
}

void ABRPlayerController::ToggleLockOn()
{
	ABRPlayerCharacter* P = Cast<ABRPlayerCharacter>(GetPawn());
	if (!P) return;
	
	if (bIsLockOn)
	{
		bIsLockOn = false;
		CurrentTarget = nullptr;
		P->SetLockOnState(false);
	}
	else
	{
		CurrentTarget = FindBestTarget(P->GetCamera());

		if (CurrentTarget.IsValid())
		{
			bIsLockOn = true;
			P->SetLockOnState(true);
		}
	}
}
