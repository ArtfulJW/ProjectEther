// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "PEInteractableBase.h"
#include "InteractableInterface.h"
#include "PEPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PEEther.generated.h"

UCLASS()
class PROJECTETHER_API APEEther : public APEInteractableBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APEEther();

	FTimerHandle EtherPulseTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Specs")
	float fPulseRate;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEtherPulse();
};