// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEtherSpawner.h"

#include "PEGameState.h"
#include "PEMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APEEtherSpawner::APEEtherSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APEEtherSpawner::BeginPlay()
{
	Super::BeginPlay();

	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!IsValid(GameState))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid GameState"))
		return;
	}

	FVector PointRelativeToSpawnRegion = UKismetMathLibrary::InverseTransformLocation(GameState->EtherSpawnRegion->GetTransform(), GetActorLocation());
	float fPointDistanceFromBoxEdge = PEMathLibrary::SignedDistanceFieldBox(PointRelativeToSpawnRegion, GameState->EtherSpawnRegion->SpawnRegion->GetUnscaledBoxExtent());

	// DrawDebugSphere(GetWorld(), PointRelativeToSpawnRegion, 3, 15, FColor::Red, true);
	// DrawDebugSphere(GetWorld(), GameState->EtherSpawnRegion->GetActorScale(), 3, 15, FColor::Purple, true);
	// DrawDebugSphere(GetWorld(), GetActorLocation(), 7, 15,FColor::Yellow, true);
	// DrawDebugSphere(GetWorld(), GameState->EtherSpawnRegion->GetTransform().GetLocation(), 7, 15,FColor::Cyan, true);
	// DrawDebugLine(GetWorld(),GameState->EtherSpawnRegion->GetTransform().GetLocation(), GameState->EtherSpawnRegion->GetTransform().GetLocation() + PointRelativeToSpawnRegion, FColor::Black, true, -1, 0, 1);
	
	if (fPointDistanceFromBoxEdge <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("EtherSpawner within the bounds of EtherSpawnRegion - subscribing %s GameState"), *this->GetName())
		DrawDebugSphere(GetWorld(), GetActorLocation(), 15, 15, FColor::Emerald, true);
		GameState->ServerSubscribeEtherSpawner(this);
	}
}

// Called every frame
void APEEtherSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}