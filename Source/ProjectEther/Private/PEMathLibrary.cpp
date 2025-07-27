// Fill out your copyright notice in the Description page of Project Settings.

#include "PEMathLibrary.h"

PEMathLibrary::PEMathLibrary()
{
}

PEMathLibrary::~PEMathLibrary()
{
}

float PEMathLibrary::SignedDistanceFieldBox(const FVector& Point, const FVector& Box)
{
	FVector VectorFromEdge = Point.GetAbs() - Box;
	return FMath::Max(VectorFromEdge.Length(), 0.0f) + FMath::Min(FMath::Max(VectorFromEdge.X , FMath::Max(VectorFromEdge.Y, VectorFromEdge.Z)), 0.0f);
}
