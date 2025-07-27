// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEtherCompass.h"

#include "PEGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APEEtherCompass::APEEtherCompass():
bIsTakenOut(false)
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

void APEEtherCompass::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEEtherCompass, bIsTakenOut);
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
	
	UpdateCollisionAndVisibility();
}

void APEEtherCompass::UpdateEtherPointer() const
{
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	const APEEther* Ether = GameState->Ether;
	if (!IsValid(Ether))
	{
		return;
	}
	
	FVector EtherLocation = Ether->GetActorLocation();
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

void APEEtherCompass::UpdateCollisionAndVisibility()
{
	StaticMeshCompassBase->SetVisibility(bIsTakenOut);
	StaticMeshPointer->SetVisibility(bIsTakenOut);

	if (!bIsTakenOut)
	{
		StaticMeshCompassBase->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMeshPointer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	} 
	else
	{
		StaticMeshCompassBase->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StaticMeshPointer->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void APEEtherCompass::SetIsCheckingCompass(bool bInBool)
{
	bIsTakenOut = bInBool;
}