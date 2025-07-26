// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEtherCompass.h"

#include "PEGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APEEtherCompass::APEEtherCompass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	StaticMeshCompassBase = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshCompassBase");
	StaticMeshPointer = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshPointer");

	StaticMeshCompassBase->SetupAttachment(RootComponent);
	StaticMeshPointer->SetupAttachment(StaticMeshCompassBase);
		
	StaticMeshCompassBase->SetIsReplicated(true);
	StaticMeshPointer->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void APEEtherCompass::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APEEtherCompass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateEtherPointer();
}

void APEEtherCompass::UpdateEtherPointer() const
{
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	FVector EtherLocation = GameState->Ether->GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();

	EtherLocation = UKismetMathLibrary::InverseTransformLocation(GetTransform(), EtherLocation);

	EtherLocation.Z = 0.0f;
	ForwardVector.Z = 0.0f;
	
	FVector LookAtLocation = EtherLocation - ForwardVector;
	LookAtLocation.Z = 0.0f;
	
	// DrawDebugSphere(GetWorld(), GameState->Ether->GetActorLocation(),30, 15, FColor::Red);
	// DrawDebugSphere(GetWorld(), EtherLocation,30, 15, FColor::Red);
	// DrawDebugSphere(GetWorld(), ForwardVector,20, 15, FColor::Blue);
	// DrawDebugSphere(GetWorld(), LookAtLocation,10, 15, FColor::Yellow);
	// DrawDebugLine(GetWorld(),FVector(0,0,0), EtherLocation, FColor::Red);
	// DrawDebugLine(GetWorld(), FVector(0,0,0), ForwardVector, FColor::Blue);
	// DrawDebugLine(GetWorld(), FVector(0,0,0), LookAtLocation, FColor::Yellow);

	StaticMeshPointer->SetRelativeRotation(LookAtLocation.Rotation());
}
