// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "PEEtherDepositHomeIconWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTETHER_API UPEEtherDepositHomeIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Home Icon Specs")
	UBorder* HomeIconBorder = nullptr;
};