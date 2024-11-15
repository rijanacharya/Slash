// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}



void ABaseCharacter::Die()
{
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if(HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetTransform());
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance	 = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return INDEX_NONE;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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

bool ABaseCharacter::isAlive()
{
	return Attributes && Attributes->IsAlive();
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




