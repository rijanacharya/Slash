
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
	/** <AActor>*/
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/** <AActor>*/
	
	/** <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint , AActor* Hitter) override;
	/** <IHitInterface> */



	
protected:

    /** <AActor>*/
    virtual void BeginPlay() override;
    /** <AActor>*/

    /* <ABaseCharacter>*/
	virtual void Die() override;
	virtual bool CanAttack() override;
	virtual void AttackEnd() override;
	virtual void HandleDamage(float DamageAmount) override;
	/* <ABaseCharacter>*/

	
	
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;


private:
	// AI Behaviour
	void InitializeEnemy();
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LooseInterest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target,  double Radius);
	void MoveToTarget(AActor* Target);
	void Attack();
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();
	
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // CallBack for OnPawnSeen In UPawnSensing Component
	
	UPROPERTY(VisibleAnywhere, Category = Attributes)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	

	UPROPERTY(EditAnywhere)
	double CombatRadius = 1000.f;
	
	UPROPERTY(EditAnywhere)
	double AttackRadius = 140.f;
	
	UPROPERTY()
	AAIController* EnemyController;
	UPROPERTY(EditInstanceOnly,Category="AI Navigation" )
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<AActor*> PatrolTargets;
	
	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float PatrollingSpeed = 125.f;	
	
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMin = 0.2f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float AttackMax = 0.6f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float ChasingSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan =6.f;


	



};
