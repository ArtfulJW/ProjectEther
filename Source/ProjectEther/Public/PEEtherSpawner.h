// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PEEtherSpawner.generated.h"

UCLASS()
class PROJECTETHER_API APEEtherSpawner : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	APEEtherSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Box Component")
	UBoxComponent* BoxComponent;
	
};