// Fill out your copyright notice in the Description page of Project Settings.


#include "PECrushingPresencePassive.h"

UPECrushingPresencePassive::UPECrushingPresencePassive()
{
	SphereColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereColliderComponent"));
}