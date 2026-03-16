// Fill out your copyright notice in the Description page of Project Settings.


#include "PEMageCharacter.h"

void APEMageCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (GetNetMode() < NM_Client)
	{
		AttributeSet->AddResource(AttributeSet->GetResourceReplenishRate());
	}
}