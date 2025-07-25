// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Treasure.generated.h"

// Forward Decleration
class UStaticMeshComponent;
class USphereComponent;
class UNiagaraComponent;

UCLASS()
class MYLEARNINGECHO_API ATreasure : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	float RunningTime{ 0.f };

	UPROPERTY(VisibleAnywhere)
	USphereComponent* sphere;
public:	
	// Sets default values for this actor's properties
	ATreasure();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UNiagaraComponent* GetEmberEffect() { return this->EmbersEffect; }

protected:
	// For hovering
	//enum class EItemState : uint8
	//{
	//	EIS_Unequipped,
	//	EIS_Equipped
	//};

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* EmbersEffect;

	UFUNCTION()
	virtual void MySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void MySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//EItemState ItemState{ EItemState::EIS_Unequipped };
};
