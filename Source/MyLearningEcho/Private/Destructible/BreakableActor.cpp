// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "..\..\Public\Treasure.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BreakableComponent = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Breakable Component"));

	BreakableComponent->SetGenerateOverlapEvents(true);
	BreakableComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BreakableComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BreakableComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SetRootComponent(BreakableComponent);

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collision"));
	capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	capsule->SetupAttachment(GetRootComponent());
	isBroken = false;
}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (isBroken) {
		return;
	}

	// Get the world.
	UWorld* world{ GetWorld() };

	// verify  our pointers are valid.
	if (world && TreasureClass) {
		// adjust location.
		FVector location{ GetActorLocation() };
		location.Z += 45.f;
		
		// to spawn actor.
		world->SpawnActor<ATreasure>(TreasureClass, location, FRotator::ZeroRotator);

		capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

		isBroken = true;
	}
}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

