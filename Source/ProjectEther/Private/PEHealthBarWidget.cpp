// Fill out your copyright notice in the Description page of Project Settings.

#include "PEHealthBarWidget.h"

#include "PEBaseCharacterAttributeSet.h"
#include "PEGameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void UPEHealthBarWidget::ServerUpdateHealthBar_Implementation(UPEBaseCharacterAttributeSet* AttributeSet)
{
	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(AttributeSet->GetOwningActor());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	
	float fPercentage = AttributeSet->GetHealth() / PlayerCharacter->fMaxHealthMaxValue;
	HealthBar->SetPercent(fPercentage);
}

void UPEHealthBarWidget::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(UPEHealthBarWidget, TeamTag);
}

// void UPEHealthBarWidget::OnRep_TeamTag()
// {
// 	APEPlayerController* PC = Cast<APEPlayerController>(GetOwningPlayer());
// 	
// 	for (const APlayerState* playerstate : GameState->PlayerArray)
// 	{
// 		if (playerstate && playerstate->GetOwner() == PC)
// 		{
// 			APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(PC->GetPawn());
// 			UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(PlayerCharacter->HealthBarWidgetComponent->GetWidget());
// 			HealthBarWidget->TeamTag->SetText(HealthBarWidget->TeamTagText);
// 		}
// 	}
// 	
// 	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(PC->GetPawn());
// 	UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(PlayerCharacter->HealthBarWidgetComponent->GetWidget());
// 	HealthBarWidget->TeamTag->SetText(HealthBarWidget->TeamTagText);
// }