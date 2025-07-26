// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"

#include "EnhancedInputComponent.h"
#include "InputState.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEEquipmentCache.h"
#include "PEEther.h"
#include "PEGameState.h"
#include "PEPlayerCharacter.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

class APEPlayerCharacter;

void APEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->ServerAssignPlayerToTeam(this);
	
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
}

void APEPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEPlayerController, Team);
}

void APEPlayerController::MoveEvent(const FInputActionValue& Value)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
	}
	
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (IsValid(PC))
	{
		FVector Direction = Value.Get<FVector>();	
		ServerMovePlayer(this, Direction * PC->AbilitySystemComponent->GetSet<UPEBaseCharacterAttributeSet>()->GetSpeed());
	}
}

void APEPlayerController::ServerMovePlayer_Implementation(APEPlayerController* Requester, FVector InVector)
{
	Cast<APEPlayerCharacter>(Requester->GetPawn())->AddActorLocalTransform(FTransform(InVector));
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
	PC->CameraComponent->SetRelativeRotation(InRotator);
}

void APEPlayerController::UseAbilityEvent(const FInputActionValue& Value)
{
	if (IsPossessingSpectatorPawn(this))
	{
		return;
	}

	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (PlayerInput->IsPressed(EKeys::Two))
	{
		ServerUseGameplayAbilityEvent(this, PC->AbilityOneHandle);
	}
	
	if (PlayerInput->IsPressed(EKeys::Three))
	{
		ServerUseGameplayAbilityEvent(this, PC->AbilityTwoHandle);
	}
	
	if (PlayerInput->IsPressed(EKeys::Four))
	{
		ServerUseGameplayAbilityEvent(this, PC->AbilityThreeHandle);
	}
}

void APEPlayerController::ServerUseGameplayAbilityEvent_Implementation(APEPlayerController* Requester,
	FGameplayAbilitySpecHandle AbilityHandle)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	PC->AbilitySystemComponent->TryActivateAbility(AbilityHandle);
}

void APEPlayerController::UseWeaponEvent(const FInputActionValue& Value)
{
	if (IsPossessingSpectatorPawn(this))
	{
		return;
	}

	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (PlayerInput->IsPressed(EKeys::LeftMouseButton))
	{
		ServerUseGameplayAbilityEvent(this, PC->WeaponAbilityOneHandle);
	}
	if (PlayerInput->IsPressed(EKeys::RightMouseButton))
	{
		ServerUseGameplayAbilityEvent(this, PC->WeaponAbilityTwoHandle);
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
	if (!InteractableActor->IsA(APEInteractableBase::StaticClass()))
	{
		return;
	}
	
	InteractableActor->Carrier = PC;
	InteractableActor->ApplyCarryEffect();
	PC->CarriedInteractableActor = InActor;
	InteractableActor->MulticastSetSimulatePhysics(false);
	InteractableActor->Interact();
	
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
	GetWorld()->LineTraceSingleByChannel(Hit, PC->CameraComponent->GetComponentLocation(), PC->CameraComponent->GetForwardVector() * 10000, ECC_Visibility);
	AActor* Actor = Hit.GetActor();

	if (Actor->IsA(APEEquipmentCache::StaticClass()))
	{
		APEEquipmentCache* EquipmentCache = Cast<APEEquipmentCache>(Actor);
		if (EquipmentCache && EquipmentCache->bIsDeployed || EquipmentCache->Team != Team)
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
	GetWorld()->LineTraceSingleByChannel(Hit, PC->CameraComponent->GetComponentLocation(), PC->CameraComponent->GetForwardVector() * 10000, ECC_Visibility);

	AActor* HitActor = Hit.GetActor();
	if (!IsValid(HitActor))
	{
		return;
	}

	APEInteractableBase* InteractableActor = Cast<APEInteractableBase>(HitActor);
	if (!HitActor->IsA(APEEquipmentCache::StaticClass()) || HitActor == PC->CarriedInteractableActor)
	{
		return;		
	}

	APEEquipmentCache* EquipmentCache = Cast<APEEquipmentCache>(HitActor);
	if (!IsValid(EquipmentCache) || EquipmentCache->Team != Team)
	{
		return;
	}
	
	ServerDeployInteractable(InteractableActor);
}

void APEPlayerController::SubscribeToGameState(TSubclassOf<APEPlayerCharacter> PossessedCharacter)
{
	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (!GameState->PlayerControllerCharacterArray.Contains(this))
	{
		GameState->PlayerControllerCharacterArray.Add(this, PossessedCharacter);
	}
}

void APEPlayerController::ServerDeployInteractable_Implementation(APEInteractableBase* InActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Server Deploy: %s"), *InActor->GetName())
	InActor->Deploy();
}

void APEPlayerController::ServerDropInteractableActor_Implementation(APEPlayerController* Requester)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (PC->CarriedInteractableActor)
	{
		PC->CarriedInteractableActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UE_LOG(LogTemp, Warning, TEXT("Server drop Interactable Actor"));
	}

	if (PC->CarriedInteractableActor->IsA(APEInteractableBase::StaticClass()))
	{
		APEInteractableBase* InteractableActor = Cast<APEInteractableBase>(PC->CarriedInteractableActor);
		InteractableActor->MulticastSetSimulatePhysics(true);
		InteractableActor->RemoveCarryEffect();
		InteractableActor->Carrier = nullptr;
	}
	
	PC->CarriedInteractableActor = nullptr;
}
