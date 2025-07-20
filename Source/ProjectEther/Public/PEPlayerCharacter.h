// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEPlayerCharacter.generated.h"

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
	TSubclassOf<UGameplayAbility> AbilityOne;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle AbilityOneHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UGameplayAbility> AbilityTwo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle AbilityTwoHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UGameplayAbility> AbilityThree;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle AbilityThreeHandle;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};