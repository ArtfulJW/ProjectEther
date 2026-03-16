// Fill out your copyright notice in the Description page of Project Settings.

#include "PEWarriorChallenge.h"
#include "PEMathLibrary.h"
#include "PEWarriorChallengeFlag.h"

UPEWarriorChallenge::UPEWarriorChallenge(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UPEWarriorChallenge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Check for valid Cooldown Component
	if (!IsValid(AbilityCooldownComponentClass))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Check if this ability is ready to be executed
	if (!AbilityCooldownComponent->CheckIsAbilityReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: DeployRations not ready"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Ability is ready to be executed.
	UE_LOG(LogTemp, Warning, TEXT("%s: Server Executed Ability DeployRations"), *ActorInfo->OwnerActor->GetInstigatorController()->GetName())

	// Get Owning PEPlayerCharacter
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	if (!IsValid(PC))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Get Owning PlayerCharacter's Camera
	UCameraComponent* PlayerCamera = PC->CameraComponent;
	if (!IsValid(PlayerCamera))
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Do a Line trace from Owning PlayerCharacter's Camera
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, ECC_Visibility);
	DrawDebugLine(GetWorld(), PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fRange, FColor::Red);

	// Check if we've set a valid WarriorChallengFlag Actor to spawn
	if (!BP_WarriorChallengeActor)
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Check to see if we've hit a player
	APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());
	if (!HitActor)
	{
		// Unsuccessfully landed a ability we've committed to excecuting. End immediately.
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		AbilityCooldownComponent->ServerSetIsReadyToExecute(false);
		return;
	}
	
	if (!GetActorInfo().IsNetAuthority())
	{
		EndAbility(Handle,ActorInfo, ActivationInfo, true, false);
		return;
	}

	ServerSpawnWarriorChallenge(Hit, PC, HitActor);

	if (!IsValid(PC->PlayerHUD))
	{
		return;
	}
	PC->PlayerHUD->ActivateHitMarker();
}

void UPEWarriorChallenge::ServerSpawnWarriorChallenge_Implementation(FHitResult Hit, APEPlayerCharacter* SourcePlayer, APEPlayerCharacter* TargetPlayer)
{
	// Get the owning PlayerCharacter
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwningActorFromActorInfo());
	if (!IsValid(PC))
	{
		return;
	}

	// Get the target player character
	APEPlayerCharacter* HitActor = Cast<APEPlayerCharacter>(Hit.GetActor());

	// Find the midpoint vector and cast a line trace down to find the ground in between the two players.
	FVector MidpointVector = PEMathLibrary::MidpointVector(PC->GetActorLocation(), HitActor->GetActorLocation());
	
	FHitResult GroundHitCheck;
	GetWorld()->LineTraceSingleByChannel(GroundHitCheck, MidpointVector, MidpointVector + (FVector(0,0, -1) * 200), ECC_Visibility);
	
	// Check for valid blocking hit
	if (!GroundHitCheck.IsValidBlockingHit())
	{
		return;
	}

	// Finally spawn the flag.
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEWarriorChallengeFlag* SpawnedFlag = World->SpawnActor<APEWarriorChallengeFlag>(BP_WarriorChallengeActor, GroundHitCheck.Location, FVector(0, 1, 0).Rotation());
	SpawnedFlag->PlayersWithinRange.Add(SourcePlayer);
	SpawnedFlag->PlayersWithinRange.Add(TargetPlayer);
}