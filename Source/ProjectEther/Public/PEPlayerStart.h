// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEEtherWarStructs.h"
#include "GameFramework/PlayerStart.h"
#include "PEPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player Start Spec")
	TEnumAsByte<ETeam> Team;
};