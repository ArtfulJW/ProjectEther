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

float PEMathLibrary::EuclideanDistance(const FVector& VectorOne, const FVector& VectorTwo)
{
	return (VectorOne - VectorTwo).Size();
}

FVector PEMathLibrary::MidpointVector(const FVector& VectorOne, const FVector& VectorTwo)
{
	return FVector((VectorOne.X + VectorTwo.X)/2, (VectorOne.Y + VectorTwo.Y)/2, (VectorOne.Z + VectorTwo.Z)/2);
}
