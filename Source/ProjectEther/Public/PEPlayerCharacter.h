// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "PEDamageDirectionComponent.h"
#include "PEBaseCharacterAttributeSet.h"
#include "PEEtherCompass.h"
#include "PEHealthBarWidget.h"
#include "PEBasePassiveAbilityActor.h"
#include "PEPlayerHUD.h"
#include "PEWeapon.h"
#include "Components/WidgetComponent.h"
#include "Engine/SpringInterpolator.h"
#include "PEPlayerCharacter.generated.h"

class APEPlayerState;

UENUM(BlueprintType)
enum class EDamageDirection : uint8
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
		default: return "Unimplemented DamageDirection";
	}
}

DECLARE_DELEGATE_OneParam(FOnEnemyHitDelegate, APEPlayerCharacter*);

DECLARE_DELEGATE(FOnTakeDamage);

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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mesh")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Root")
	USceneComponent* RootSceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Carry")
	USceneComponent* CarrySceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Specs")
	float fInteractDistance;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Character Specs")
	UPEBaseCharacterAttributeSet* AttributeSet;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category="Gameplay")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	UDataTable* DataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<APEBasePassiveAbilityActor> PassiveAbilityClass;

	UPROPERTY(Replicated)
	APEBasePassiveAbilityActor* PassiveAbilityActor;

	UPROPERTY(BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle PassiveAbilityHandle;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle WeaponAbilityOneHandle;

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle WeaponAbilityTwoHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UAbilityContainer> AbilityOneContainerClass;

	UPROPERTY()
	UAbilityContainer* AbilityOneContainer;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle AbilityOneHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UAbilityContainer> AbilityTwoContainerClass;

	UPROPERTY()
	UAbilityContainer* AbilityTwoContainer;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle AbilityTwoHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UAbilityContainer> AbilityThreeContainerClass;

	UPROPERTY()
	UAbilityContainer* AbilityThreeContainer;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category="Character Specs")
	FGameplayAbilitySpecHandle AbilityThreeHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<UGameplayEffect> RunningGameplayEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character Specs")
	TSubclassOf<APEWeapon> WeaponClass;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Character Specs")
	APEWeapon* WeaponActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character Specs")
	USkeletalMeshComponent* FPSArms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character Specs")
	APEWeapon* FirstPersonWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HUD")
	TSubclassOf<UPEPlayerHUD> HUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Compass")
	TSubclassOf<APEEtherCompass> EtherCompassClass;

	UPROPERTY(Replicated)
	APEEtherCompass* EtherCompassActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ether Compass")
	USceneComponent* EtherCompassSceneComponent;
	
	UPROPERTY()
	UPEPlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health Bar Widget Specs")
	TSubclassOf<UPEHealthBarWidget> HealthBarWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health Bar Widget Specs")
	UWidgetComponent* HealthBarWidgetComponent;
	
	UPROPERTY(Replicated)
	AActor* CarriedInteractableActor;
	
	bool bIsLookingAtInteractableActor;
	
	FOnEnemyHitDelegate OnEnemyHitDelegate;

	FOnTakeDamage OnTakeDamageDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_CopiedTeamTagText)
	FText CopiedTeamTagText;

	bool bTeamTagTextUpdated;

	FGameplayEffectContextHandle RunningEffectContext;

	FGameplayEffectSpecHandle RunningEffectSpecHandle;
	
	FActiveGameplayEffectHandle RunningEffectHandle;

	FGameplayEffectSpec* RunningEffectSpec;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage Direction Component Specs")
	TSubclassOf<UPEDamageDirectionComponent> DamageDirectionComponentClass;;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Damage Direction Component Specs")
	UWidgetComponent* DamageDirectionComponent;

	UPROPERTY(Replicated)
	bool bIsRunning;

	UPROPERTY(ReplicatedUsing=OnRep_DamageDirectionRotation, EditAnywhere, BlueprintReadWrite, Category = "Damage Direction Component Specs")
	FRotator DamageDirectionRotation;

	float fMaxHealthMaxValue;

	FRK4SpringInterpolator<FVector> ChainSpringInterpolator;
	bool bIsChained = false;
	FVector ChainedTargetLocation;
	
	UFUNCTION(Client, Unreliable)
	void ClientRemovePlayerHUD();
	
	EDamageDirection DetermineDamageDirection(const FHitResult& HitResult, FLinearColor InColor) const;

	void BeforeDestroy();

	void IsLookingAtInteractable();

	UFUNCTION(BlueprintCallable, Category = "Health Bar")
	void UpdateHealthBar();

	UFUNCTION(Server, Unreliable)
	void ServerCleanupPlayerCharacter();

	UFUNCTION()
	void UpdateTeamTagDisplay(const FText& InTag);

	UFUNCTION()
	void OnRep_CopiedTeamTagText();

	UFUNCTION(Server, Reliable)
	void ServerSetCopiedTeamTagText(APEPlayerState* InPlayerState);

	void TryUpdateTeamTagDisplay();

	UFUNCTION(Server, Reliable)
	void ServerRunEvent();
	
	UFUNCTION(Server, Reliable)
	void ServerStopRunEvent();

	UFUNCTION()
	void OnRep_DamageDirectionRotation() const;

	UFUNCTION(Server, Reliable)
	void ServerSetDamageDirectionImageRotation(const FRotator& InRotation);

	// float GetMaxHealthMaxValue();

	UFUNCTION(Server, Reliable)
	void ServerActivateHitIndicator(EDirectionDamageIndicator InDirection, FLinearColor InColor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAnimMontage(UAnimMontage* InAnimMontage);

	void PlayFPSArmsAnimMontage(UAnimMontage* InAnimMontage);

	UFUNCTION(Client, Reliable)
	void ClientCleanupClientWeapon();
	
	virtual void OnTakeDamage() PURE_VIRTUAL(APEPlayerCharacter::OnTakeAnyDamage);

	UFUNCTION()
	void AllowBasicAttack(const APEPlayerCharacter* TargetPlayerCharacter);

	void ChainPlayer(FVector OriginVector, float Duration, FRK4SpringInterpolator<FVector> InSpring);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};