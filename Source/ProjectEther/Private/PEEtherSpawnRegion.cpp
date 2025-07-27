// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEtherSpawnRegion.h"
#include "PEGameState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APEEtherSpawnRegion::APEEtherSpawnRegion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SpawnRegion = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnRegion"));
	SpawnRegion->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APEEtherSpawnRegion::BeginPlay()
{
	Super::BeginPlay();
	
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid GameState"))
		return;
	}
	
	GameState->ServerSubscribeEtherSpawnRegion(this);
}

// Called every frame
void APEEtherSpawnRegion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}