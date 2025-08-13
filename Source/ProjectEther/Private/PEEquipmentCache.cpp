// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEquipmentCache.h"
#include "PEGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

APEEquipmentCache::APEEquipmentCache():
	bIsDeployed(false),
	NumRevives(3)
{
	bReplicates = true;
}

void APEEquipmentCache::BeginPlay()
{
	Super::BeginPlay();
	
	ServerSubscribeToGameState();
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

	if (NumRevives <= 0)
	{
		APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
		UE_LOG(LogTemp, Warning, TEXT("Equipment Cache: %s has no more Revive Tokens"), *this->GetName());
		GameState->ServerRemoveEquipmentCache(this, Team);
		Destroy();
	}
	
	// if (NumRevives > 0)
	// {
	// 	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	// 	APEPlayerCharacter* RequesterCharacter = Cast<APEPlayerCharacter>(Requester->GetCharacter());
	// 	TSubclassOf<APEPlayerCharacter> PlayerClass = RequesterCharacter->GetClass();
	// 	// GetWorld()->SpawnActor<APEPlayerCharacter>(GameState->PlayerControllerCharacterArray[Requester]);
	// 	FVector Vector = GetTransform().GetLocation();
	// 	Vector.X += 30;
	// 	// APEPlayerCharacter* SpawnedActor = GetWorld()->SpawnActor<APEPlayerCharacter>(PlayerClass, Vector,FRotator(0, 0, 0));
	// 	Requester->Possess(SpawnedActor);
	// 	UE_LOG(LogTemp, Warning, TEXT("Equipment Cache Spawning Player: %s. Revives left: %i"), *Requester->GetName(), NumRevives);
	// }

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEPlayerCharacter* SpawnedPlayerCharacter = nullptr;

	switch (Requester->CharacterClass)
	{
	case EClassType::Base:
		SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEPlayerCharacterClass, GetTransform().GetLocation(), GetTransform().Rotator());
		break;
	case EClassType::Berserker:
		SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEBerserkerPlayerCharacterClass, GetTransform().GetLocation(), GetTransform().Rotator());
		break;
	case EClassType::Mage:
		SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEMagePlayerCharacterClass, GetTransform().GetLocation(), GetTransform().Rotator());
		break;
	case EClassType::Priest:
		SpawnedPlayerCharacter = World->SpawnActor<APEPlayerCharacter>(Requester->PEPriestPlayerCharacterClass, GetTransform().GetLocation(), GetTransform().Rotator());
		break;
	}
	Requester->Possess(SpawnedPlayerCharacter);
	NumRevives--;
}

void APEEquipmentCache::ServerSubscribeToGameState_Implementation()
{
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->ServerAddEquipmentCache(this, Team);
}
