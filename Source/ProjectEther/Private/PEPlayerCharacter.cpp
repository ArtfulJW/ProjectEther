// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEPlayerController.h"
#include "PEBaseGameplayAbility.h"
#include "PEEquipmentCache.h"
#include "PEEther.h"
#include "PEGameState.h"
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
	bReplicates = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UPEBaseCharacterAttributeSet>(TEXT("AttributeSet"));
	CarrySceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CarrySceneComponent"));
	EtherCompassSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EtherCompassSceneComponent"));
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	
	StaticMeshComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(RootComponent);
	CarrySceneComponent->SetupAttachment(RootComponent);
	EtherCompassSceneComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetupAttachment(RootComponent);

	HealthBarWidgetComponent->SetIsReplicated(true);
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
	DOREPLIFETIME(APEPlayerCharacter, CarriedInteractableActor);
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

	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!IsValid(GameState))
	{
		return;
	}

	// GameState->PlayerControllerCharacterArray.Remove(PlayerController);
	PlayerCharacter->ClientRemovePlayerHUD();
	SpectatorPawn->SetActorLocation(GetActorLocation());
	PlayerController->Possess(SpectatorPawn);
	EtherCompassActor->Destroy();
}

void APEPlayerCharacter::IsLookingAtInteractable()
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, CameraComponent->GetComponentLocation(), CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 10000, ECC_Visibility);
	AActor* Actor = Hit.GetActor();

	// DrawDebugLine(GetWorld(), CameraComponent->GetComponentLocation(), CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * 10000, FColor::Black, false, 5.0f);
	
	if (!IsValid(PlayerHUD))
	{
		return;
	}

	if (!IsValid(Actor))
	{
		return;
	}
	
	UTextBlock* TextBlock =  Cast<UTextBlock>(PlayerHUD->GetWidgetFromName(FName("InteractTextBlock")));
	bIsLookingAtInteractableActor = UKismetSystemLibrary::DoesImplementInterface(Actor, UInteractableInterface::StaticClass());

	APEEquipmentCache* EquipmentCache = Cast<APEEquipmentCache>(Actor);
	APEPlayerController* PC = Cast<APEPlayerController>(GetController());
	if (EquipmentCache)
	{
		if (EquipmentCache->Team != PC->Team)
		{
			TextBlock->SetText(FText::FromString(""));
			return;
		}
		if (EquipmentCache->bIsDeployed)
		{
			TextBlock->SetText(FText::FromString("Cannot pick up deployed Equipment Cache"));
			return;
		}
	}

	APEEther* Ether = Cast<APEEther>(Actor);
	if (IsValid(Ether))
	{
		if (Ether->bIsDeposited)
		{
			TextBlock->SetText(FText::FromString(""));
			return;
		}	
	}
	
	if (Actor == CarriedInteractableActor)
	{
		TextBlock->SetText(FText::FromString("Press 'E' to drop"));
		return;
	}
	
	if (bIsLookingAtInteractableActor)
	{
		TextBlock->SetText(FText::FromString("Press 'E' to interact"));
		return;
	}
	
	TextBlock->SetText(FText::FromString(""));
}

void APEPlayerCharacter::MulticastUpdateHealthBar_Implementation()
{
	UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HealthBarWidgetComponent->GetWidget());
	if (!IsValid(HealthBarWidget))
	{
		return;
	}

	float fPercentage = AttributeSet->GetHealth() / 100;
	HealthBarWidget->HealthBar->SetPercent(fPercentage);
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

	if (IsValid(EtherCompassClass))
	{
		EtherCompassActor = Cast<APEEtherCompass>(GetWorld()->SpawnActor(EtherCompassClass));
		EtherCompassActor->AttachToComponent(EtherCompassSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		EtherCompassActor->SetOwner(this);
	}

	if (IsValid(HealthBarWidgetClass))
	{
		// UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HealthBarWidgetComponent->GetWidget());
		// HealthBarWidget->PlayerCharacterRef = this;
		
		HealthBarWidgetComponent->SetDrawAtDesiredSize(true);
		HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetClass);
		HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	}
	
	if (IsLocallyControlled())
	{
		APEPlayerController* PC = Cast<APEPlayerController>(GetController());
		ensureMsgf(PC, TEXT("Was not able to reference PlayerController"));
		PlayerHUD = CreateWidget<UPEPlayerHUD>(PC, HUDClass);
		ensureMsgf(PlayerHUD, TEXT("PlayerHUD failed to instantiate properly"));
		PlayerHUD->AddToViewport();
		PC->SubscribeToGameState(this->GetClass());
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
	IsLookingAtInteractable();
}