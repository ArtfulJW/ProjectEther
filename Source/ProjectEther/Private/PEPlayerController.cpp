// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"

#include "EnhancedInputComponent.h"
#include "InputState.h"
#include "PEBaseCharacterAttributeSet.h"
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
	
	if (PlayerInput->IsPressed(EKeys::Two))
	{
		ServerUseAbilityOneEvent(this);
	}
	
	if (PlayerInput->IsPressed(EKeys::Three))
	{
		ServerUseAbilityTwoEvent(this);
	}
	
	if (PlayerInput->IsPressed(EKeys::Four))
	{
		ServerUseAbilityThreeEvent(this);
	}
}

void APEPlayerController::UseWeaponEvent(const FInputActionValue& Value)
{
	if (IsPossessingSpectatorPawn(this))
	{
		return;
	}
	
	if (PlayerInput->IsPressed(EKeys::LeftMouseButton))
	{
		ServerUseWeaponOneAbilityEvent(this);
	}
	if (PlayerInput->IsPressed(EKeys::RightMouseButton))
	{
		ServerUseWeaponTwoAbilityEvent(this);
	}
}

bool APEPlayerController::IsPossessingSpectatorPawn(APEPlayerController* Requester)
{
	return  Requester->GetPawn()->IsA(ASpectatorPawn::StaticClass());
}

void APEPlayerController::ServerUseWeaponTwoAbilityEvent_Implementation(APEPlayerController* Requester)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	PC->AbilitySystemComponent->TryActivateAbility(PC->WeaponAbilityTwoHandle);
}

void APEPlayerController::ServerUseWeaponOneAbilityEvent_Implementation(APEPlayerController* Requester)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	PC->AbilitySystemComponent->TryActivateAbility(PC->WeaponAbilityOneHandle);
}

void APEPlayerController::ServerUseAbilityOneEvent_Implementation(APEPlayerController* Requester)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	PC->AbilitySystemComponent->TryActivateAbility(PC->AbilityOneHandle);
}

void APEPlayerController::ServerUseAbilityTwoEvent_Implementation(APEPlayerController* Requester)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	PC->AbilitySystemComponent->TryActivateAbility(PC->AbilityTwoHandle);
}

void APEPlayerController::ServerUseAbilityThreeEvent_Implementation(APEPlayerController* Requester)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	PC->AbilitySystemComponent->TryActivateAbility(PC->AbilityThreeHandle);
}