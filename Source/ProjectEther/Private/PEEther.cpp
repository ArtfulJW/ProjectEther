// Fill out your copyright notice in the Description page of Project Settings.


#include "PEEther.h"

// Sets default values
APEEther::APEEther()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	
	StaticMesh->SetupAttachment(RootComponent);
	SphereCollider->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APEEther::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APEEther::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

