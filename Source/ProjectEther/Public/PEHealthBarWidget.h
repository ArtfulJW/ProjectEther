// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PEHealthBarWidget.generated.h"

class UPEBaseCharacterAttributeSet;
class APEPlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Bar Specs")
	UTextBlock* TeamTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Bar Specs")
	UProgressBar* HealthBar;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health Bar Functions")
	void ServerUpdateHealthBar(UPEBaseCharacterAttributeSet* AttributeSet);
};