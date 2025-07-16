// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerCharacter.h"

#include "PEPlayerController.h"
#include "Kismet/GameplayStatics.h"

class APEPlayerController;
// Sets default values
APEPlayerCharacter::APEPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
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

void APEPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Super::SetupPlayerInputComponent(PlayerInputComponent);
	//
	// UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// if (!IsValid(Input))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Invalid UEnhancedInputComponent"));
	// }
	//
	// UWorld* World = GetWorld();
	// if (!IsValid(World))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
	// }
	//
	// APEPlayerController* PC = Cast<APEPlayerController>(UGameplayStatics::GetPlayerController(World, 0));
	// if (!IsValid(PC))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Invalid Player Character"));
	// }
}