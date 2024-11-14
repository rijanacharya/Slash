// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Items/Weapons/Weapon.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

/*void ABaseCharacter::Attack(const FInputActionValue& Value)
{
}*/

void ABaseCharacter::Die()
{
}

void ABaseCharacter::PlayAttackMontage()
{
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const  FVector Forward = GetActorForwardVector();
	const  FVector ImpactLower(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLower - GetActorLocation()).GetSafeNormal();
	//Forward * ToHit = |forward| |ToHit| * cos(theta)
	// both are unit vector so magnitude is 1 , so forward * ToHit = cos(theta)
	// Acos is inverse cosine or also known as arc-cosine
	// default angle is in Radians so we are converting it to degree 
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, ToHit)));

	// if cross product face down then angle should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0.0f)
	{
		Angle *= -1.0f;
	}

	FName Section("FromBack");
	//if angle is between -45 and 45 then it is from front
	if (Angle >= -45 && Angle <= 45)
	{
		Section = FName("FromFront");
	}
	else if (Angle >= -135 && Angle <= -45)
	{
		Section = FName("FromLeft");
	}
	else if (Angle >= 45 && Angle <= 135)
	{
		Section = FName("FromRight");
	}
	PlayHitReactMontage(Section);

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->IgnoreActors.Empty();
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
	}
}




