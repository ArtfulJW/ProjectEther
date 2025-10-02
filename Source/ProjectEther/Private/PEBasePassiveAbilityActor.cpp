// Fill out your copyright notice in the Description page of Project Settings.


#include "PEBasePassiveAbilityActor.h"

// Sets default values for this component's properties
APEBasePassiveAbilityActor::APEBasePassiveAbilityActor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	
	PrimaryActorTick.bCanEverTick = true;
}

void APEBasePassiveAbilityActor::SetupPassiveAbility()
{
}

// Called when the game starts
void APEBasePassiveAbilityActor::BeginPlay()
{
	Super::BeginPlay();
}

void APEBasePassiveAbilityActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}