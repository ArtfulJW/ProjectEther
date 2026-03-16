// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEPlayerCharacter.h"
#include "PEMageCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEMageCharacter : public APEPlayerCharacter
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
};
