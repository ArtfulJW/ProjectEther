// Fill out your copyright notice in the Description page of Project Settings.

#include "PEDamageDirectionComponent.h"
#include "PEPlayerCharacter.h"

FRotator UPEDamageDirectionComponent::UpdateDamageDirectionImage()
{
	if (!IsValid(DamageDirectionImage))
	{
		return FRotator::ZeroRotator;
	}

	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(GetOwningLocalPlayer()->GetPlayerController(GetWorld())->GetPawn());
	if (!IsValid(PlayerCharacter))
	{
		return FRotator::ZeroRotator;
	}
	
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerCharacter->DamageDirectionComponent->GetComponentLocation(), PlayerCharacter->DamageDirectionComponent->GetComponentLocation() + PlayerCharacter->GetActorUpVector() * -1 * 50, ECC_Visibility);

	if (!IsValid(Hit.GetActor()) || !IsValid(Hit.GetComponent()))
	{
		// PlayerCharacter->DamageDirectionComponent->SetWorldRotation(FRotator(90, PlayerCharacter->GetActorRotation().Yaw, 0));
		// DamageDirectionRotation = FRotator(90, PlayerCharacter->GetActorRotation().Yaw, 0);
		return FRotator(90, PlayerCharacter->GetActorRotation().Yaw, 0);
	}
	
	FVector ForwardVector = PlayerCharacter->GetActorForwardVector();
	FVector PlaneProjectedForwardVector = FVector::VectorPlaneProject(ForwardVector, Hit.Normal);
	
	FVector RightVector = FVector::CrossProduct(Hit.Normal.GetSafeNormal(), PlaneProjectedForwardVector);
	FVector CorrectedForward = FVector::CrossProduct(RightVector, Hit.Normal.GetSafeNormal()) * -1;
	FRotator FinalRot = FRotationMatrix::MakeFromZX(CorrectedForward, Hit.Normal.GetSafeNormal()).Rotator();

	return FinalRot;
	
	// DamageDirectionRotation = FinalRot;
	// PlayerCharacter->DamageDirectionComponent->SetWorldRotation(FinalRot);
	
	// DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + Hit.Normal * 50.0f, FColor::Green, false, -1.0f, 0, 2);
	// DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + ForwardVector * 50.0f, FColor::Cyan, false, -1.0f, 0, 2);
	// DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + PlaneProjectedForwardVector * 50.0f, FColor::Blue, false, -1.0f, 0, 2);
	// DrawDebugLine(GetWorld(), Hit.ImpactPoint, Hit.ImpactPoint + RightVector * 50.0f, FColor::Magenta, false, -1.0f, 0, 2);
}

void UPEDamageDirectionComponent::SetOwningWidgetComponent(UWidgetComponent* InWidgetComponent)
{
	OwningWidgetComponent = InWidgetComponent;
}

UWidgetComponent* UPEDamageDirectionComponent::GetOwningWidgetComponent() const
{
	return OwningWidgetComponent;
}

// void UPEDamageDirectionComponent::OnRep_DamageDirectionRotation() const
// {
// 	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(GetOwningLocalPlayer()->GetPlayerController(GetWorld())->GetPawn());
// 	if (!IsValid(PlayerCharacter))
// 	{
// 		return;
// 	}
//
// 	PlayerCharacter->DamageDirectionComponent->SetWorldRotation(DamageDirectionRotation);
// }

void UPEDamageDirectionComponent::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	FRotator OutRot = UpdateDamageDirectionImage();
	APEPlayerCharacter* PlayerCharacter = Cast<APEPlayerCharacter>(GetOwningPlayerPawn());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}
	PlayerCharacter->ServerSetDamageDirectionImageRotation(OutRot);
}