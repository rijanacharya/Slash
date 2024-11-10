
#include "Enemy/Enemy.h"
#include"Components/SkeletalMeshComponent.h"
#include"Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Slash/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetAllBodiesCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);	
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);



}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);

	DirectionalHitReact(ImpactPoint);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if(HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetTransform());
	}
	
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
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


	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Angle: %f"), Angle));
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation(), GetActorLocation() + Forward * 100.0f, 5.0f, FColor::Green, 10.0f, 1.0f);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ToHit * 100.0f, 5.0f, FColor::Red, 10.0f, 1.0f);
		UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation(), GetActorLocation() + CrossProduct * 100.0f, 5.0f, FColor::Blue, 10.0f, 1.0f);
	}*/
}

