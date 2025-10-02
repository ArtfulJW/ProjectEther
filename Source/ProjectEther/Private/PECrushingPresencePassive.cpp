// Fill out your copyright notice in the Description page of Project Settings.


#include "PECrushingPresencePassive.h"

#include "PEPlayerCharacter.h"
#include "PEPlayerController.h"

APECrushingPresencePassive::APECrushingPresencePassive():
NumHits(0),
NumHitsToReset(3),
fOverlapDelayToApplyBrokenWill(15),
bBindToPlayerOnHitDelegate(true)
{
	bReplicates = true;
	
	SphereColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColliderComponent"));

	SphereColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &APECrushingPresencePassive::OverlapEvent);
	SphereColliderComponent->OnComponentEndOverlap.AddDynamic(this, &APECrushingPresencePassive::EndOverlapEvent);
}

void APECrushingPresencePassive::SetupPassiveAbility()
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(PC))
	{
		return;
	}

	if (bBindToPlayerOnHitDelegate)
	{
		PC->OnEnemyHitDelegate.BindUObject(this, &APECrushingPresencePassive::OnHit);
	}
}

void APECrushingPresencePassive::OnHit(APEPlayerCharacter* HitPlayer)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("CrushingPresence::OnHit, Invalid PlayerCharacter"));
	}

	NumHits++;
	UE_LOG(LogTemp, Warning, TEXT("Calling APECrushingPresencePassive::OnHit after PlayerCharacter.OnEnemyHitDelegate. NumHits:(%d)"), NumHits);
	
	if (NumHits >= NumHitsToReset)
	{
		// TODO: Apply a stacking Crushing Presence Gameplay Effect to player on every third hit.
		
		ServerApplyOnHitGameplayEffect(HitPlayer);
		
		NumHits = 0;
	}
}

void APECrushingPresencePassive::OverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* Other,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Other->IsA(APEPlayerCharacter::StaticClass()))
 	{
 		return;
 	}
	
	APEPlayerCharacter* OverlappedPlayerCharacter = Cast<APEPlayerCharacter>(Other);
	if (!IsValid(OverlappedPlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped actor is not a APEPlayerCharacter"));
		return;
	}
	
	APEPlayerCharacter* OwningPlayerCharacter = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(OwningPlayerCharacter))
	{
		return;
	}
	
	if (OwningPlayerCharacter == OverlappedPlayerCharacter)
	{
		return;
	}

	APEPlayerController* OverlappedPlayerController = Cast<APEPlayerController>(OverlappedPlayerCharacter->GetController());
	APEPlayerController* OwningPlayerController = Cast<APEPlayerController>(OwningPlayerCharacter->GetController());
	
	if (!IsValid(OverlappedPlayerController) || !IsValid(OwningPlayerController))
	{
		return;
	}
	
	if (OverlappedPlayerController->Team == OwningPlayerController->Team)
	{
		return;
	}
	
	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;

	TimerDelegate.BindUFunction(this, FName("ApplyGameplayEffectAfterTimer"), OverlappedPlayerCharacter);

	const UGameplayEffect* ActiveGameplayEffect = OverlappedPlayerCharacter->AbilitySystemComponent->GetGameplayEffectDefForHandle(ActiveBrokenWillEffectHandle);
	if (IsValid(ActiveGameplayEffect))
	{
		return;
	}

	if (!OverlappedPlayerCharacters.Contains(OverlappedPlayerCharacter))
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, fOverlapDelayToApplyBrokenWill, false);
		OverlappedPlayerCharacters.AddUnique(OverlappedPlayerCharacter);
	}
}

void APECrushingPresencePassive::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	if (!OtherActor->IsA(APEPlayerCharacter::StaticClass()))
	{
		return;
	}
	
	APEPlayerCharacter* OverlappedPlayerCharacter = Cast<APEPlayerCharacter>(OtherActor);
	if (!IsValid(OverlappedPlayerCharacter))
	{
		return;
	}
	
	OverlappedPlayerCharacters.Remove(OverlappedPlayerCharacter);
}

void APECrushingPresencePassive::ServerApplyOnHitGameplayEffect_Implementation(
	const APEPlayerCharacter* OverlappedPlayerCharacter)
{
	if (!IsValid(OverlappedPlayerCharacter))
	{
		return;
	}
	ConstructGameplayEffectSpec();
	
	// On hit: Immediately apply.
	if (BrokenWillEffectSpec)
	{
		ActiveBrokenWillEffectHandle = OverlappedPlayerCharacter->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*BrokenWillEffectSpec);
	}
}

void APECrushingPresencePassive::ApplyGameplayEffectAfterTimer(const APEPlayerCharacter* OverlappedPlayerCharacter)
{
	ConstructGameplayEffectSpec();
	
	if (!BrokenWillEffectSpec)
	{
		return;
	}
	
	// Only apply 'Broken Will' after timer if the overlapped character is still within range.
	if (OverlappedPlayerCharacters.Contains(OverlappedPlayerCharacter))
	{
		ActiveBrokenWillEffectHandle = OverlappedPlayerCharacter->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*BrokenWillEffectSpec);
	}
}

void APECrushingPresencePassive::ConstructGameplayEffectSpec()
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("CrushingPresence::OnHit, Invalid PlayerCharacter"));
	}
	
	EffectContext = PC->AbilitySystemComponent->MakeEffectContext();
	BrokenWillEffectHandle = PC->AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.0f, EffectContext);

	if (!BrokenWillEffectHandle.IsValid())
	{
		return;
	}

	BrokenWillEffectSpec = BrokenWillEffectHandle.Data.Get();
}

void APECrushingPresencePassive::BeginPlay()
{
	Super::BeginPlay();
	SetupPassiveAbility();
}
