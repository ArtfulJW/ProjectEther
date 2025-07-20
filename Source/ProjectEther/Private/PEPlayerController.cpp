// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"

#include "EnhancedInputComponent.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEPlayerCharacter.h"
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
}

void APEPlayerController::MoveEvent(const FInputActionValue& Value)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
	}
	
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerCharacter"));
	}
	
	FVector Direction = Value.Get<FVector>();	
	ServerMovePlayer(this, Direction * PC->AbilitySystemComponent->GetSet<UPEBaseCharacterAttributeSet>()->GetSpeed());
}

void APEPlayerController::ServerMovePlayer_Implementation(APEPlayerController* Requester, FVector InVector)
{
	Cast<APEPlayerCharacter>(Requester->GetPawn())->AddActorLocalTransform(FTransform(InVector));
}

void APEPlayerController::LookEvent(const FInputActionValue& Value)
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
	}
	
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid PlayerCharacter"));
	}

	FVector InVector = Value.Get<FVector>();
	
	PC->AddControllerPitchInput(InVector.Y);
	PC->AddControllerYawInput(InVector.X);
}

void APEPlayerController::UseAbilityEvent(const FInputActionValue& Value)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
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

void APEPlayerController::ServerUseAbilityOneEvent_Implementation(APEPlayerController* Requester)
{
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(Requester->GetPawn());
	PC->AbilitySystemComponent->TryActivateAbility(PC->AbilityOneHandle);
}

void APEPlayerController::ServerUseAbilityTwoEvent_Implementation(APEPlayerController* Requester)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Executed Ability (2)"), *Requester->GetName())
}

void APEPlayerController::ServerUseAbilityThreeEvent_Implementation(APEPlayerController* Requester)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: Executed Ability (3)"), *Requester->GetName())
}