// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "CharacterTypes.h"
#include "BaseCharacter.generated.h"

class UCharacterAttributes;
class UParticleSystem;
class UWidget_HealthBarComponent;

UCLASS()
class MYLEARNINGECHO_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetHit_Implementation(const FVector& ImpactPoint) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* hitParticle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> DeathNames;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<EDeathPose> DeathPosesArray;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackNames;

	UPROPERTY()
	EDeathPose DeathPose;

	/**
		Following parts defined for relevant animation montages.
	*/
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* AttackMontage;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// calculating the hit is coming from which direction.
	const FName calculateHitLocation(const FVector& ImpactPoint);

	bool isAlive();

	virtual void Die();

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UCharacterAttributes* charAttributes;

	UPROPERTY(EditAnywhere)
	UWidget_HealthBarComponent* WHealthBarComponent;

	void PlayMontage(const FName& sectionName, UAnimMontage* montageName);

private:
	
};
