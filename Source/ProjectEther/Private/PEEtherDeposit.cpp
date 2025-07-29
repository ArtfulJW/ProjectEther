// Fill out your copyright notice in the Description page of Project Settings.

#include "PEEtherDeposit.h"
#include "PEEther.h"
#include "PEGameMode.h"
#include "PEGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
APEEtherDeposit::APEEtherDeposit():
NumDepositedEther(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	EtherDepositOne = CreateDefaultSubobject<USceneComponent>(TEXT("Ether Deposit One"));
	EtherDepositTwo = CreateDefaultSubobject<USceneComponent>(TEXT("Ether Deposit Two"));
	EtherDepositThree = CreateDefaultSubobject<USceneComponent>(TEXT("Ether Deposit Three"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	StaticMeshComponent =  CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));

	RootComponent = StaticMeshComponent;
	
	EtherDepositOne->SetupAttachment(RootComponent);
	EtherDepositTwo->SetupAttachment(RootComponent);
	EtherDepositThree->SetupAttachment(RootComponent);
	BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &APEEtherDeposit::DepositEther);
}

void APEEtherDeposit::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APEEtherDeposit, NumDepositedEther)
}

// Called when the game starts or when spawned
void APEEtherDeposit::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(EtherDepositOne))
	{
		EtherDepositSceneComponents.Add(EtherDepositOne);
	}
	if (IsValid(EtherDepositTwo))
	{
		EtherDepositSceneComponents.Add(EtherDepositTwo);
	}
	if (IsValid(EtherDepositThree))
	{
		EtherDepositSceneComponents.Add(EtherDepositThree);
	}
}

// Called every frame
void APEEtherDeposit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APEEtherDeposit::Destroyed()
{
	Super::Destroyed();
	
	BoxComponent->OnComponentBeginOverlap.RemoveDynamic(this, &APEEtherDeposit::DepositEther);
}

void APEEtherDeposit::DepositEther(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (NumDepositedEther >= 3)
	{
		return;
	}
	
	if (!IsValid(Other))
	{
		return;
	}

	if (!Other->IsA(APEEther::StaticClass()))
	{
		return;
	}
	
	APEEther* Ether = Cast<APEEther>(Other);
	if (!IsValid(Ether->Carrier))
	{
		return;
	}
	
	APEPlayerCharacter* PlayerCharacter = Ether->Carrier;
	APEPlayerController* PlayerController = Cast<APEPlayerController>(PlayerCharacter->GetController());
	if (Team != PlayerController->Team)
	{
		return;
	}
	
	Ether->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	USceneComponent* TargetSceneComponent = Cast<USceneComponent>(EtherDepositSceneComponents[NumDepositedEther]);
	Ether->AttachToComponent(TargetSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Ether->ServerSetIsDeposited(true);
	Ether->RemoveCarryEffect();
	
	PlayerCharacter->CarriedInteractableActor = nullptr;
	Ether->Carrier = nullptr;

	APEGameState* GameState = Cast<APEGameState>(UGameplayStatics::GetGameState(GetWorld()));
	GameState->ServerClearEther();
	GameState->ServerSpawnEther();
	GameState->SpawnEquipmentCache(PlayerController->Team);

	NumDepositedEther++;
	ServerCheckEndGame();
}

void APEEtherDeposit::ServerCheckEndGame_Implementation()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APEGameMode* GameMode = Cast<APEGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (NumDepositedEther >= GameMode->GetNumEtherToWin())
	{
		UE_LOG(LogTemp, Warning, TEXT("Game is ready to end"));
		World->ServerTravel(FString("/Script/Engine.World'/Game/Maps/MainMenuMap.MainMenuMap"));
	}
}