// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributes.h"

// Sets default values for this component's properties
UCharacterAttributes::UCharacterAttributes()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
}


// Called when the game starts
void UCharacterAttributes::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCharacterAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

