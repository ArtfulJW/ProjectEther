// Fill out your copyright notice in the Description page of Project Settings.

#include "PEInteractableBase.h"
#include "Net/UnrealNetwork.h"

APEInteractableBase::APEInteractableBase():
Transform(GetTransform())
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	
	RootComponent = StaticMesh;
	SphereCollider->SetupAttachment(StaticMesh);

	StaticMesh->SetSimulatePhysics(true);
}

APEInteractableBase::~APEInteractableBase()
{
}

void APEInteractableBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEInteractableBase, Transform);
}

// Called every frame
void APEInteractableBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() < NM_Client)
	{
		Transform = GetTransform();
	}
	else
	{
		SetActorTransform(Transform);
	}
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

void APEInteractableBase::MulticastSetSimulatePhysics_Implementation(bool bInBool)
{
	StaticMesh->SetSimulatePhysics(bInBool);
}
