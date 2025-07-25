// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEquipmentCache.h"

#include "Net/UnrealNetwork.h"

APEEquipmentCache::APEEquipmentCache():
bIsDeployed(false),
NumRevives(3)
{
}

void APEEquipmentCache::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEEquipmentCache, bIsDeployed);
	DOREPLIFETIME(APEEquipmentCache, NumRevives);
}

void APEEquipmentCache::Deploy()
{
	Super::Deploy();

	bIsDeployed = true;
	MulticastSetSimulatePhysics(false);
	UE_LOG(LogTemp, Warning, TEXT("Deploying Equipment Cache"));
}
