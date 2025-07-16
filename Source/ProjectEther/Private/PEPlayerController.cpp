// Fill out your copyright notice in the Description page of Project Settings.

#include "PEPlayerController.h"

#include "EnhancedInputComponent.h"
#include "PEPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

class APEPlayerCharacter;

void APEPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	
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
}

void APEPlayerController::MoveEvent(const FInputActionValue& Value)
{
	FVector Direction = Value.Get<FVector>();
	
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid World"));
	}
	
	APEPlayerCharacter* PC = Cast<APEPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	FTransform PlayerTransform = PC->GetTransform();
	PC->GetTransform().GetLocation().AddBounded(Direction);
	
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("FUCK: %s"), *Direction.ToString()));
}