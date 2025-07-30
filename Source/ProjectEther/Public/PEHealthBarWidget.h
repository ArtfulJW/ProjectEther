// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "PEHealthBarWidget.generated.h"

class APEPlayerCharacter;
/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Bar Specs")
	APEPlayerCharacter* PlayerCharacterRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Bar Specs", meta = (BindWidget))
	UProgressBar* HealthBar;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Health Bar")
	void ServerUpdateHealthBar(float fPercentage);
};