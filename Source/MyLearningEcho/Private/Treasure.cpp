// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
ATreasure::ATreasure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	sphere->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ATreasure::BeginPlay()
{
	Super::BeginPlay();
	
	sphere->OnComponentBeginOverlap.AddDynamic(this, &ATreasure::MySphereOverlap);
	sphere->OnComponentEndOverlap.AddDynamic(this, &ATreasure::MySphereEndOverlap);
}

void ATreasure::MySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString actorName{ "Overlap started for " + OtherActor->GetName()};
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Blue, actorName);
	}
}

void ATreasure::MySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString actorName{ "Overlap ended for " + OtherActor->GetName() };
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Red, actorName);
	}
}

// Called every frame
void ATreasure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// To make it hanging in the air.
	RunningTime += DeltaTime;
	float newSin = (FMath::Sin(RunningTime * 5.f));

	// We will add this to offset as a Z value because we want this move
	// in Z direction.
	AddActorWorldOffset(FVector(0.f, 0.f, newSin));
}

