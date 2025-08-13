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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Ether Compass Specs")
	bool bIsTakenOut;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StaticMeshComponent")
	UStaticMeshComponent* StaticMeshCompassBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StaticMeshComponent")
	UStaticMeshComponent* StaticMeshPointer;

	void UpdateEtherPointer() const;
	
	void UpdateCollisionAndVisibility();
	
	void SetIsCheckingCompass(bool bInBool);
};