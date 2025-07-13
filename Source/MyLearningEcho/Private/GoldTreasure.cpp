// Fill out your copyright notice in the Description page of Project Settings.


#include "GoldTreasure.h"
#include "MyEchoChar.h"

void AGoldTreasure::MySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyEchoChar* echoChar{ Cast<AMyEchoChar>(OtherActor) };

	if (echoChar && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 10.f, FColor::Blue, "picked up gold.");
		Destroy();
	}
}

void AGoldTreasure::MySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::MySphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}
