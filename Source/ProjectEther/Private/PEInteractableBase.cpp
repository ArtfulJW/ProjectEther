// Fill out your copyright notice in the Description page of Project Settings.

#include "PEInteractableBase.h"
#include "Net/UnrealNetwork.h"

APEInteractableBase::APEInteractableBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SphereColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	
	RootComponent = StaticMeshComponent;
	SphereColliderComponent->SetupAttachment(StaticMeshComponent);

	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetIsReplicated(true);
	SphereColliderComponent->SetIsReplicated(true);
}

APEInteractableBase::~APEInteractableBase()
{
}

// Called every frame
void APEInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APEInteractableBase::ApplyCarryEffect()
{
	if (!IsValid(Carrier))
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = Carrier->AbilitySystemComponent->MakeEffectContext();
	InteractableCarryEffectHandle = Carrier->AbilitySystemComponent->MakeOutgoingSpec(InteractableCarryEffect, 1.0f, EffectContext);

	if (InteractableCarryEffectHandle.IsValid())
	{
		InteractableCarryEffectSpec = InteractableCarryEffectHandle.Data.Get();
	}

	if (InteractableCarryEffectSpec)
	{
		ActiveCarryGameplayEffect = Carrier->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*InteractableCarryEffectSpec);
	}
}

void APEInteractableBase::RemoveCarryEffect()
{
	if (!IsValid(Carrier))
	{
		return;
	}
	
	Carrier->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveCarryGameplayEffect);
}

void APEInteractableBase::Interact()
{
	IInteractableInterface::Interact();

	AttachToComponent(Carrier->CarrySceneComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void APEInteractableBase::MulticastSetSimulatePhysics_Implementation(bool bInBool)
{
	StaticMeshComponent->SetSimulatePhysics(bInBool);
}