// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PEEtherCompass.generated.h"

UCLASS()
class PROJECTETHER_API APEEtherCompass : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	APEEtherCompass();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StaticMeshComponent")
	UStaticMeshComponent* StaticMeshCompassBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StaticMeshComponent")
	UStaticMeshComponent* StaticMeshPointer;

	void UpdateEtherPointer() const;
};