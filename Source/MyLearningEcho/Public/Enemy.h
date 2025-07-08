// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetHit(const FVector &ImpactPoint) override;

	// animation montages for getting hit.
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* hitParticle;

private:
	void PlayMontage(const FName& sectionName);
	const FName calculateHitLocation(const FVector& ImpactPoint);
};
