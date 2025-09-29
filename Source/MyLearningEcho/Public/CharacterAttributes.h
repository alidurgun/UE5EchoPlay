// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterAttributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYLEARNINGECHO_API UCharacterAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterAttributes();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetMaxHealth(float MaxHealth_) { this->MaxHealth = MaxHealth_; }
	FORCEINLINE void SetCurrentHealth(float CurrentHealth_) { this->CurrentHealth = CurrentHealth_; }
	FORCEINLINE float GetCurrentHealthPercent() { return this->CurrentHealth / this->MaxHealth; }
	FORCEINLINE void DamageCurrentHealth(float Damage) { this->CurrentHealth = FMath::Clamp(this->CurrentHealth - Damage, 0.0f, this->MaxHealth); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float CurrentHealth;

	UPROPERTY(EditAnywhere)
	float MaxHealth;
	
};
