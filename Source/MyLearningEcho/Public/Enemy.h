// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "CharacterTypes.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UCharacterAttributes;
class UWidget_HealthBarComponent;

UCLASS()
class MYLEARNINGECHO_API AEnemy : public ACharacter, public IHitInterface
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

	void GetHit_Implementation(const FVector &ImpactPoint) override;

	// animation montages for getting hit.
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* hitParticle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UCharacterAttributes* CharacterAttributes;

	UPROPERTY(EditAnywhere)
	UWidget_HealthBarComponent* WHealthBarComponent;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose deathPose{ EDeathPose::EDP_StillAlive };

	bool isAlive();

private:
	void PlayMontage(const FName& sectionName);
	const FName calculateHitLocation(const FVector& ImpactPoint);
	void Die();

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AActor* DamageCauserActor;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float AggroRange{ 500.f };
};
