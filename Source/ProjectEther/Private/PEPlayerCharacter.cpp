// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerCharacter.h"

#include "PEPlayerController.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

class APEPlayerController;
// Sets default values
APEPlayerCharacter::APEPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// RootComponent = RootSceneComponent;
	StaticMeshComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(RootComponent);
}		

// Called when the game starts or when spawned
void APEPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APEPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}