// Fill out your copyright notice in the Description page of Project Settings.

#include "PEWeapon.h"

#include "PEPlayerCharacter.h"

// Sets default values
APEWeapon::APEWeapon() :
WeaponSkeletalMeshComponent(nullptr),
WeaponSocket(FName("right_hand_socket_Jnt"))
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
}

// Called when the game starts or when spawned
void APEWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(MainWeaponAbilityContainerClass))
	{
		MainWeaponAbilityContainer = NewObject<UAbilityContainer>(this, MainWeaponAbilityContainerClass);
	}

	if (IsValid(SecondaryWeaponAbilityContainerClass))
	{
		SecondaryWeaponAbilityContainer = NewObject<UAbilityContainer>(this, SecondaryWeaponAbilityContainerClass);
	}
}

// Called every frame
void APEWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}