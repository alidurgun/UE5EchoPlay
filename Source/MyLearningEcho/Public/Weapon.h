// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Treasure.h"
#include "Weapon.generated.h"

class USceneComponent;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class MYLEARNINGECHO_API AWeapon : public ATreasure
{
	GENERATED_BODY()

public:
	AWeapon();

	void Equip(USceneComponent* sceneComponent, FName socketName);

	UFUNCTION(BlueprintCallable)
	void OnBoxOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE UBoxComponent* GetWeaponBox() { return BoxComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void MySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void MySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* WeaponCollisionStart;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* WeaponCollisionEnd;
};
