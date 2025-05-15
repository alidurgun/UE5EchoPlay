// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Treasure.generated.h"

// Forward Decleration
class UStaticMeshComponent;
class USphereComponent;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ItemMesh;

	UFUNCTION()
	virtual void MySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void MySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
