#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_OneHandedWeaponEquipped UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_TwoHandedWeaponEquipped UMETA(DisplayName = "Equipped Two-Handed Weapon")
};