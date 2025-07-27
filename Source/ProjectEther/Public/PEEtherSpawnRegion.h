// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEEtherSpawnRegion.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTETHER_API APEEtherSpawnRegion : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	APEEtherSpawnRegion();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Box Component")
	UBoxComponent* SpawnRegion;
};