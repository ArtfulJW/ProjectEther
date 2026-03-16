// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityContainer.h"
#include "GameFramework/Actor.h"
#include "PEWeapon.generated.h"

class UGameplayAbility;

UCLASS()
class PROJECTETHER_API APEWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APEWeapon();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	USkeletalMeshComponent* WeaponSkeletalMeshComponent;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	// UAnimMontage* BasicAttackMontage;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	// UAnimMontage* FPSBasicAttackMontage;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	// UAnimMontage* SecondaryAttackMontage;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	// UAnimMontage* FPSSecondaryAttackMontage;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	// TSubclassOf<UGameplayAbility> WeaponAbilityOne;
	//
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	// TSubclassOf<UGameplayAbility> WeaponAbilityTwo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	TSubclassOf<UAbilityContainer> MainWeaponAbilityContainerClass;

	UPROPERTY()
	UAbilityContainer* MainWeaponAbilityContainer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	TSubclassOf<UAbilityContainer> SecondaryWeaponAbilityContainerClass;

	UPROPERTY()
	UAbilityContainer* SecondaryWeaponAbilityContainer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon Specs")
	FName WeaponSocket;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};