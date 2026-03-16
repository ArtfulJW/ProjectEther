// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"

#include "EnhancedInputComponent.h"
// #include "InputState.h"
#include "PEGameOptionsOverlay.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEBaseGameplayAbility.h"
#include "PEEquipmentCache.h"
#include "PEEther.h"
#include "PEGameState.h"
#include "PEBaseGameplayAbility.h"
#include "PEPlayerCharacter.h"
#include "PEPriestCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

class APEPlayerCharacter;

void APEPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		PEPickClassHUD = CreateWidget<UPEPickClassHUD>(this, PEPickClassHUDClass);
		PEPickClassHUD->AddToViewport();
		SetShowMouseCursor(true);
	}

	// APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	// GameState->ServerAssignPlayerToTeam(this);
	//
	// const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	// if (!IsValid(LocalPlayer))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Invalid LocalPlayer"));
	// }
	//
	// UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	// if (IsValid(InputSystem) && IsValid(InputMapping))
	// {
	// 	InputSystem->AddMappingContext(InputMapping, 0);
	// }
	//
	// GameState->ServerSpawnPlayerCharacter(this, this->CharacterClass);
}

void APEPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent);
	Input->BindAction(MoveAction,ETriggerEvent::Triggered, this, &APEPlayerController::MoveEvent);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APEPlayerController::LookEvent);
	Input->BindAction(AbilityAction, ETriggerEvent::Triggered, this, &APEPlayerController::UseAbilityEvent);
	Input->BindAction(WeaponAction, ETriggerEvent::Triggered, this, &APEPlayerController::UseWeaponEvent);
	Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APEPlayerController::InteractEvent);
	Input->BindAction(DeployInteractableAction, ETriggerEvent::Triggered, this, &APEPlayerController::DeployInteractableEvent);
	Input->BindAction(CheckCompassAction, ETriggerEvent::Triggered, this, &APEPlayerController::CheckCompassEvent);
	Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APEPlayerController::JumpEvent);
	Input->BindAction(RunAction, ETriggerEvent::Triggered, this, &APEPlayerController::RunEvent);
	Input->BindAction(OptionsAction, ETriggerEvent::Triggered, this, &APEPlayerController::OptionsWindowEvent);
}

void APEPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEPlayerController, Team);
	DOREPLIFETIME(APEPlayerController, CharacterClass);
	DOREPLIFETIME(APEPlayerController, bCanBasicAttack);
}

APEPlayerController::APEPlayerController():
InputMapping(nullptr),
MoveAction(nullptr),
LookAction(nullptr),
AbilityAction(nullptr),
WeaponAction(nullptr),
InteractAction(nullptr),
DeployInteractableAction(nullptr),
CheckCompassAction(nullptr),
PEPickClassHUD(nullptr),
Team(),
bCanBasicAttack(true)
{
}

void APEPlayerController::MoveEvent(const FInputActionValue& Value)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
	}
	
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (IsValid(PC) && PC->AttributeSet->GetHealth() > 0)
	{
		// Getting and rotating the input directions (Cardinal) by the current Yaw rotation of the PlayerCharacter
		FVector Direction = Value.Get<FVector>();
		FRotator YawRotation(0, PC->GetActorRotation().Yaw, 0);
		Direction = YawRotation.RotateVector(Direction);
		
		float WorldDeltaSeconds = World->GetDeltaSeconds();
		PC->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		float fMoveSpeed = PC->AbilitySystemComponent->GetSet<UPEBaseCharacterAttributeSet>()->GetSpeed();
		PC->AddMovementInput(Direction, fMoveSpeed);
	}
}

void APEPlayerController::ServerMovePlayer_Implementation(APEPlayerController* Requester, FVector InVector)
{
	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(Requester->GetPawn());
	if (!IsValid(PlayerCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerMovePlayer: Cannot move an invalid PlayerCharacter"));
		return;
	}
	
	// PlayerCharacter->AddActorLocalTransform(FTransform(InVector));
	// PlayerCharacter->GetMovementComponent()->AddInputVector(InVector, true);
	// PlayerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    
	PlayerCharacter->AddMovementInput(InVector);
}

void APEPlayerController::LookEvent(const FInputActionValue& Value)
{
	if (IsPossessingSpectatorPawn(this))
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
	}
	
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (IsValid(PC))
	{
		FVector InVector = Value.Get<FVector>();
		PC->AddControllerPitchInput(InVector.Y);
		PC->AddControllerYawInput(InVector.X);
		ServerLookEvent(this, PC->CameraComponent->GetRelativeRotation());
	}
}

void APEPlayerController::ServerLookEvent_Implementation(APEPlayerController* Requester, FRotator InRotator)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerLookEvent: Invalid PlayerCharacter"));
		return;
	}
	
	PC->CameraComponent->SetRelativeRotation(InRotator);
}

void APEPlayerController::UseAbilityEvent(const FInputActionValue& Value)
{
	if (IsPossessingSpectatorPawn(this))
	{
		return;
	}

	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerCharacter"));
		return;
	}
	
	if (PlayerInput->IsPressed(EKeys::Two) && IsValid(PC->AbilityOneContainer) && bCanBasicAttack)
	{
		if (PC->AttributeSet->GetResource() - PC->AbilityOneContainer->AbilityCost < 0)
		{
			return;
		}
		// PC->AttributeSet->SetResource(PC->AttributeSet->GetResource() - PC->AbilityOneContainer->AbilityCost);
		
		ServerUseGameplayAbilityEvent(this, PC->AbilityOneHandle, PC->AbilityOneContainer->CharacterAnimMontage, PC->AbilityOneContainer->AbilityCost, PC->AttributeSet->GetResource());
		// PC->PlayFPSArmsAnimMontage(PC->AbilityOneContainer->FPSAnimMontage);
		ServerCheckToPlayFPSAnimations(PC, PC->AbilityOneContainer->FPSAnimMontage, PC->AbilityOneHandle);
	}
	
	if (PlayerInput->IsPressed(EKeys::Three) && IsValid(PC->AbilityTwoContainer) && bCanBasicAttack)
	{
		if (PC->AttributeSet->GetResource() - PC->AbilityTwoContainer->AbilityCost < 0)
		{
			return;
		}
		// PC->AttributeSet->SetResource(PC->AttributeSet->GetResource() - PC->AbilityTwoContainer->AbilityCost);
		
		ServerUseGameplayAbilityEvent(this, PC->AbilityTwoHandle, PC->AbilityTwoContainer->CharacterAnimMontage, PC->AbilityTwoContainer->AbilityCost, PC->AttributeSet->GetResource());
		// PC->PlayFPSArmsAnimMontage(PC->AbilityTwoContainer->FPSAnimMontage);
		ServerCheckToPlayFPSAnimations(PC, PC->AbilityTwoContainer->FPSAnimMontage, PC->AbilityTwoHandle);
	}
	
	if (PlayerInput->IsPressed(EKeys::Four) && IsValid(PC->AbilityThreeContainer) && bCanBasicAttack)
	{
		if (PC->AttributeSet->GetResource() - PC->AbilityThreeContainer->AbilityCost < 0)
		{
			return;
		}
		// PC->AttributeSet->SetResource(PC->AttributeSet->GetResource() - PC->AbilityThreeContainer->AbilityCost);
		
		ServerUseGameplayAbilityEvent(this, PC->AbilityThreeHandle, nullptr, PC->AbilityThreeContainer->AbilityCost, PC->AttributeSet->GetResource());
		// PC->PlayFPSArmsAnimMontage(PC->AbilityThreeContainer->FPSAnimMontage);
		ServerCheckToPlayFPSAnimations(PC, PC->AbilityThreeContainer->FPSAnimMontage, PC->AbilityThreeHandle);
	}
}

void APEPlayerController::ServerUseGameplayAbilityEvent_Implementation(APEPlayerController* Requester, FGameplayAbilitySpecHandle AbilityHandle, UAnimMontage* InAnimMontage, float
                                                                       ResourceCost, float CurrentResourceAmnt)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	if (!IsValid(PC))
	{
		return;
	}

	if (GetNetMode() < NM_Client)
	{
		FGameplayAbilitySpec* Spec = PC->AbilitySystemComponent->FindAbilitySpecFromHandle(AbilityHandle);
		if (!Spec)
		{
			return;
		}

		UPEBaseGameplayAbility* BaseGameplayAbility = Cast<UPEBaseGameplayAbility>(Spec->GetPrimaryInstance());
		if (!BaseGameplayAbility)
		{
			return;
		}

		UPEAbilityCooldownComponent* CooldownComponent = BaseGameplayAbility->AbilityCooldownComponent;
		if (!CooldownComponent)
		{
			return;
		}

		if (CooldownComponent->CheckIsAbilityReady())
		{
			PC->AttributeSet->SetResource(CurrentResourceAmnt - ResourceCost);
			PC->AbilitySystemComponent->TryActivateAbility(AbilityHandle);
			if (IsValid(InAnimMontage))
			{
				PC->MulticastPlayAnimMontage(InAnimMontage);
			}
		}
	}
	
	// PC->AbilitySystemComponent->TryActivateAbility(AbilityHandle);
	bCanBasicAttack = false;
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APEPlayerController::ReplenishBasicAttack, 1/PC->AttributeSet->GetAttackSpeed(), false);
}

void APEPlayerController::UseWeaponEvent(const FInputActionValue& Value)
{
	if (IsPossessingSpectatorPawn(this))
	{
		return;
	}

	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerCharacter"));
		return;
	}

	if (!IsValid(PC->WeaponActor))
	{
		return;
	}
	
	if (PlayerInput->IsPressed(EKeys::LeftMouseButton) && bCanBasicAttack)
	{
		if (!IsValid(PC->WeaponActor->MainWeaponAbilityContainer))
		{
			return;
		}

		if (PC->AttributeSet->GetResource() -  PC->WeaponActor->MainWeaponAbilityContainer->AbilityCost < 0)
		{
			return;
		}
		
		ServerUseGameplayAbilityEvent(this, PC->WeaponAbilityOneHandle, PC->WeaponActor->MainWeaponAbilityContainer->CharacterAnimMontage, PC->WeaponActor->MainWeaponAbilityContainer->AbilityCost, PC->AttributeSet->GetResource());
		
		ServerCheckToPlayFPSAnimations(PC, PC->WeaponActor->MainWeaponAbilityContainer->FPSAnimMontage, PC->WeaponAbilityOneHandle);
		
	}
	if (PlayerInput->IsPressed(EKeys::RightMouseButton) && bCanBasicAttack)
	{
		if (!IsValid(PC->WeaponActor->SecondaryWeaponAbilityContainer))
		{
			return;
		}

		if (PC->AttributeSet->GetResource() -  PC->WeaponActor->MainWeaponAbilityContainer->AbilityCost < 0)
		{
			return;
		}
		
		ServerUseGameplayAbilityEvent(this, PC->WeaponAbilityTwoHandle, PC->WeaponActor->SecondaryWeaponAbilityContainer->CharacterAnimMontage, 0.0f, PC->AttributeSet->GetResource());
		
		ServerCheckToPlayFPSAnimations(PC, PC->WeaponActor->MainWeaponAbilityContainer->FPSAnimMontage, PC->WeaponAbilityTwoHandle);
	}
}

bool APEPlayerController::IsPossessingSpectatorPawn(APEPlayerController* Requester)
{
	return  Requester->GetPawn()->IsA(ASpectatorPawn::StaticClass());
}

void APEPlayerController::ServerInteractEvent_Implementation(APEPlayerController* Requester, AActor* InActor)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	if (!IsValid(PC))
	{
		return;
	}
	
	APEInteractableBase* InteractableActor = Cast<APEInteractableBase>(InActor);
	if (!IsValid(InteractableActor))
	{
		return;
	}
	
	if (!InteractableActor->IsA(APEInteractableBase::StaticClass()))
	{
		return;
	}
	
	InteractableActor->Carrier = PC;
	// InteractableActor->ApplyCarryEffect();
	// PC->CarriedInteractableActor = InActor;
	// InteractableActor->MulticastSetSimulatePhysics(false);
	InteractableActor->Interact(*PC);
	
	UE_LOG(LogTemp, Warning, TEXT("Server Interacting with Ether"));
}

void APEPlayerController::InteractEvent()
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (!IsValid(PC))
	{
		return;
	}

	if (PC->CarriedInteractableActor)
	{
		ServerDropInteractableActor(this);
		return;
	}
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PC->CameraComponent->GetComponentLocation(), PC->CameraComponent->GetComponentLocation() + PC->CameraComponent->GetForwardVector() * PC->fInteractDistance, ECC_Visibility);
	AActor* Actor = Hit.GetActor();

	// DrawDebugLine(GetWorld(), PC->CameraComponent->GetComponentLocation(), PC->CameraComponent->GetComponentLocation() + PC->CameraComponent->GetForwardVector() * 10000, FColor::Black, false, 5.0f);

	if (!IsValid(Actor))
	{
		return;
	}
	
	if (Actor->IsA(APEEquipmentCache::StaticClass()))
	{
		APEEquipmentCache* EquipmentCache = Cast<APEEquipmentCache>(Actor);
		if (!IsValid(EquipmentCache) || EquipmentCache->bIsDeployed || EquipmentCache->Team != Team)
		{
			if (PC->IsA(APEPriestCharacter::StaticClass()))
			{
				APEPriestCharacter* PriestCharacter = Cast<APEPriestCharacter>(PC);
				PriestCharacter->ReplenishScrolls();
			}
			return;
		}
	}

	if (Actor->IsA(APEEther::StaticClass()))
	{
		APEEther* Ether = Cast<APEEther>(Actor);
		if (!IsValid(Ether) || Ether->bIsDeposited || IsValid(Ether->Carrier))
		{
			return;
		}
	}
	
	if (UKismetSystemLibrary::DoesImplementInterface(Actor, UInteractableInterface::StaticClass()))
	{
		ServerInteractEvent(this, Actor);
	}
}

void APEPlayerController::DeployInteractableEvent()
{
	FHitResult Hit;
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	GetWorld()->LineTraceSingleByChannel(Hit, PC->CameraComponent->GetComponentLocation(), PC->CameraComponent->GetComponentLocation() + PC->CameraComponent->GetForwardVector() * 10000, ECC_Visibility);

	// DrawDebugLine(GetWorld(), PC->CameraComponent->GetComponentLocation(), PC->CameraComponent->GetComponentLocation() + PC->CameraComponent->GetForwardVector() * 10000, FColor::Black, false, 5.0f);
	
	AActor* HitActor = Hit.GetActor();
	if (!IsValid(HitActor))
	{
		return;
	}
	
	if (!HitActor->IsA(APEEquipmentCache::StaticClass()) || HitActor == PC->CarriedInteractableActor)
	{
		return;		
	}

	APEEquipmentCache* EquipmentCache = Cast<APEEquipmentCache>(HitActor);
	if (IsValid(EquipmentCache))
	{
		if (EquipmentCache->Team != Team)
		{
			ServerDestroyEquipmentCache(EquipmentCache);
		}
		else
		{
			ServerDeployInteractable(EquipmentCache);	
		}
	}
}

void APEPlayerController::CheckCompassEvent()
{
	ServerCheckCompassEvent(this);
}

void APEPlayerController::JumpEvent()
{
 	ServerJumpEvent();
}

void APEPlayerController::RunEvent()
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (!IsValid(PC))
	{
		return;
	}
	
	if (!PC->bIsRunning)
	{
		PC->ServerRunEvent();
	}
	else
	{
		PC->ServerStopRunEvent();
	}
}

void APEPlayerController::SubscribeToGameState(TSubclassOf<APEPlayerCharacter> PossessedCharacter)
{
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GameState->PlayerControllerCharacterArray.Contains(this))
	{
		GameState->PlayerControllerCharacterArray.Add(this, PossessedCharacter);
	}
}

void APEPlayerController::ClientSetupInputControls_Implementation()
{
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid LocalPlayer"));
	}
	
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if (IsValid(InputSystem) && IsValid(InputMapping))
	{
		InputSystem->AddMappingContext(InputMapping, 0);
	}
}

void APEPlayerController::RequestSpawn_Implementation()
{
	APEGameState* GameState = Cast<APEGameState>(GetWorld()->GetGameState());
	GameState->ServerSpawnPlayerCharacter(this, this->CharacterClass);
}

void APEPlayerController::ServerSetCharacterClass_Implementation(APEPlayerController* Requester, EClassType InClassType)
{
	Requester->CharacterClass = InClassType;
}

void APEPlayerController::ClientRemovePickClassHUD_Implementation()
{
	if (IsValid(PEPickClassHUD))
	{
		PEPickClassHUD->RemoveFromParent();
		SetShowMouseCursor(false);
	}
}

void APEPlayerController::RequestTeamAssignment_Implementation()
{
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->ServerAssignPlayerToTeam(this);
}

void APEPlayerController::ReplenishBasicAttack()
{
	bCanBasicAttack = true;
}

void APEPlayerController::ServerJumpEvent_Implementation()
{
	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(GetPawn());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	UPEBaseCharacterAttributeSet* PCAttributeSet = PlayerCharacter->AttributeSet;
	if (!IsValid(PCAttributeSet))
	{
		return;
	}

	// TODO: Use this when Mage finally gets their own skeletalmesh
	// USkeletalMeshComponent* PlayerCharacterSkeletalMesh = Cast<USkeletalMeshComponent>(PlayerCharacter->GetMesh());

	UStaticMeshComponent* StaticMeshComponent = PlayerCharacter->StaticMeshComponent;
	if (!IsValid(StaticMeshComponent))
	{
		return;
	}
	
	// Line Trace to the floor to see if we're still in the air
	// FHitResult Hit;
	// GetWorld()->LineTraceSingleByChannel(Hit, PlayerCharacterSkeletalMesh->GetComponentLocation(), PlayerCharacterSkeletalMesh->GetComponentLocation() + (PlayerCharacterSkeletalMesh->GetUpVector() * -1) * 30, ECC_Visibility);

	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, StaticMeshComponent->GetComponentLocation(), StaticMeshComponent->GetComponentLocation() + (StaticMeshComponent->GetUpVector() * -1) * 30, ECC_Visibility);

	
	if (!IsValid(Hit.GetComponent()))
	{
		return;
	}
	
	PlayerCharacter->GetCharacterMovement()->AddImpulse(PCAttributeSet->GetJumpMagnitude() * FVector(0,0,1));
}

void APEPlayerController::ServerRunEvent_Implementation()
{
}

void APEPlayerController::ServerRequestSpectatorSpawn_Implementation()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(World));
	if (!IsValid(GameState))
	{
		return;
	}

	APEEtherSpawnRegion* EtherSpawnRegion = GameState->EtherSpawnRegion;
	if (!IsValid(EtherSpawnRegion))
	{
		return;
	}
	
	ASpectatorPawn* SpawnedActor = World->SpawnActor<ASpectatorPawn>(EtherSpawnRegion->GetTransform().GetLocation(), EtherSpawnRegion->GetTransform().Rotator());
	if (!IsValid(SpawnedActor))
	{
		return;
	}
	
	this->Possess(SpawnedActor);
}

void APEPlayerController::ClientProcessActivateHitIndicator_Implementation(
	EDirectionDamageIndicator InDirectionDamageIndicator, FLinearColor InColor)
{
	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(GetPawn());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	PlayerCharacter->PlayerHUD->ActivateHitIndicator(InDirectionDamageIndicator, InColor);
}

void APEPlayerController::OptionsWindowEvent()
{
	if (!IsValid(GameOptionsOverlay))
	{
		GameOptionsOverlay = CreateWidget<UPEGameOptionsOverlay>(this, OptionsOverlayClass);
		GameOptionsOverlay->AddToViewport();

		// FInputModeUIOnly InputMode;
		// InputMode.SetWidgetToFocus(GameOptionsOverlay->TakeWidget());
		// InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		
		SetInputMode(FInputModeGameAndUI());
		SetShowMouseCursor(true);
		bShowMouseCursor = true;
		
		return;
	}
	
	this->SetInputMode(FInputModeGameOnly());
	
	GameOptionsOverlay->RemoveFromParent();
	bShowMouseCursor = false;
	// bEnableClickEvents = false;
	// bEnableMouseOverEvents = false;
	GameOptionsOverlay = nullptr;
}

void APEPlayerController::OnPossess(APawn* APawn)
{
	Super::OnPossess(APawn);

	ClientRemovePickClassHUD();
}

void APEPlayerController::ServerCheckCompassEvent_Implementation(APEPlayerController* Requester)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	if (!IsValid(PC))
	{
		return;
	}

	APEEtherCompass* EtherCompass = PC->EtherCompassActor;
	EtherCompass->SetIsCheckingCompass(!EtherCompass->bIsTakenOut);
}

void APEPlayerController::ServerDestroyEquipmentCache_Implementation(APEEquipmentCache* EquipmentCache)
{
	EquipmentCache->Destroy();
	
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!IsValid(GameState))
	{
		return;
	}

	GameState->ServerRemoveEquipmentCache(EquipmentCache, EquipmentCache->Team);
}

void APEPlayerController::ServerDeployInteractable_Implementation(APEInteractableBase* InActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Server Deploy: %s"), *InActor->GetName())
	InActor->Deploy();
}

void APEPlayerController::ServerDropInteractableActor_Implementation(APEPlayerController* Requester)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (!IsValid(PC->CarriedInteractableActor))
	{
		return;
	}
	
	if (PC->CarriedInteractableActor)
	{
		PC->CarriedInteractableActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UE_LOG(LogTemp, Warning, TEXT("Server drop Interactable Actor"));
	}

	if (PC->CarriedInteractableActor->IsA(APEInteractableBase::StaticClass()))
	{
		APEInteractableBase* InteractableActor = Cast<APEInteractableBase>(PC->CarriedInteractableActor);
		if (!IsValid(InteractableActor))
		{
			return;
		}
		InteractableActor->MulticastSetSimulatePhysics(true);
		InteractableActor->RemoveCarryEffect();
		InteractableActor->Carrier = nullptr;
	}
	
	PC->CarriedInteractableActor = nullptr;
}

void APEPlayerController::ServerCheckToPlayFPSAnimations_Implementation(APEPlayerCharacter* PC, UAnimMontage* InAnimMontage, FGameplayAbilitySpecHandle AbilityHandle)
{
	// On on the server, I check if this specific client is supposed to play their FPS anim. If so, request that they do.
	if (GetNetMode() >= NM_Client)
	{
		return;
	}

	FGameplayAbilitySpec* Spec = PC->AbilitySystemComponent->FindAbilitySpecFromHandle(AbilityHandle);
	if (!Spec)
	{
		return;
	}

	UPEBaseGameplayAbility* BaseGameplayAbility = Cast<UPEBaseGameplayAbility>(Spec->GetPrimaryInstance());
	if (!BaseGameplayAbility)
	{
		return;
	}

	UPEAbilityCooldownComponent* CooldownComponent = BaseGameplayAbility->AbilityCooldownComponent;
	if (!CooldownComponent)
	{
		return;
	}
	
	if (CooldownComponent->CheckIsAbilityReady())
	{
		ClientPlayFPSAnim(PC, InAnimMontage);
	}
}

void APEPlayerController::ClientPlayFPSAnim_Implementation(APEPlayerCharacter* PC, UAnimMontage* InAnimMontage)
{
	PC->PlayFPSArmsAnimMontage(InAnimMontage);
}