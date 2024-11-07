// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class AWeapon;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UGroomComponent;
class AItem;
class UAnimMontage;


UCLASS()
class SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Input")
	UInputMappingContext* SlashMappingContext;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input")
	UInputAction* EKeyPressed;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input")
	UInputAction* DodgeAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input")
	UInputAction* AttackAction;

	/**
	 * 
	 * Callbacks for Input
	 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Equip(const  FInputActionValue& Value);
	void Dodge(const  FInputActionValue& Value);
	void Attack(const  FInputActionValue& Value);

	/**
	 * Play Montage Function
	 * */
	void PlayEquipMontage();
	void PlayDodgeMontage();
	void PlayAttackMontage();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	bool  CanAttack();
	void PlayEquipMontage(FName SectionName);
	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();
	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	
private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite,meta=(AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere,Category= Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere,Category= Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/**
	 * Animation montages
	 **/
	
	UPROPERTY(EditDefaultsOnly, Category = Monatages)
	UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = Monatages)
	UAnimMontage* DodgeMontage;
	UPROPERTY(EditDefaultsOnly, Category = Monatages)
	UAnimMontage* AttackMontage;

	/**
	 * 
	 */
	// ReSharper disable once CppUE4ProbableMemoryIssuesWithUObject
	AWeapon* EquippedWeapon;
	

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }

	FORCEINLINE ECharacterState GetCharacterState () const {return CharacterState; }
	

};
