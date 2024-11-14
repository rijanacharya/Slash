
#include "Enemy/Enemy.h"
#include "Components/AttributeComponent.h"
#include"Components/SkeletalMeshComponent.h"
#include"Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Characters/SlashCharacter.h"
#include "Navigation/PathFollowingComponent.h"

#include "Slash/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetAllBodiesCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);	
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);


	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius =5000.f;
	PawnSensing->SetPeripheralVisionAngle(50.f);
	


}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarWidget && Attributes)
	{
		HealthBarWidget->SetVisibility(false);
	}
	EnemyController =  Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
	
}



void AEnemy::Die()
{
	PlayDeathMontage();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorEnableCollision(false);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
	SetLifeSpan(10.f);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget =  (Target->GetActorLocation()-GetActorLocation()).Size();

	return DistanceToTarget <= Radius;

}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest  MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyController->MoveTo(MoveRequest);
	
}

AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTarget;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTarget.Add(Target);
		}
	}
	if(ValidTarget.Num() > 0)
	{
		return ValidTarget[FMath::RandRange(0, ValidTarget.Num() - 1)];
	}
	return nullptr;
}


void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState ==EEnemyState::EES_Chasing) return;
	if (SeenPawn->ActorHasTag(FName("SlashCharacter")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
		CombatTarget = SeenPawn;
		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}
		


	}
		
}

void AEnemy::PlayDeathMontage()
{
	
	
	if (UAnimInstance* AnimInstance	 = GetMesh()->GetAnimInstance(); AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
		const int32 Selection = FMath::RandRange(0, 5);
		FName SectionName= FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			SectionName = FName("Death3");
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			SectionName = FName("Death4");
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			SectionName = FName("Death5");
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 5:
			SectionName = FName("Death6");
			DeathPose = EDeathPose::EDP_Death6;
			break;
		default:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_Death1;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
	
	
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		//outside combat radius loose interest
		CombatTarget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
		MoveToTarget(PatrolTarget);

	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		//outside attack range chase character
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
		MoveToTarget(CombatTarget);
	}
	else if (InTargetRange(CombatTarget , AttackRadius) && EnemyState  != EEnemyState::EES_Attacking)
	{
		//within attack range attack
		EnemyState = EEnemyState::EES_Attacking;
		//GetCharacterMovement()->MaxWalkSpeed = 0.f;
		//PlayAttackMontage();
	}
}

void AEnemy::CheckPatrolTarget()
{
	if(InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);

	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	
	CheckPatrolTarget();

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
		if (Attributes && Attributes->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if(HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetTransform());
	}
	
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
	AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
	CombatTarget = EventInstigator->GetPawn();
	GetWorldTimerManager().ClearTimer(PatrolTimer);
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
	return DamageAmount;
}


