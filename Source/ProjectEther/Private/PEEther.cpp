// Fill out your copyright notice in the Description page of Project Settings.


#include "PEEther.h"

// Sets default values
APEEther::APEEther()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	
	StaticMesh->SetupAttachment(RootComponent);
	SphereCollider->SetupAttachment(RootComponent);
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
		ActiveGameplayEffect = Carrier->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EtherCarryEffectSpec);
	}
}

void APEEther::RemoveCarryEffect() const
{
	if (!IsValid(Carrier))
	{
		return;
	}
	
	Carrier->AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGameplayEffect);
}