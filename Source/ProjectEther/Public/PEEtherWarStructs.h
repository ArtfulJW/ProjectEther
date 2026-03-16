#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

UENUM(BlueprintType)
enum class ETeam : uint8
{
	TeamOne,
	TeamTwo,
	NoTeam
};

UENUM(BlueprintType)
enum class EDirectionDamageIndicator : uint8
{
	Front,
	Left,
	Right,
	Back
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

inline FSlateColor ETeam_GetTeamColor(ETeam e)
{
	switch (e)
	{
	case ETeam::TeamOne: return FSlateColor(FLinearColor(1.0f, 0.2f, 0.0f, 1.0f));
	case ETeam::TeamTwo: return FSlateColor(FLinearColor(0.0f, 0.5f, 0.8f, 1.0f));
	default: return FSlateColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f));
	}
}

inline ETeam ETeam_GetTeamFromString(const FText& InText)
{
	if (InText.ToString() == "Team One")
	{
		return ETeam::TeamOne;
	}
	else if (InText.ToString() == "Team Two")
	{
		return ETeam::TeamTwo;
	}

	return ETeam::NoTeam;
}

UENUM(BlueprintType)
enum class EClassType : uint8
{
	Base,
	Berserker,
	Mage,
	Priest
};