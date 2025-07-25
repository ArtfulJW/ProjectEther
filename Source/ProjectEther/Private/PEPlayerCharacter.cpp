// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEPlayerController.h"
#include "PEBaseGameplayAbility.h"
#include "PEEther.h"
#include "SNegativeActionButton.h"
#include "Components/TextBlock.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

APEPlayerCharacter::APEPlayerCharacter():
	bIsLookingAtInteractableActor(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UPEBaseCharacterAttributeSet>(TEXT("AttributeSet"));
	CarrySceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CarrySceneComponent"));
	
	StaticMeshComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(RootComponent);
	CarrySceneComponent->SetupAttachment(RootComponent);
}

UAbilitySystemComponent* APEPlayerCharacter::GetAbilitySystemComponent() const
{
	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid AbilitySystemComponent"));
	}
	
	return AbilitySystemComponent;
}

void APEPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APEPlayerCharacter, AbilityOneHandle, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APEPlayerCharacter, AbilityTwoHandle, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APEPlayerCharacter, AbilityThreeHandle, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APEPlayerCharacter, WeaponAbilityOneHandle, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APEPlayerCharacter, WeaponAbilityTwoHandle, COND_OwnerOnly);
}

void APEPlayerCharacter::ClientRemovePlayerHUD_Implementation()
{
	PlayerHUD->RemoveFromParent();
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

	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(PlayerController->GetPawn());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	
	PlayerCharacter->ClientRemovePlayerHUD();
	SpectatorPawn->SetActorLocation(GetActorLocation());
	PlayerController->Possess(SpectatorPawn);
}

void APEPlayerCharacter::IsLookingAtEther()
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, CameraComponent->GetComponentLocation(), CameraComponent->GetForwardVector() * 10000, ECC_Visibility);
	AActor* Actor = Hit.GetActor();

	if (!IsValid(PlayerHUD))
	{
		return;
	}

	if (!IsValid(Actor))
	{
		return;
	}
	
	UTextBlock* TextBlock =  Cast<UTextBlock>(PlayerHUD->GetWidgetFromName(FName("InteractTextBlock")));
	// bIsLookingAtEther = Actor->IsA(APEEther::StaticClass());
	bIsLookingAtInteractableActor = UKismetSystemLibrary::DoesImplementInterface(Actor, UInteractableInterface::StaticClass());
	if (bIsLookingAtInteractableActor)
	{
		TextBlock->SetText(FText::FromString("Press 'E' to interact"));
	}
	else
	{
		TextBlock->SetText(FText::FromString(""));
	}
}

// Called when the game starts or when spawned
void APEPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid AbilitySystemComponent"))
	}
	
	float InSpeed = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.Speed"), TEXT("Could not find PEBaseAttributeSet.Speed"))->BaseValue;
	float InHealth = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.Health"), TEXT("Could not find PEBaseAttributeSet.Health"))->BaseValue;
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
	IsLookingAtEther();
}