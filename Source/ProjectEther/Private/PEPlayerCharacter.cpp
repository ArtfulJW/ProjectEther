// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PETestGameplayAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

APEPlayerCharacter::APEPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UPEBaseCharacterAttributeSet>(TEXT("AttributeSet"));
	
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

void APEPlayerCharacter::CalculateDamageDirection(FVector InVector)
{
	InVector = InVector + GetActorLocation();
	FVector ForwardVector = GetActorLocation() + GetActorForwardVector() * 100;

	InVector = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), InVector);
	ForwardVector = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), ForwardVector);
	
	float DotProduct = FVector::DotProduct(InVector, ForwardVector);
	float Angle = FMath::Acos(DotProduct / (InVector.Size() * ForwardVector.Size()));
	Angle = FMath::RadiansToDegrees(Angle);

	// DrawDebugSphere(GetWorld(), FVector(0,0,0), 5, 16, FColor::Yellow);
	// DrawDebugSphere(GetWorld(), InVector, 15,  16, FColor::Red);
	// DrawDebugSphere(GetWorld(), ForwardVector, 15,  16, FColor::Blue);
	// DrawDebugLine(GetWorld(), GetActorLocation(), InVector, FColor::Red);
	// DrawDebugLine(GetWorld(), GetActorLocation(), ForwardVector, FColor::Blue);
	
	if (Angle <= 45.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage from front: %f"), Angle);
	}
	else if (Angle >= 45.0f && Angle <= 135.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage from sides: %f"), Angle);
	}
	else if (Angle >= 135.0f && Angle <= 180.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage from back: %f"), Angle);
	}
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

	if (GetNetMode() < NM_Client)
	{
		PassiveAbilityHandle = AbilitySystemComponent->GiveAbility(PassiveAbility);
		WeaponAbilityHandle = AbilitySystemComponent->GiveAbility(WeaponAbility);
		AbilityOneHandle = AbilitySystemComponent->GiveAbility(AbilityOne);
		AbilityTwoHandle = AbilitySystemComponent->GiveAbility(AbilityTwo);
		AbilityThreeHandle = AbilitySystemComponent->GiveAbility(AbilityThree);
	}
}

// Called every frame
void APEPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateDamageDirection(FVector(30,0,0));
}