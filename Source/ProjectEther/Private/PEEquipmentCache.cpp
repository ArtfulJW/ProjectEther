// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEquipmentCache.h"
#include "PEGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

APEEquipmentCache::APEEquipmentCache():
	bIsDeployed(false),
	NumRevives(3)
{
}

void APEEquipmentCache::BeginPlay()
{
	Super::BeginPlay();

	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	// GameState->AssignTeamToEquipmentCache(this);
	GameState->ServerAddEquipmentCache(this, Team);
}

void APEEquipmentCache::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEEquipmentCache, bIsDeployed);
	DOREPLIFETIME(APEEquipmentCache, NumRevives);
	DOREPLIFETIME(APEEquipmentCache, Team);
}

void APEEquipmentCache::Deploy()
{
	Super::Deploy();

	bIsDeployed = true;
	MulticastSetSimulatePhysics(false);
	UE_LOG(LogTemp, Warning, TEXT("Deploying Equipment Cache"));
}

void APEEquipmentCache::SpawnPlayer(APEPlayerController* Requester)
{
	if (!bIsDeployed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Equipment Cache: %s is not Deployed"), *this->GetName());
		return;
	}

	if (Requester->Team != Team)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot Spawn Player: Equipment Cache and Requested Player not on the same team"));
		return;
	}
	
	if (NumRevives > 0)
	{
		NumRevives--;
		APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
		// GetWorld()->SpawnActor<APEPlayerCharacter>(GameState->PlayerControllerCharacterArray[Requester]);
		FVector Vector = GetTransform().GetLocation();
		Vector.X += 30;
		APEPlayerCharacter* SpawnedActor = GetWorld()->SpawnActor<APEPlayerCharacter>(PlayerCharacter, Vector,FRotator(0, 0, 0));
		Requester->Possess(SpawnedActor);
		UE_LOG(LogTemp, Warning, TEXT("Equipment Cache Spawning Player: %s. Revives left: %i"), *Requester->GetName(), NumRevives);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Equipment Cache: %s has no more Revive Tokens"), *this->GetName());
		Destroy();
	}
}