// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Treasure.h"
#include "Weapon.generated.h"

class USceneComponent;

/**
 * 
 */
UCLASS()
class MYLEARNINGECHO_API AWeapon : public ATreasure
{
	GENERATED_BODY()

public:

	void Equip(USceneComponent* sceneComponent, FName socketName);

protected:

	virtual void MySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void MySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
