// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PEPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	UPROPERTY(ReplicatedUsing=OnRep_TeamTag, EditAnywhere, BlueprintReadWrite, Category = "Health Bar Specs")
	FText TeamTagText;
	
	UFUNCTION()
	void OnRep_TeamTag() const;

	UFUNCTION(Server, Reliable)
	void ServerSetTagname(const FText& newText);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
