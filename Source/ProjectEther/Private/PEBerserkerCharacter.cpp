// Fill out your copyright notice in the Description page of Project Settings.

#include "PEBerserkerCharacter.h"

void APEBerserkerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Bind Berserker specific OnTakeDamage behaviour 
	OnTakeDamageDelegate.BindUObject(this, &APEBerserkerCharacter::OnTakeDamage);
}

void APEBerserkerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetNetMode() < NM_Client)
	{
		AttributeSet->AddResource(AttributeSet->GetResourceReplenishRate());
	}
}

void APEBerserkerCharacter::OnTakeDamage()
{
	RegenerateWillpower(AttributeSet->GetOnTakeDamageResourceReplenishRate());
}

void APEBerserkerCharacter::RegenerateWillpower_Implementation(int amount)
{
	if (GetNetMode() < NM_Client)
	{
		AttributeSet->AddResource(amount);	
	}
}