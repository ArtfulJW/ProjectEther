// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "InteractableInterface.h"
#include "PEPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PEEther.generated.h"

UCLASS()
class PROJECTETHER_API APEEther : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APEEther();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collider")
	USphereComponent* SphereCollider;

	UPROPERTY(Replicated)
	FTransform Transform;
	
	UPROPERTY()
	APEPlayerCharacter* Carrier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay Effect")
	TSubclassOf<UGameplayEffect> EtherCarryEffect;
	
	FGameplayEffectSpec* EtherCarryEffectSpec;

	FGameplayEffectSpecHandle EtherCarryEffectHandle;
	
	FActiveGameplayEffectHandle ActiveCarryGameplayEffect;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyCarryEffect();
	
	void RemoveCarryEffect() const;

	virtual void Interact() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastSetSimulatePhysics(bool bInBool);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastTransform();
};
