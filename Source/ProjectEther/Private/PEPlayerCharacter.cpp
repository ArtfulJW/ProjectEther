// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEPlayerController.h"
#include "PEBaseGameplayAbility.h"
#include "PEEquipmentCache.h"
#include "PEEther.h"
#include "PEGameState.h"
#include "PEPlayerState.h"
#include "Components/TextBlock.h"
#include "Engine/SkeletalMeshSocket.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

APEPlayerCharacter::APEPlayerCharacter():
	fInteractDistance(500.0f),
	bIsLookingAtInteractableActor(false),
	bTeamTagTextUpdated(false),
	bIsRunning(false),
	DamageDirectionRotation(FRotator(0,0,0)),
	fMaxHealthMaxValue(0.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	// PassiveAbilityComponent = CreateDefaultSubobject<UPEBasePassiveAbilityComponent>(TEXT("PassiveAbilityComponent"));
	// PassiveAbilityComponent = NewObject<UPEBasePassiveAbilityComponent>(this, BP_PassiveAbilityComponent->StaticClass());
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UPEBaseCharacterAttributeSet>(TEXT("AttributeSet"));
	CarrySceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CarrySceneComponent"));
	EtherCompassSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("EtherCompassSceneComponent"));
	HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	DamageDirectionComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageDirectionComponent"));
	FPSArms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FPSArms"));
	
	StaticMeshComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(RootComponent);
	CarrySceneComponent->SetupAttachment(RootComponent);
	EtherCompassSceneComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	DamageDirectionComponent->SetupAttachment(RootComponent);
	// PassiveAbilityComponent->SetupAttachment(RootComponent);
	FPSArms->SetupAttachment(CameraComponent);

	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	HealthBarWidgetComponent->SetIsReplicated(true);
	DamageDirectionComponent->SetIsReplicated(true);

	FPSArms->SetVisibility(false);
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
	DOREPLIFETIME(APEPlayerCharacter, EtherCompassActor);
	DOREPLIFETIME(APEPlayerCharacter, PassiveAbilityActor);
	DOREPLIFETIME(APEPlayerCharacter, CopiedTeamTagText);
	DOREPLIFETIME(APEPlayerCharacter, bIsRunning);
	DOREPLIFETIME(APEPlayerCharacter, DamageDirectionRotation);
	DOREPLIFETIME(APEPlayerCharacter, WeaponActor);
	DOREPLIFETIME(APEPlayerCharacter, AttributeSet);
}

void APEPlayerCharacter::ClientRemovePlayerHUD_Implementation()
{
	PlayerHUD->RemoveFromParent();
}

EDamageDirection APEPlayerCharacter::DetermineDamageDirection(const FHitResult& HitResult, FLinearColor InColor) const
{
	AActor* HitActor = HitResult.GetActor();
	APEPlayerCharacter* HitPlayerCharacter = Cast<APEPlayerCharacter>(HitActor);
	APEPlayerController* HitPlayerController = Cast<APEPlayerController>(HitPlayerCharacter->GetController());
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
		HitPlayerCharacter->ServerActivateHitIndicator(EDirectionDamageIndicator::Front, InColor);
	}
	else if (Angle >= 45.0f && Angle <= 135.0f)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Damage from sides: %f"), Angle);
		DamageDirection = EDamageDirection::Side;
		if (InVector.Y < 0.0f)
		{
			// Left Side
			HitPlayerCharacter->ServerActivateHitIndicator(EDirectionDamageIndicator::Left, InColor);
		}
		else
		{
			// Right Side
			HitPlayerCharacter->ServerActivateHitIndicator(EDirectionDamageIndicator::Right, InColor);
		}
	}
	else if (Angle >= 135.0f && Angle <= 180.0f)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Damage from back: %f"), Angle);
		DamageDirection = EDamageDirection::Back;
		HitPlayerCharacter->ServerActivateHitIndicator(EDirectionDamageIndicator::Back, InColor);
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
	PlayerController->ServerDropInteractableActor(PlayerController);
	PlayerController->Possess(SpectatorPawn);
	ServerCleanupPlayerCharacter();
}

void APEPlayerCharacter::IsLookingAtInteractable()
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, CameraComponent->GetComponentLocation(), CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * fInteractDistance, ECC_Visibility);
	AActor* Actor = Hit.GetActor();

	// DrawDebugLine(GetWorld(), CameraComponent->GetComponentLocation(), CameraComponent->GetComponentLocation() + CameraComponent->GetForwardVector() * fInteractDistance, FColor::Black, false, 5.0f);
	
	if (!IsValid(PlayerHUD))
	{
		return;
	}
	
	UTextBlock* TextBlock =  Cast<UTextBlock>(PlayerHUD->GetWidgetFromName(FName("InteractTextBlock")));
	bIsLookingAtInteractableActor = UKismetSystemLibrary::DoesImplementInterface(Actor, UInteractableInterface::StaticClass());

	if (!IsValid(Actor))
	{
		TextBlock->SetText(FText::FromString(""));
		return;
	}

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

void APEPlayerCharacter::UpdateHealthBar()
{
	UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HealthBarWidgetComponent->GetWidget());
	if (!IsValid(HealthBarWidget))
	{
		return;
	}

	float fPercentage = AttributeSet->GetHealth() / 100;
	HealthBarWidget->HealthBar->SetPercent(fPercentage);
}

void APEPlayerCharacter::ServerCleanupPlayerCharacter_Implementation()
{
	if (GetNetMode() < NM_Client)
	{
		if (IsValid(EtherCompassActor))
		{
			EtherCompassActor->Destroy();
		}

		if (IsValid(PassiveAbilityActor))
		{
			PassiveAbilityActor->Destroy();
		}

		if (IsValid(WeaponActor))
		{
			WeaponActor->Destroy();
		}

		if (IsValid(FirstPersonWeapon))
		{
			FirstPersonWeapon->Destroy();
		}
	}
}

void APEPlayerCharacter::UpdateTeamTagDisplay(const FText& InTag)
{
	if (HealthBarWidgetComponent)
	{
		UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HealthBarWidgetComponent->GetWidget());
		if (!IsValid(HealthBarWidget))
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid HealthBarWidget"))
			return;
		}

		HealthBarWidget->TeamTag->SetText(InTag);
		HealthBarWidget->TeamTag->SetColorAndOpacity(ETeam_GetTeamColor(ETeam_GetTeamFromString(InTag)));

		if (!IsValid(PlayerHUD))
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerHUD"))
			return;
		}
			
		PlayerHUD->HUDTeamTagTextBlock->SetText(InTag);
		PlayerHUD->HUDTeamTagTextBlock->SetColorAndOpacity(ETeam_GetTeamColor(ETeam_GetTeamFromString(InTag)));
		
		bTeamTagTextUpdated = true;
	}
}

void APEPlayerCharacter::OnRep_CopiedTeamTagText()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_CopiedTeamTagText called for %s: %s"), *GetName(), *CopiedTeamTagText.ToString());
	UpdateTeamTagDisplay(CopiedTeamTagText);

	if (!IsValid(PlayerHUD))
	{
		return;
	}
	
	PlayerHUD->SetHUDTeamTag(CopiedTeamTagText);
	PlayerHUD->HUDTeamTagTextBlock->SetColorAndOpacity(ETeam_GetTeamColor(ETeam_GetTeamFromString(CopiedTeamTagText)));
}

void APEPlayerCharacter::ServerSetCopiedTeamTagText_Implementation(APEPlayerState* InPlayerState)
{
	if (IsValid(InPlayerState))
	{
		CopiedTeamTagText = InPlayerState->TeamTagText;
	}
}

void APEPlayerCharacter::TryUpdateTeamTagDisplay()
{
	APEPlayerState* InPlayerState = GetPlayerState<APEPlayerState>();
	if (IsValid(InPlayerState))
	{
		UpdateTeamTagDisplay(InPlayerState->TeamTagText);
	}
}

void APEPlayerCharacter::ServerRunEvent_Implementation()
{
	if (!HasAuthority())
	{
		return;
	}
	
	if (!bIsRunning)
	{
		if (!RunningGameplayEffect->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Error, TEXT("RunningGameplayEffect is not a valid low-level object!"));
		}
		
		RunningEffectContext = AbilitySystemComponent->MakeEffectContext();
		RunningEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(RunningGameplayEffect, 1.0f, RunningEffectContext);

		if (!RunningEffectSpecHandle.IsValid())
		{
			return;
		}

		RunningEffectSpec = RunningEffectSpecHandle.Data.Get();
		if (RunningEffectSpec)
		{
			RunningEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*RunningEffectSpec);
			bIsRunning = true;
		}
	}
}

void APEPlayerCharacter::ServerStopRunEvent_Implementation()
{
	AbilitySystemComponent->RemoveActiveGameplayEffect(RunningEffectHandle);
	bIsRunning = false;
}

// float APEPlayerCharacter::GetMaxHealthMaxValue()
// {
// 	return DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.MaxHealth"), TEXT("Could not find PEBaseAttributeSet.MaxHealth"))->MaxValue;
// }

void APEPlayerCharacter::ServerActivateHitIndicator_Implementation(EDirectionDamageIndicator InDirection, FLinearColor InColor)
{
	APEPlayerController* PlayerController = Cast<APEPlayerController>(GetController());
	if (IsValid(PlayerController))
	{
		PlayerController->ClientProcessActivateHitIndicator(InDirection, InColor);
	}
}

void APEPlayerCharacter::MulticastPlayAnimMontage_Implementation(UAnimMontage* InAnimMontage)
{
	PlayAnimMontage(InAnimMontage);
}

void APEPlayerCharacter::PlayFPSArmsAnimMontage(UAnimMontage* InAnimMontage)
{
	if (!IsValid(InAnimMontage))
	{
		return;
	}

	UAnimInstance* AnimInstance = FPSArms->GetAnimInstance();
	if (!IsValid(AnimInstance))
	{
		return;
	}
	
	AnimInstance->Montage_Play(InAnimMontage);
}

void APEPlayerCharacter::ClientCleanupClientWeapon_Implementation()
{
	if (!IsLocallyControlled())
	{
		return;
	}
	
	if (IsValid(FirstPersonWeapon))
	{
		FirstPersonWeapon->Destroy();
	}

	if (IsValid(WeaponActor))
	{
		WeaponActor->Destroy();
	}
}

// Called when the game starts or when spawned.
void APEPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid AbilitySystemComponent"))
	}
	
	float InSpeed = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.Speed"), TEXT("Could not find PEBaseAttributeSet.Speed"))->BaseValue;
	float InMaxHealth = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.MaxHealth"), TEXT("Could not find PEBaseAttributeSet.MaxHealth"))->BaseValue;
	float InHealth = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.Health"), TEXT("Could not find PEBaseAttributeSet.Health"))->BaseValue;
	float InAttackSpeed = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.AttackSpeed"), TEXT("Could not find PEBaseAttributeSet.AttackSpeed"))->BaseValue;
	float InAbilityCostMultiplier = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.AbilityCostMultiplier"), TEXT("Could not find PEBaseAttributeSet.AbilityCostMultiplier"))->BaseValue;
	float InCooldownMultiplier = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.CooldownMultiplier"), TEXT("Could not find PEBaseAttributeSet.CooldownMultiplier"))->BaseValue;
	float InDamageDirectionFront = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.DamageDirectionFront"), TEXT("Could not find PEBaseAttributeSet.DamageDirectionFront"))->BaseValue;
	float InDamageDirectionSide = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.DamageDirectionSide"), TEXT("Could not find PEBaseAttributeSet.DamageDirectionSide"))->BaseValue;
	float InDamageDirectionBack = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.DamageDirectionBack"), TEXT("Could not find PEBaseAttributeSet.DamageDirectionBack"))->BaseValue;
	float InJumpMagnitude = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.JumpMagnitude"), TEXT("Could not find PEBaseAttributeSet.JumpMagnitude"))->BaseValue;
	float InResource = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.Resource"), TEXT("Could not find PEBaseAttributeSet.Resource"))->BaseValue;
	float InResourceMaxValue = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.Resource"), TEXT("Could not find PEBaseAttributeSet.Resource"))->MaxValue;
	float InResourceReplenishRate = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.ResourceReplenishRate"), TEXT("Could not find PEBaseAttributeSet.ResourceReplenishRate"))->BaseValue;

	float fOnTakeDamageResourceReplenishRate = 0.0f;
	FAttributeMetaData* OnTakeDamageResourceReplenishRate = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.OnTakeDamageResourceReplenishRate"), TEXT("Could not find PEBaseAttributeSet.OnTakeDamageResourceReplenishRate"));
	if (OnTakeDamageResourceReplenishRate != nullptr)
	{
		fOnTakeDamageResourceReplenishRate = OnTakeDamageResourceReplenishRate->BaseValue;
	}
	
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetSpeed(InSpeed);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetMaxHealth(InMaxHealth);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetHealth(InHealth);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetAttackSpeed(InAttackSpeed);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetAbilityCostMultiplier(InAbilityCostMultiplier);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetCooldownMultiplier(InCooldownMultiplier);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetDamageDirectionFront(InDamageDirectionFront);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetDamageDirectionSide(InDamageDirectionSide);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetDamageDirectionBack(InDamageDirectionBack);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetJumpMagnitude(InJumpMagnitude);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetResource(InResource);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetResourceMaxValue(InResourceMaxValue);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetResourceReplenishRate(InResourceReplenishRate);
	Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->SetOnTakeDamageResourceReplenishRate(fOnTakeDamageResourceReplenishRate);

	fMaxHealthMaxValue = DataTable->FindRow<FAttributeMetaData>(FName("PEBaseAttributeSet.MaxHealth"), TEXT("Could not find PEBaseAttributeSet.MaxHealth"))->MaxValue;
	
	UE_LOG(LogTemp, Warning, TEXT("My Speed is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetSpeed())
	UE_LOG(LogTemp, Warning, TEXT("My MaxHealth is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetMaxHealth())
	UE_LOG(LogTemp, Warning, TEXT("My Health is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetHealth())
	UE_LOG(LogTemp, Warning, TEXT("My AttackSpeed is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetAttackSpeed())
	UE_LOG(LogTemp, Warning, TEXT("My AbilityCostMultiplier is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetAbilityCostMultiplier())
	UE_LOG(LogTemp, Warning, TEXT("My CooldownMultiplier is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetCooldownMultiplier())
	UE_LOG(LogTemp, Warning, TEXT("My DamageDirectionFront is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetDamageDirectionFront())
	UE_LOG(LogTemp, Warning, TEXT("My DamageDirectionSide is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetDamageDirectionSide())
	UE_LOG(LogTemp, Warning, TEXT("My DamageDirectionBack is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetDamageDirectionBack())
	UE_LOG(LogTemp, Warning, TEXT("My JumpMagnitude is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetJumpMagnitude())
	UE_LOG(LogTemp, Warning, TEXT("My Resource is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetResource())
	UE_LOG(LogTemp, Warning, TEXT("My ResourceReplenishRate is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetResourceReplenishRate())
	UE_LOG(LogTemp, Warning, TEXT("My OnTakeDamageResourceReplenishRate is set to: %f"), Cast<UPEBaseCharacterAttributeSet>(AttributeSet)->GetOnTakeDamageResourceReplenishRate())

	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (!IsValid(CharacterMovementComponent))
	{
		return;
	}

	CharacterMovementComponent->MaxWalkSpeed = 1000.f;
	CharacterMovementComponent->AirControl = 0.2f;
	
	if (!IsValid(HUDClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid HUDClass"))
	}

	// if (IsValid(EtherCompassClass))
	// {
	// 	EtherCompassActor = Cast<APEEtherCompass>(GetWorld()->SpawnActor(EtherCompassClass));
	// 	EtherCompassActor->AttachToComponent(EtherCompassSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	// 	EtherCompassActor->SetOwner(this);
	// }
	//
	// if (IsValid(PassiveAbilityClass))
	// {
	// 	// PassiveAbilityComponent = NewObject<UPEBasePassiveAbilityComponent>(this, BP_PassiveAbilityComponent->StaticClass());
	// 	// PassiveAbilityComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	//
	// 	PassiveAbilityActor = Cast<APEBasePassiveAbilityActor>(GetWorld()->SpawnActor(PassiveAbilityClass));
	// 	PassiveAbilityActor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	// 	PassiveAbilityActor->SetOwner(this);
	// 	PassiveAbilityActor->SetupPassiveAbility();
	// }

	if (IsValid(HealthBarWidgetClass))
	{
		// UPEHealthBarWidget* HealthBarWidget = Cast<UPEHealthBarWidget>(HealthBarWidgetComponent->GetWidget());
		// HealthBarWidget->PlayerCharacterRef = this;
		
		HealthBarWidgetComponent->SetDrawAtDesiredSize(true);
		HealthBarWidgetComponent->SetWidgetClass(HealthBarWidgetClass);
		HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HealthBarWidgetComponent->SetOwnerPlayer(GetWorld()->GetFirstLocalPlayerFromController());
		HealthBarWidgetComponent->InitWidget();
	}

	if (IsValid(DamageDirectionComponentClass))
	{
		// DamageDirectionComponent->SetDrawAtDesiredSize(true);
		DamageDirectionComponent->SetWidgetClass(DamageDirectionComponentClass);
		DamageDirectionComponent->SetWidgetSpace(EWidgetSpace::World);
		DamageDirectionComponent->SetOwnerPlayer(GetWorld()->GetFirstLocalPlayerFromController());
		UPEDamageDirectionComponent* DDC = Cast<UPEDamageDirectionComponent>(DamageDirectionComponent->GetWidget());
		if (IsValid(DDC))
		{
			DDC->SetOwningWidgetComponent(DamageDirectionComponent);
		}
		
		DamageDirectionComponent->InitWidget();
	}

	if (IsValid(AbilityOneContainerClass))
	{
		AbilityOneContainer = NewObject<UAbilityContainer>(this,AbilityOneContainerClass);
	}
	if (IsValid(AbilityTwoContainerClass))
	{
		AbilityTwoContainer = NewObject<UAbilityContainer>(this,AbilityTwoContainerClass);
	}
	if (IsValid(AbilityThreeContainerClass))
	{
		AbilityThreeContainer = NewObject<UAbilityContainer>(this, AbilityThreeContainerClass);
	}
	
	if (IsLocallyControlled())
	{
		APEPlayerController* PC = Cast<APEPlayerController>(GetController());
		ensureMsgf(PC, TEXT("Was not able to reference PlayerController"));
		PlayerHUD = CreateWidget<UPEPlayerHUD>(PC, HUDClass);
		ensureMsgf(PlayerHUD, TEXT("PlayerHUD failed to instantiate properly"));
		PlayerHUD->AddToViewport();
		PC->SubscribeToGameState(this->GetClass());

		APEPlayerState* PEPlayerState = GetPlayerState<APEPlayerState>();
		if (IsValid(PEPlayerState))
		{
			if (!PEPlayerState->TeamTagText.IsEmpty())
			{
				UpdateTeamTagDisplay(PEPlayerState->TeamTagText);
			}
			else
			{
				ServerSetCopiedTeamTagText(PEPlayerState);
			}
		}
		
		FPSArms->SetVisibility(true);

		FirstPersonWeapon = Cast<APEWeapon>(GetWorld()->SpawnActor(WeaponClass));
		FirstPersonWeapon->AttachToComponent(FPSArms, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FirstPersonWeapon->WeaponSocket);

		const USkeletalMeshSocket* FirstPersonWeaponSkeletalMesh = FirstPersonWeapon->WeaponSkeletalMeshComponent->GetSocketByName("top_hand_JntSocket");
		if (IsValid(FirstPersonWeaponSkeletalMesh))
		{
			FVector HandleJointLocation = FirstPersonWeaponSkeletalMesh->GetSocketTransform(FirstPersonWeapon->WeaponSkeletalMeshComponent).GetLocation();
			FVector WeaponOriginLocation = FirstPersonWeapon->GetTransform().GetLocation();
			FirstPersonWeapon->AddActorWorldTransform(FTransform(FVector(WeaponOriginLocation - HandleJointLocation)));
			FirstPersonWeapon->AddActorLocalRotation(FirstPersonWeaponSkeletalMesh->GetSocketLocalTransform().GetRotation());	
		}
		FirstPersonWeapon->SetOwner(this);

		// FPSBasicMeleeAttackMontage = FirstPersonWeapon->MainWeaponAbilityContainer.FPSAnimMontage;
		// FPSSecondaryMeleeAttackMontage = FirstPersonWeapon->SecondaryWeaponAbilityContainer.FPSAnimMontage;
		GetMesh()->SetVisibility(false);
	}
	
	if (GetNetMode() < NM_Client)
	{
		if (IsValid(EtherCompassClass))
		{
			EtherCompassActor = Cast<APEEtherCompass>(GetWorld()->SpawnActor(EtherCompassClass));
			EtherCompassActor->AttachToComponent(EtherCompassSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			EtherCompassActor->SetOwner(this);
		}

		if (IsValid(PassiveAbilityClass))
		{
			// PassiveAbilityComponent = NewObject<UPEBasePassiveAbilityComponent>(this, BP_PassiveAbilityComponent->StaticClass());
			// PassiveAbilityComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			PassiveAbilityActor = Cast<APEBasePassiveAbilityActor>(GetWorld()->SpawnActor(PassiveAbilityClass));
			PassiveAbilityActor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			PassiveAbilityActor->SetOwner(this);
			PassiveAbilityActor->SetupPassiveAbility();
		}
		
		// PlayerCharacters must have their Weapons equipped before starting.
		if (IsValid(WeaponClass))
		{
			WeaponActor = Cast<APEWeapon>(GetWorld()->SpawnActor(WeaponClass));
			WeaponActor->AttachToComponent(this->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), WeaponActor->WeaponSocket);

			const USkeletalMeshSocket* SocketOffset = WeaponActor->WeaponSkeletalMeshComponent->GetSocketByName("top_hand_JntSocket");
			if (IsValid(SocketOffset))
			{
				FVector HandleJointLocation = SocketOffset->GetSocketTransform(WeaponActor->WeaponSkeletalMeshComponent).GetLocation();
				FVector WeaponOriginLocation = WeaponActor->GetTransform().GetLocation();
				WeaponActor->AddActorWorldTransform(FTransform(FVector(WeaponOriginLocation - HandleJointLocation)));	
			}
			WeaponActor->SetOwner(this);

			if (IsValid(WeaponActor->MainWeaponAbilityContainer))
			{
				WeaponAbilityOneHandle = AbilitySystemComponent->GiveAbility(WeaponActor->MainWeaponAbilityContainer->Ability);
			}
			if (IsValid(WeaponActor->SecondaryWeaponAbilityContainer))
			{
				WeaponAbilityTwoHandle = AbilitySystemComponent->GiveAbility(WeaponActor->SecondaryWeaponAbilityContainer->Ability);
			}
			
			// BasicMeleeAttackMontage = WeaponActor->MainWeaponAbilityContainer.CharacterAnimMontage;
			// SecondaryMeleeAttackMontage = WeaponActor->SecondaryWeaponAbilityContainer.CharacterAnimMontage;
		}
		
		if (IsValid(AbilityOneContainer))
		{
			AbilityOneHandle = AbilitySystemComponent->GiveAbility(AbilityOneContainer->Ability);
		}
		if (IsValid(AbilityTwoContainer))
		{
			AbilityTwoHandle = AbilitySystemComponent->GiveAbility(AbilityTwoContainer->Ability);
		}
		if (IsValid(AbilityThreeContainer))
		{
			AbilityThreeHandle = AbilitySystemComponent->GiveAbility(AbilityThreeContainer->Ability);
		}
	}
}

// Called every frame
void APEPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	IsLookingAtInteractable();
	if (!bTeamTagTextUpdated)
	{
		TryUpdateTeamTagDisplay();
	}

	if (IsLocallyControlled())
	{
		PlayerHUD->UpdateGenericResourceBar(AttributeSet->GetResource()/AttributeSet->GetResourceMaxValue());
		PlayerHUD->UpdateResourceTexts(AttributeSet);

		if (IsValid(WeaponActor) && WeaponActor->WeaponSkeletalMeshComponent->IsVisible())
		{
			WeaponActor->WeaponSkeletalMeshComponent->SetVisibility(false);
		}
	}
	
	// if (IsValid(WeaponActor) && WeaponActor->WeaponSkeletalMeshComponent->IsVisible())
	// {
	// 	WeaponActor->WeaponSkeletalMeshComponent->SetVisibility(false);
	// }

	if (bIsChained)
	{
		FVector deltaLocation = ChainSpringInterpolator.Update(GetActorLocation(), ChainSpringInterpolator.GetPosition(), DeltaTime);
		SetActorLocation(deltaLocation);
	}
}

void APEPlayerCharacter::OnRep_DamageDirectionRotation() const
{
	DamageDirectionComponent->SetWorldRotation(DamageDirectionRotation);
}

void APEPlayerCharacter::ServerSetDamageDirectionImageRotation_Implementation(const FRotator& InRotation)
{
	DamageDirectionRotation = InRotation;
	OnRep_DamageDirectionRotation();
}

void APEPlayerCharacter::AllowBasicAttack(const APEPlayerCharacter* TargetPlayerCharacter)
{
	if (!IsValid(TargetPlayerCharacter))
	{
		return;
	}
	
	APEPlayerController* TargetPlayerController = Cast<APEPlayerController>(TargetPlayerCharacter->GetController());
	TargetPlayerController->bCanBasicAttack = true;
}

void APEPlayerCharacter::ChainPlayer(FVector OriginVector, float Duration, FRK4SpringInterpolator<FVector> InSpring)
{
	ChainSpringInterpolator = InSpring;
	bIsChained = true;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this](){bIsChained = false;});

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, Duration, false);
	
	ChainSpringInterpolator.SetPosition(GetActorLocation());
	ChainedTargetLocation = OriginVector;
}
