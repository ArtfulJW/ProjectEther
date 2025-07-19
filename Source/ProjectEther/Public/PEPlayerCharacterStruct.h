#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PEPlayerCharacterStruct.generated.h"

USTRUCT()
struct FPEPlayerCharacterStruct : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Specs")
    float Speed;
};