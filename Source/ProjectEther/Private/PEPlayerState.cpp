// Fill out your copyright notice in the Description page of Project Settings.


#include "PEPlayerState.h"

#include "PEPlayerController.h"
#include "Net/UnrealNetwork.h"

void APEPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEPlayerState, TeamTagText);
}

void APEPlayerState::ServerSetTagname_Implementation(const FText& newText)
{
	TeamTagText = newText;
	OnRep_TeamTag();
}

void APEPlayerState::OnRep_TeamTag() const
{
	UE_LOG(LogTemp, Display, TEXT("OnRep_TeamTag: %s"), *TeamTagText.ToString());
}
