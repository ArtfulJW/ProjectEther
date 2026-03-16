// Fill out your copyright notice in the Description page of Project Settings.


#include "PEWarriorChallengeFlag.h"

#include "PEMathLibrary.h"
#include "PEPlayerCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
APEWarriorChallengeFlag::APEWarriorChallengeFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	SphereColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	
	RootComponent = SceneComponent;
	StaticMeshComponent->SetupAttachment(RootComponent);
	SphereColliderComponent->SetupAttachment(RootComponent);
	
	StaticMeshComponent->SetIsReplicated(true);
	SphereColliderComponent->SetIsReplicated(true);
	SceneComponent->SetIsReplicated(true);
}

void APEWarriorChallengeFlag::ConstructGameplayEffectSpec(APEPlayerCharacter* TargetPlayer)
{
	// Make the effect context for the first player to leave the range of WarriorChallengeFlag
	EffectContext = TargetPlayer->AbilitySystemComponent->MakeEffectContext();

	// Create Handles for corresponding GameplayEffects
	ShamedEffectHandle = TargetPlayer->AbilitySystemComponent->MakeOutgoingSpec(ShamedGameplayEffect, 1.0f, EffectContext);
	VulnerableEffectHandle = TargetPlayer->AbilitySystemComponent->MakeOutgoingSpec(VulnerableGameplayEffect, 1.0f, EffectContext);
}

void APEWarriorChallengeFlag::CheckPlayerDistance()
{
	// Compare each associated Player Character with the position of the Flag, and see if they're too far.
	for (APEPlayerCharacter* PC : PlayersWithinRange)
	{
		// If any of the players that we are tracking are dead, we don't need to keep existing.
		if (!IsValid(PC))
		{
			Destroy();
			return;
		}
		
		if (PEMathLibrary::EuclideanDistance(PC->GetActorLocation(), GetActorLocation()) > fChallengeRange)
		{
			// If far enough, immediately apply debuffs and destroy itself.
			ApplyDebuffs(PC);
			Destroy();
			return;
		}
	}
}

void APEWarriorChallengeFlag::ApplyDebuffs(APEPlayerCharacter* PC)
{
	ConstructGameplayEffectSpec(PC);
	if (ShamedEffectHandle.IsValid() && VulnerableEffectHandle.IsValid())
	{
		ActiveShamedEffectHandle = PC->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*ShamedEffectHandle.Data.Get());
		ActiveVulnerableEffectHandle = PC->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VulnerableEffectHandle.Data.Get());
	}
}

void APEWarriorChallengeFlag::DestroyAfterTime()
{
	Destroy();
}

void APEWarriorChallengeFlag::BeginPlay()
{
	Super::BeginPlay();

	SphereColliderComponent->SetSphereRadius(fChallengeRange);

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	FTimerHandle TimerHandle;
	World->GetTimerManager().SetTimer(TimerHandle, this, &APEWarriorChallengeFlag::DestroyAfterTime, fLifetime, false);
}

// Called every frame
void APEWarriorChallengeFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() >= NM_Client)
	{
		return;
	}
	
	// Every tick, check if someone is far enough to trigger.
	CheckPlayerDistance();
}