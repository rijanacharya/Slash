// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;



UCLASS()
class SLASH_API ABaseCharacter : public ACharacter ,public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnabled);


protected:
	virtual void BeginPlay() override;
//	virtual void Attack(const  FInputActionValue& Value);
	virtual void Die();


	/**
	 * Play Montage Function
	 * */
	virtual void PlayAttackMontage();
	void PlayHitReactMontage(const FName& SectionName);


	virtual bool CanAttack();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UPROPERTY(VisibleAnywhere, Category=Weapon)
	AWeapon* EquippedWeapon;
	/**
	 * Animation Montage
	 */
	UPROPERTY(EditDefaultsOnly, Category = Monatages)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Monatages)
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = Monatages)
	UAnimMontage* DeathMontage;


	virtual  void DirectionalHitReact(const FVector& ImpactPoint);

	/**
	* Components
	*/

	UPROPERTY(VisibleAnywhere, Category = Attributes)
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisalEffect)
	UParticleSystem* HitParticles;




};
