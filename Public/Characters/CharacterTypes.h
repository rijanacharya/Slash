#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Hand Weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Hand Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon")
	/*EAS_Walking UMETA(DisplayName = "Walking"),
	EAS_Running UMETA(DisplayName = "Running"),
	EAS_Jumping UMETA(DisplayName = "Jumping"),
	EAS_Falling UMETA(DisplayName = "Falling"),
	EAS_Defending UMETA(DisplayName = "Defending"),
	EAS_Dead UMETA(DisplayName = "Dead")*/
};
