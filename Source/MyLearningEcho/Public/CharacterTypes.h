#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_OneHandedWeaponEquipped UMETA(DisplayName = "Equipped One-Handed Weapon"),
	ECS_TwoHandedWeaponEquipped UMETA(DisplayName = "Equipped Two-Handed Weapon")
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8 {
	ECAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECAS_Attacking UMETA(DisplayName = "Attacking")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8 {
	EDP_StillAlive UMETA(DisplayName = "Still Alive"),
	EDP_DeathPose1 UMETA(DisplayName = "Death Pose 1"),
	EDP_DeathPose2 UMETA(DisplayName = "Death Pose 2"),
	EDP_DeathPose3 UMETA(DisplayName = "Death Pose 3"),
	EDP_DeathPose4 UMETA(DisplayName = "Death Pose 4")
};

UENUM(BlueprintType)
enum class EEnemyActionState : uint8 {
	EEAS_Waiting UMETA(DisplayName = "Waiting"),
	EEAS_Patrolling UMETA(DisplayName = "Patrolling"),
	EEAS_Chasing UMETA(DisplayName = "Chasing"),
	EEAS_Attacking UMETA(DisplayName = "Attacking"),
	EEAS_Engaging UMETA(DisplayName = "Engaging"),
	EEAS_Dead UMETA(DisplayName = "Dead")
};