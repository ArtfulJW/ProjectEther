// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECTETHER_API PEMathLibrary
{
public:
	PEMathLibrary();
	~PEMathLibrary();

	/**
	 * Credit to Inigo Quilez for the formula. Found on:
	 * \n https://iquilezles.org/articles/distfunctions/
	 */
	static float SignedDistanceFieldBox(const FVector& Point, const FVector& Box);
};