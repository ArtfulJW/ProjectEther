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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:	
	// Sets default values for this actor's properties
	APEEther();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact() override;
	
	FTimerHandle EtherPulseTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Specs")
	float fPulseRate;

	UPROPERTY(Replicated, VisibleAnywhere, Category="Ether Specs")
	bool bIsDeposited;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEtherPulse();

	 UFUNCTION(Server, Reliable)
	void ServerSetIsDeposited(bool InBool);
};