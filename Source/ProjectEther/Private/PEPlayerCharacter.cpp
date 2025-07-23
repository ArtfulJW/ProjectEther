// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEPlayerController.h"
#include "PEBaseGameplayAbility.h"
#include "SNegativeActionButton.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
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

EDamageDirection APEPlayerCharacter::DetermineDamageDirection(const FHitResult& HitResult) const
{
	const AActor* HitActor = HitResult.GetActor();
	FVector InVector = HitResult.Location;
	FVector ForwardVector = HitActor->GetActorLocation() + HitActor->GetActorForwardVector() * 100;
	InVector.Z = GetActorLocation().Z;
	
	InVector = UKismetMathLibrary::InverseTransformLocation(HitActor->GetTransform(), InVector);
	ForwardVector = UKismetMathLibrary::InverseTransformLocation(HitActor->GetTransform(), ForwardVector);
	
	float DotProduct = FVector::DotProduct(InVector, ForwardVector);
	float Angle = FMath::Acos(DotProduct / (InVector.Size() * ForwardVector.Size()));
	Angle = FMath::RadiansToDegrees(Angle);

	// DrawDebugSphere(GetWorld(), FVector(0,0,0), 5, 16, FColor::Yellow);
	// DrawDebugSphere(GetWorld(), InVector, 15,  16, FColor::Red);
	// DrawDebugSphere(GetWorld(), ForwardVector, 15,  16, FColor::Blue);
	// DrawDebugLine(GetWorld(), GetActorLocation(), InVector, FColor::Red);
	// DrawDebugLine(GetWorld(), GetActorLocation(), ForwardVector, FColor::Blue);

	EDamageDirection DamageDirection = {};
	if (Angle <= 45.0f)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Damage from front: %f"), Angle);
		DamageDirection = EDamageDirection::Front;
	}
	else if (Angle >= 45.0f && Angle <= 135.0f)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Damage from sides: %f"), Angle);
		DamageDirection = EDamageDirection::Side;
	}
	else if (Angle >= 135.0f && Angle <= 180.0f)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Damage from back: %f"), Angle);
		DamageDirection = EDamageDirection::Back;
	}

	return DamageDirection;
}

void APEPlayerCharacter::BeforeDestroy()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEPlayerController* PlayerController = Cast<APEPlayerController>(GetController());
	if (!IsValid(PlayerController))
	{
		return;
	}
	
	ASpectatorPawn* SpectatorPawn = World->SpawnActor<ASpectatorPawn>(PlayerController->PESpectatorPawn);
	if (!IsValid(SpectatorPawn))
	{
		return;
	}
	
	SpectatorPawn->SetActorLocation(GetActorLocation());
	PlayerController->Possess(SpectatorPawn);
}

// Called when the game starts or when spawned
void APEPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid AbilitySystemComponent"))
	}
	
	float InSpeed = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.Speed"), TEXT("Finding"))->BaseValue;
	float InHealth = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.Health"), TEXT("Finding"))->BaseValue;
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetSpeed(InSpeed);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetHealth(InHealth);
	
	UE_LOG(LogTemp, Warning, TEXT("My Speed is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetSpeed())
	UE_LOG(LogTemp, Warning, TEXT("My Health is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetHealth())

	if (!IsValid(HUDClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid HUDClass"))
	}
	
	if (IsLocallyControlled())
	{
		APEPlayerController* PC = Cast<APEPlayerController>(GetController());
		ensureMsgf(PC, TEXT("Was not able to reference PlayerController"));
		PlayerHUD = CreateWidget<UPEPlayerHUD>(PC, HUDClass);
		ensureMsgf(PlayerHUD, TEXT("PlayerHUD failed to instantiate properly"));
		PlayerHUD->AddToViewport();
	}
	
	if (GetNetMode() < NM_Client)
	{
		PassiveAbilityHandle = AbilitySystemComponent->GiveAbility(PassiveAbility);
		WeaponAbilityOneHandle = AbilitySystemComponent->GiveAbility(WeaponAbilityOne);
		WeaponAbilityTwoHandle = AbilitySystemComponent->GiveAbility(WeaponAbilityTwo);
		AbilityOneHandle = AbilitySystemComponent->GiveAbility(AbilityOne);
		AbilityTwoHandle = AbilitySystemComponent->GiveAbility(AbilityTwo);
		AbilityThreeHandle = AbilitySystemComponent->GiveAbility(AbilityThree);
	}
}

// Called every frame
void APEPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// EDamageDirection Direction = DetermineDamageDirection(FVector(200,0,0));
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *EDamageDirection_ToString(Direction))
}