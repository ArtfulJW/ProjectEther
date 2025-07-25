// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "InteractableInterface.h"
#include "PEPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "PEInteractableBase.generated.h"

/*
 * 
 */
UCLASS()
class PROJECTETHER_API APEInteractableBase : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:
	APEInteractableBase();
	
	virtual ~APEInteractableBase() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collider")
	USphereComponent* SphereCollider;

	UPROPERTY(Replicated)
	FTransform Transform;
	
	UPROPERTY()
	APEPlayerCharacter* Carrier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay Effect")
	TSubclassOf<UGameplayEffect> InteractableCarryEffect;
	
	FGameplayEffectSpec* InteractableCarryEffectSpec;

	FGameplayEffectSpecHandle InteractableCarryEffectHandle;
	
	FActiveGameplayEffectHandle ActiveCarryGameplayEffect;
	
	virtual void ApplyCarryEffect();

	virtual void RemoveCarryEffect();

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastSetSimulatePhysics(bool bInBool);
};