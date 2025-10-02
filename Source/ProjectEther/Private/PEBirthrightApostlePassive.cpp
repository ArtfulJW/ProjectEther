// Fill out your copyright notice in the Description page of Project Settings.


#include "PEBirthrightApostlePassive.h"

#include "PEPlayerController.h"

APEBirthrightApostlePassive::APEBirthrightApostlePassive():
bBindToPlayerLookingAtTeammateDelegate(true),
fPassiveLookAtDistance(500.0f),
LookedAtTeammate(nullptr),
bAlreadyAppliedGuardianAngelEffect(false)
{
	bReplicates = true;
	
	SphereColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColliderComponent"));

	SphereColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &APEBirthrightApostlePassive::OverlapEvent);
	SphereColliderComponent->OnComponentEndOverlap.AddDynamic(this, &APEBirthrightApostlePassive::EndOverlapEvent);
}

void APEBirthrightApostlePassive::OverlapEvent(UPrimitiveComponent* OverlappedComp, AActor* Other,
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
	
	if (OverlappedPlayerController->Team != OwningPlayerController->Team)
	{
		return;
	}

	// Apply HealingGameplayEffect here!
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("CrushingPresence::OnHit, Invalid PlayerCharacter"));
	}
	
	HolyPresenceEffectContext = PC->AbilitySystemComponent->MakeEffectContext();
	HolyPresenceEffectSpecHandle = PC->AbilitySystemComponent->MakeOutgoingSpec(GE_HolyPresence, 1.0f, HolyPresenceEffectContext);
	if (!HolyPresenceEffectSpecHandle.IsValid())
	{
		return;
	}
	
	HolyPresenceEffectSpec = *HolyPresenceEffectSpecHandle.Data.Get();
	
	// HolyPresenceEffectSpec = ConstructGameplayEffect(HolyPresenceEffectContext, HolyPresenceEffectSpecHandle, GE_HolyPresence);
	
	ActiveHolyPresenceEffectHandle = OverlappedPlayerCharacter->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(HolyPresenceEffectSpec);
}

void APEBirthrightApostlePassive::EndOverlapEvent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

	APEPlayerCharacter* OwningPlayerCharacter = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(OwningPlayerCharacter))
	{
		return;
	}

	// Remove HealingGameplay Effect from OverlappedPlayerCharacter here!
	OverlappedPlayerCharacter->AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GE_HolyPresence, OwningPlayerCharacter->AbilitySystemComponent, -1);
}

FGameplayEffectSpec APEBirthrightApostlePassive::ConstructGameplayEffect(FGameplayEffectContextHandle InContextHandle,
                                                                         FGameplayEffectSpecHandle InSpecHandle,
                                                                         TSubclassOf<UGameplayEffect> InGameplayEffect) const
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("CrushingPresence::OnHit, Invalid PlayerCharacter"));
	}
	
	InContextHandle = PC->AbilitySystemComponent->MakeEffectContext();
	InSpecHandle = PC->AbilitySystemComponent->MakeOutgoingSpec(InGameplayEffect, 1.0f, InContextHandle);
	if (!InSpecHandle.IsValid())
	{
		return FGameplayEffectSpec();
	}
	return *InSpecHandle.Data.Get();
}

APEPlayerCharacter* APEBirthrightApostlePassive::LookingAtTeammate()
{
	// Owner of type APEBasePassiveComponent always will be the APEPlayerCharacter
	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(GetOwner());
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("IsLookingAtTeammate: Invalid Player Character"))
		return nullptr;
	}
	
	// Initiate a LineTrace
	if (!IsValid(PlayerCharacter->CameraComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerCharacter Camera Component"))
		return nullptr;
	}
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCharacter->CameraComponent->GetComponentLocation(), PlayerCharacter->CameraComponent->GetComponentLocation() + PlayerCharacter->CameraComponent->GetForwardVector() * fPassiveLookAtDistance, ECC_Visibility);
	AActor* Actor = Hit.GetActor();
	
	APEPlayerCharacter* LookedAtPlayerCharacter = Cast<APEPlayerCharacter>(Actor);
	if (!IsValid(LookedAtPlayerCharacter))
	{
		// We are not looking at a PlayerCharacter
		return nullptr;
	}

	// Get PlayerControllers for both 'myself' and the LookedAtPlayerCharacter, and compare teams.
	APEPlayerController* PlayerController = Cast<APEPlayerController>(PlayerCharacter->GetController());
	if (!IsValid(PlayerController))
	{
		return nullptr;
	}
	APEPlayerController* LookedAtPlayerController = Cast<APEPlayerController>(LookedAtPlayerCharacter->GetController());
	if (!IsValid(LookedAtPlayerController))
	{
		return nullptr;
	}
	
	if (PlayerController->Team != LookedAtPlayerController->Team)
	{
		// Not on the same team, do not apply damage reduction Gameplay Effect.
		return nullptr;
	}
	
	return LookedAtPlayerCharacter;
}

void APEBirthrightApostlePassive::BeginPlay()
{
	Super::BeginPlay();
}

void APEBirthrightApostlePassive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	APEPlayerCharacter* TempLookedAtTeammate = LookingAtTeammate();
	if (LookedAtTeammate != TempLookedAtTeammate)
	{
		if (IsValid(LookedAtTeammate) && LookedAtTeammate->IsA(APEPlayerCharacter::StaticClass()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Removing effect on: %s"), *LookedAtTeammate->GetName());
			bAlreadyAppliedGuardianAngelEffect = false;
			
			LookedAtTeammate->AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(GE_GuardianAngel, Cast<APEPlayerCharacter>(GetOwner())->AbilitySystemComponent, -1);
		}
		
		LookedAtTeammate = TempLookedAtTeammate;
	}
	
	if (IsValid(LookedAtTeammate) && IsValid(GE_GuardianAngel) && !bAlreadyAppliedGuardianAngelEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Applying effect on: %s"), *LookedAtTeammate->GetName());
		
		// Apply Damage Reduction Gameplay Effect
		GuardianAngelEffectSpec = ConstructGameplayEffect(GuardianAngelEffectContext, GuardianAngelEffectSpecHandle, GE_GuardianAngel);
		ActiveGuardianAngelEffectHandle = LookedAtTeammate->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(GuardianAngelEffectSpec);

		bAlreadyAppliedGuardianAngelEffect = true;
	}
}
