// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEPlayerHUD.h"
#include "PEPlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EDamageDirection
{
	Front,
	Side,
	Back
};

inline FString EDamageDirection_ToString(EDamageDirection e)
{
	switch (e)
	{
		case EDamageDirection::Front: return "Front";
		case EDamageDirection::Side: return "Side";
		case EDamageDirection::Back: return "Back";
		default: throw std::invalid_argument("Unimplemented DamageDirection");
	}
}

/*
 * 
 */
UCLASS()
class PROJECTETHER_API APEPlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APEPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Root")
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Specs")
	UAttributeSet* AttributeSet;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	UDataTable* DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UGameplayAbility> PassiveAbility;

	UPROPERTY(BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle PassiveAbilityHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UGameplayAbility> WeaponAbility;

	UPROPERTY(BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle WeaponAbilityHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UGameplayAbility> AbilityOne;
	
	UPROPERTY(BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle AbilityOneHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UGameplayAbility> AbilityTwo;
	
	UPROPERTY(BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle AbilityTwoHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UGameplayAbility> AbilityThree;
	
	UPROPERTY(BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle AbilityThreeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UPEPlayerHUD> HUDClass;

	UPEPlayerHUD* PlayerHUD;
	
	EDamageDirection DetermineDamageDirection(FVector InVector) const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};