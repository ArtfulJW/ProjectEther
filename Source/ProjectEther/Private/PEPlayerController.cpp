// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"

#include "EnhancedInputComponent.h"
#include "InputState.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEEther.h"
#include "PEPlayerCharacter.h"
#include "GameFramework/SpectatorPawn.h"
#include "Kismet/GameplayStatics.h"

class APEPlayerCharacter;

void APEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetNetMode() < NM_Client)
	{
		return;
	}
	
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
	
	APEEther* Ether = Cast<APEEther>(InActor);
	if (!Ether->IsA(APEEther::StaticClass()))
	{
		return;
	}
	
	Ether->Carrier = PC;
	Ether->ApplyCarryEffect();
	PC->CarriedInteractableActor = InActor;
	// InActor->AttachToComponent(PC->CarrySceneComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);
	Ether->Interact();
	UE_LOG(LogTemp, Warning, TEXT("Server Interacting with Ether"));
}

void APEPlayerController::InteractEvent()
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(GetPawn());
	if (!IsValid(PC))
	{
		return;
	}
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PC->CameraComponent->GetComponentLocation(), PC->CameraComponent->GetForwardVector() * 10000, ECC_Visibility);
	AActor* Actor = Hit.GetActor();
	
	if (UKismetSystemLibrary::DoesImplementInterface(Actor, UInteractableInterface::StaticClass()))
	{
		ServerInteractEvent(this, Actor);
	}
}
