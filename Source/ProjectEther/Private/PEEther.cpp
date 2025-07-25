// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEther.h"

// Sets default values
APEEther::APEEther():
fPulseRate(5.0f)
{
	
}

// Called when the game starts or when spawned
void APEEther::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(EtherPulseTimerHandle, this, &APEEther::MulticastEtherPulse_Implementation, fPulseRate, true);
}

// Called every frame
void APEEther::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APEEther::Interact()
{
	IInteractableInterface::Interact();

	AttachToComponent(Carrier->CarrySceneComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void APEEther::MulticastEtherPulse_Implementation()
{
	if (GetNetMode() < NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("Multicast; Server Ether Pulse"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Multicast; Client Ether Pulse"));
	}
}