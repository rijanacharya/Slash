
#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"


class UPawnSensingComponent;
class AAIController;
class UHealthBarComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	void CheckCombatTarget();
	void CheckPatrolTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	bool InTargetRange(AActor* Target,  double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	
	void PlayDeathMontage();
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	

	

private:
	/**
	 * Components
	 */



	UPROPERTY(VisibleAnywhere, Category = Attributes)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	/**
	 * @brief  Montage
	 */
	
	


	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;
	
	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	/**
	 *Navigation
	 */
	
	UPROPERTY()
	AAIController* EnemyController;
	UPROPERTY(EditInstanceOnly,Category="AI Navigation" )
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<AActor*> PatrolTargets;
	
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrollingSpeed = 125.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();


	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

public:	
	

};
