// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEEtherWarStructs.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "PEEquipmentCacheSpawner.generated.h"

UCLASS()
class PROJECTETHER_API APEEquipmentCacheSpawner : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	APEEquipmentCacheSpawner();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment Cache Spawner Spec")
	TEnumAsByte<ETeam> Team;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment Cache Spawner Spec")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment Cache Spawner Spec")
	UTextRenderComponent* TextRenderComponent;
};