// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UGroomComponent;
class AItem;
class UAnimMontage;


UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
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
	UInputAction* EKeyPressedAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input")
	UInputAction* DodgeAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Input")
	UInputAction* AttackAction;

	/** Callbacks for Input	 */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void EKeyPressed(const  FInputActionValue& Value);
	void Dodge(const  FInputActionValue& Value);
	void Attack(const  FInputActionValue& Value) ;
	/** Combat */
	void EquipWeapon(AWeapon* Weapon);
	virtual void AttackEnd() override;
	virtual bool  CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();
	
	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();
	
	void PlayEquipMontage(const FName& SectionName);

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	
private:
	/** Character Components	 **/
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
	
	UPROPERTY(EditDefaultsOnly, Category = Monatages)
	UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = Monatages)
	UAnimMontage* DodgeMontage;
	
	
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite,meta=(AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState () const {return CharacterState; }
	

};
