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

	float fClosestNegativeDistance = FMath::Max(VectorFromEdge.X , FMath::Max(VectorFromEdge.Y, VectorFromEdge.Z));
	float fPositiveLengthFromEdge = FVector(FMath::Max(VectorFromEdge.X, 0.0f), FMath::Max(VectorFromEdge.Y, 0.0f), FMath::Max(VectorFromEdge.Z, 0.0f)).Length();
	
	return fPositiveLengthFromEdge + FMath::Min(fClosestNegativeDistance, 0.0f);
}
