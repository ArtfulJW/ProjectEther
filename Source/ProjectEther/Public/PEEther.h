// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Collider")
	USphereComponent* SphereCollider;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
