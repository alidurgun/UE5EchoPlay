// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MyEchoChar.h"

void AWeapon::MySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::MySphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	AMyEchoChar* echoChar{ Cast<AMyEchoChar>(OtherActor) };
	if (echoChar)
	{
		UE_LOG(LogTemp, Display, TEXT("Setting character for weapon equip!"));
		echoChar->setWeaponInRange(true);
		echoChar->setWeaponMesh(ItemMesh);
	}
	UE_LOG(LogTemp, Display, TEXT("Overlap start function ended"));
}

void AWeapon::MySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::MySphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	AMyEchoChar* echoChar{ Cast<AMyEchoChar>(OtherActor) };
	if (echoChar)
	{
		echoChar->setWeaponInRange(false);
	}
}