// Fill out your copyright notice in the Description page of Project Settings.


#include "PEEther.h"

#include "Net/UnrealNetwork.h"

// Sets default values
APEEther::APEEther():
Transform(GetTransform())
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	
	RootComponent = StaticMesh;
	SphereCollider->SetupAttachment(StaticMesh);

	StaticMesh->SetSimulatePhysics(true);
}

void APEEther::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEEther, Transform);
}

// Called when the game starts or when spawned
void APEEther::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APEEther::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetNetMode() < NM_Client)
	{
		MulticastTransform();
	}
	else
	{
		SetActorTransform(Transform);
	}
	
}

void APEEther::ApplyCarryEffect()
{
	if (!IsValid(Carrier))
	{
		return;
	}
	
	FGameplayEffectContextHandle EffectContext = Carrier->AbilitySystemComponent->MakeEffectContext();
	EtherCarryEffectHandle = Carrier->AbilitySystemComponent->MakeOutgoingSpec(EtherCarryEffect, 1.0f, EffectContext);

	if (EtherCarryEffectHandle.IsValid())
	{
		EtherCarryEffectSpec = EtherCarryEffectHandle.Data.Get();
	}

	if (EtherCarryEffectSpec)
	{
		ActiveCarryGameplayEffect = Carrier->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EtherCarryEffectSpec);
	}
}

void APEEther::RemoveCarryEffect() const
{
	if (!IsValid(Carrier))
	{
		return;
	}
	
	Carrier->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveCarryGameplayEffect);
}

void APEEther::Interact()
{
	IInteractableInterface::Interact();

	AttachToComponent(Carrier->CarrySceneComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void APEEther::MulticastTransform_Implementation()
{
	Transform = GetTransform();
}

void APEEther::MulticastSetSimulatePhysics_Implementation(bool bInBool)
{
	StaticMesh->SetSimulatePhysics(bInBool);
}
