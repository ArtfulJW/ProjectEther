// Fill out your copyright notice in the Description page of Project Settings.

#include "PEGameMode.h"

APEGameMode::APEGameMode():
NumEtherToWin(3)
{
}

int APEGameMode::GetNumEtherToWin() const
{
	return NumEtherToWin;
}