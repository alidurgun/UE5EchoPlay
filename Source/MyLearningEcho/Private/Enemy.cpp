// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	//DrawDebugSphere(GetWorld(), ImpactPoint, 10.0f, 10, FColor::Red, false, 5.0f);

	// Spawn particle now
	if (hitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitParticle, ImpactPoint);
	}
	
	// Calculate where the hit comes from.
	const FName sectionName = calculateHitLocation(ImpactPoint);

	// Then play montage according to hit direction.
	PlayMontage(sectionName);
}

void AEnemy::PlayMontage(const FName& sectionName)
{
	UAnimInstance* anim{ GetMesh()->GetAnimInstance() };
	
	if (anim && HitMontage)
	{
		// To play montage.
		anim->Montage_Play(HitMontage);

		// Jump to that section.
		anim->Montage_JumpToSection(sectionName, HitMontage);
	}
}

const FName AEnemy::calculateHitLocation(const FVector& ImpactPoint)
{
	const FVector Forward{ GetActorForwardVector() };

	// Lower impact point to the Enemy's Actor Location Z.
	const FVector ImpactLowered{ ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z };
	const FVector ToHit = FVector(ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double cosTheta = FVector::DotProduct(Forward, ToHit);

	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	// result in radiant form.
	double theta = FMath::Acos(cosTheta);

	// convert from radiant to degrees.
	theta = FMath::RadiansToDegrees(theta);

	const FVector Crossprod{ FVector::CrossProduct(Forward,ToHit) };

	// if cross product points down then theta should be negative.
	if (Crossprod.Z < 0)
	{
		theta *= -1;
	}

	FName sectionName{ "HitFromBack" };
	if (theta < 45.f && theta >= -45.f) {
		sectionName = FName{ "HitFromFront" };
	}
	else if (theta >= 135.f && theta < -45.f) {
		sectionName = FName{ "HitFromLeft" };
	}
	else if (theta >= 45.f && theta < 135.f) {
		sectionName = FName{ "HitFromRight" };
	}

	return sectionName;
}

