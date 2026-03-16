// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PEPlayerCharacter.h"
#include "PEBerserkerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API APEBerserkerCharacter : public APEPlayerCharacter
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	/*
	 * Berserker specific behaviour when taking damage.
	 */
	virtual void OnTakeDamage() override;

	/*
	 * On taking any incoming damage, adds a flat amount of Willpower.
	 */
	UFUNCTION(Server, Reliable)
	void RegenerateWillpower(int amount);
};
