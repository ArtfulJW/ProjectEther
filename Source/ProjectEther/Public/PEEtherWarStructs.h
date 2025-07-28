#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

UENUM(Blueprintable)
enum ETeam
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
	default: throw std::invalid_argument("Unimplemented DamageDirection");
	}
}