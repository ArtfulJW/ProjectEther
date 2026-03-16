// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Components/SceneComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "PEDamageDirectionComponent.generated.h"


UCLASS()
class PROJECTETHER_API UPEDamageDirectionComponent : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Direction Component Specs")
	UImage* DamageDirectionImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Direction Component Specs")
	UWidgetComponent* OwningWidgetComponent;

	// UPROPERTY(ReplicatedUsing=OnRep_DamageDirectionRotation, EditAnywhere, BlueprintReadWrite, Category = "Damage Direction Component Specs")
	// FRotator DamageDirectionRotation = FRotator(0,0,0);

	FRotator UpdateDamageDirectionImage();

	void SetOwningWidgetComponent(UWidgetComponent* InWidgetComponent);

	UWidgetComponent* GetOwningWidgetComponent() const;

	// UFUNCTION()
	// void OnRep_DamageDirectionRotation() const;
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};