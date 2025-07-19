// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "PEBaseCharacterAttributeSet.h"

APEPlayerCharacter::APEPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UPEBaseCharacterAttributeSet>(TEXT("AttributeSet"));

	// RootComponent = RootSceneComponent;
	StaticMeshComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(RootComponent);
}

UAbilitySystemComponent* APEPlayerCharacter::GetAbilitySystemComponent() const
{
	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid AbilitySystemComponent"));
	}
	
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void APEPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		
	}
	
	float InSpeed = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.Speed"), TEXT("Finding"))->BaseValue;
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetSpeed(InSpeed); 
	UE_LOG(LogTemp, Warning, TEXT("My speed is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetSpeed())
}

// Called every frame
void APEPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}