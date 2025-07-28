// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEquipmentCacheSpawner.h"
#include "PEGameState.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APEEquipmentCacheSpawner::APEEquipmentCacheSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	TextRenderComponent->SetupAttachment(BoxComponent);
	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APEEquipmentCacheSpawner::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(World));
	if (!IsValid(GameState))
	{
		return;
	}

	GameState->SubscribeEquipmentCacheSpawner(this, Team);
	TextRenderComponent->Text = FText::FromString(FText::FromString(*ETeam_ToString(Team)).ToString() + TEXT("Equipment Cache Spawner"));
}

// Called every frame
void APEEquipmentCacheSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}