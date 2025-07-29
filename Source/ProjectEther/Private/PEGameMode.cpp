// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameMode.h"

APEGameMode::APEGameMode():
NumEtherToWin(3)
{
	bUseSeamlessTravel = true;
}

int APEGameMode::GetNumEtherToWin() const
{
	return NumEtherToWin;
}