#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	TeamOne,
	TeamTwo
};

inline FString ETeam_ToString(ETeam e)
{
	switch (e)
	{
	case ETeam::TeamOne: return "Team One";
	case ETeam::TeamTwo: return "Team Two";
	default: return "Unimplemented DamageDirection";
	}
}

UENUM(BlueprintType)
enum class EClassType : uint8
{
	Base,
	Berserker,
	Mage,
	Priest
};