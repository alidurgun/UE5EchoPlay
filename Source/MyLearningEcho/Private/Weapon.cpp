// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MyEchoChar.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"

AWeapon::AWeapon()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	BoxComponent->SetupAttachment(GetRootComponent());

	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponCollisionStart = CreateDefaultSubobject<USceneComponent>(TEXT("Collision Start Point"));
	WeaponCollisionStart->SetupAttachment(GetRootComponent());

	WeaponCollisionEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Collision End Point"));
	WeaponCollisionEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::OnBoxOverlapStart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector start = WeaponCollisionStart->GetComponentLocation();
	const FVector end = WeaponCollisionEnd->GetComponentLocation();
	const FVector halfSize{ 5.f,5.f,5.f };
	TArray<AActor*> ActorsToIgnore{ this };
	FHitResult hitResult;

	for (AActor* actor : ActorsAlreadyHit) {
		ActorsToIgnore.AddUnique(actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(this, start, end, halfSize,
		WeaponCollisionStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1,
		false, ActorsToIgnore, EDrawDebugTrace::None, hitResult, true);

	// we are adding which actor that we hit in order to prevent multiple hit.
	ActorsAlreadyHit.AddUnique(OtherActor);

	// Call break function if we are hitting any destructible object.
	BreakSomething(hitResult.ImpactPoint);

	if (AActor* hitActor = hitResult.GetActor()) {
		IHitInterface* hitInterface = Cast<IHitInterface>(hitActor);

		if (hitInterface) {
			// because of it is native event now. we are calling it like that.
			hitInterface->Execute_GetHit(hitActor, hitResult.ImpactPoint);
		}
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlapStart);
}

void AWeapon::MySphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::MySphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	AMyEchoChar* echoChar{ Cast<AMyEchoChar>(OtherActor) };
	if (echoChar)
	{
		UE_LOG(LogTemp, Display, TEXT("Setting character for weapon equip!"));
		echoChar->setWeaponInRange(true);
		echoChar->setWeapon(this);
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