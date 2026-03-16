// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPriestCharacter.h"

#include "PEEquipmentCache.h"
#include "PEEtherDeposit.h"

void APEPriestCharacter::ReplenishScrolls_Implementation()
{
	UCameraComponent* PlayerCamera = this->CameraComponent;
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * fInteractDistance, ECC_Pawn);

	AActor* HitActor = Hit.GetActor();
	APEEquipmentCache* EquipmentCache = Cast<APEEquipmentCache>(HitActor);
	APEEtherDeposit* EtherDeposit = Cast<APEEtherDeposit>(HitActor);
	
	if ((IsValid(EquipmentCache) && EquipmentCache->bIsDeployed) || IsValid(EtherDeposit))
	{
		AttributeSet->SetResource(AttributeSet->GetResourceMaxValue());
		ClientReplenishResource(this);
	}
}

void APEPriestCharacter::ClientReplenishResource_Implementation(APEPlayerCharacter* PlayerCharacter)
{
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	PlayerCharacter->AttributeSet->SetResource(PlayerCharacter->AttributeSet->GetResourceMaxValue());
}
