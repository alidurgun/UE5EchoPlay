// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "BaseCharacter.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UCharacterAttributes;
class UWidget_HealthBarComponent;
class AAIController;
class UPawnSensingComponent;
class AWeapon;

UCLASS()
class MYLEARNINGECHO_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type newCollision);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;

private:
	//void PlayMontage(const FName& sectionName);
	//const FName calculateHitLocation(const FVector& ImpactPoint);
	void Die() override;
	void EnemyAttack();

	/**
	* AI Relavent functions.
	*/
	void ChoosePatrolTarget();
	void ChaseEnemy();
	bool InTargetRange(AActor* target, double radius);
	void MoveToTarget(AActor* target);
	void CheckPatrolTarget();
	void CheckCombatTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // Callback function for pawn sensing.
	/**
	* AI Relavent functions.
	*/

	/*
	* Timer related functions.
	*/
	UFUNCTION()
	void PatrolTimerFinished(); // Callback function for timer handle.

	UFUNCTION()
	void AttackTimerFinished();// Callback function for attack timer handle.

	void SpawnWeapon();

	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;
	/*
	* Timer related functions.
	*/

	/*
	* AI related properties.
	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* DamageCauserActor;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AggroRange{ 500.f };

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
	AActor* CurrentPatrolTarget;

	UPROPERTY()
	AActor* EnemyTarget;

	UPROPERTY(EditInstanceOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> PatrolTargets;

	UPROPERTY()
	AAIController* AiController;

	const double PatrolAcceptanceRadius{ 150.0f };
	const double AttackRadius{ 150.0f };
	const double CombatRadius{ 4000.0f };

	UPawnSensingComponent* SensingComponent;
	/*
	* AI related properties.
	*/

	UPROPERTY(BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	EEnemyActionState CurrentState{ EEnemyActionState::EEAS_Waiting };

	AWeapon* equippedWeapon;
};
