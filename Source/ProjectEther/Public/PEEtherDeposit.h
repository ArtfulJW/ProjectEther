// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEEtherWarStructs.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "PEEtherDeposit.generated.h"

UCLASS()
class PROJECTETHER_API APEEtherDeposit : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	APEEtherDeposit();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroyed() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Deposit Specs")
	TEnumAsByte<ETeam> Team; 
	
	UPROPERTY(Replicated, VisibleAnywhere)
	int NumEther;
	
	UPROPERTY(VisibleAnywhere, Category="Ether Deposit Specs")
	TArray<USceneComponent*> EtherDepositSceneComponents;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Deposit Specs")
	USceneComponent* EtherDepositOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Deposit Specs")
	USceneComponent* EtherDepositTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Deposit Specs")
	USceneComponent* EtherDepositThree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Deposit Specs")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Deposit Specs")
	UStaticMeshComponent* StaticMeshComponent;

	UFUNCTION(BlueprintCallable, Category = "Ether Deposit Functions")
	void DepositEther(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void ServerCheckEndGame();
};