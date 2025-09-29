// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include <CharacterAttributes.h>
#include <Widget_HealthBarComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"

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

	//CharacterAttributes = CreateDefaultSubobject<UCharacterAttributes>(TEXT("Character Attributes"));

	WHealthBarComponent = CreateDefaultSubobject<UWidget_HealthBarComponent>(TEXT("Widget Health Bar"));
	WHealthBarComponent->SetupAttachment(GetRootComponent());


	// AI Controlling related codes.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Pawn Sensing.
	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	SensingComponent->SightRadius = 2000.f;
	SensingComponent->SetPeripheralVisionAngle(45.0f);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (WHealthBarComponent && charAttributes) {
		// Turn of the health bar.
		WHealthBarComponent->SetVisibility(false);
		WHealthBarComponent->SetHealthPercent(charAttributes->GetCurrentHealthPercent());
	}

	// AI Navigation.
	AiController = Cast<AAIController>(GetController());

	int32 rand = FMath::RandRange(0, PatrolTargets.Num() - 1);
	CurrentPatrolTarget = PatrolTargets[rand];

	MoveToTarget(CurrentPatrolTarget);

	// set character speed to walking speed.
	GetCharacterMovement()->MaxWalkSpeed = 130.0f;

	if (SensingComponent) {
		SensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (charAttributes && WHealthBarComponent) {
		if (!isAlive()) {
			Die();
			WHealthBarComponent->SetVisibility(false);
		}
		else {
			// Start chase.
			CurrentState = EEnemyActionState::EEAS_Chasing;
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
			EnemyTarget = DamageCauserActor;
			MoveToTarget(EnemyTarget);
			WHealthBarComponent->SetVisibility(true);
		}
	}
	return Damage;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DamageCauserActor)
	{
		const double distance{ (DamageCauserActor->GetActorLocation() - GetActorLocation()).Size() };
		if (distance > AggroRange && WHealthBarComponent) {
			WHealthBarComponent->SetVisibility(false);
			DamageCauserActor = nullptr;
		}
	}

	if (CurrentState > EEnemyActionState::EEAS_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::Die()
{
	// call the parent version.
	Super::Die();

	// disable collision:
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetLifeSpan(3.0f);
}

void AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> NextLocations;
	// new array without current point.
	for (AActor* target : PatrolTargets) {
		if (target != CurrentPatrolTarget)
			NextLocations.AddUnique(target);
	}

	// select next target
	int32 index{ FMath::RandRange(0, NextLocations.Num() - 1) };
	UE_LOG(LogTemp, Display, TEXT("PatrolInTheArea"));
	CurrentPatrolTarget = NextLocations[index];

}

void AEnemy::ChaseEnemy()
{
	if (InTargetRange(EnemyTarget, AttackRadius)) {
		// attack it.
	}
}

bool AEnemy::InTargetRange(AActor* target, double radius)
{
	if (target == nullptr) return false;

	double distance{ (target->GetActorLocation() - GetActorLocation()).Size() };

	return distance <= radius;
}

void AEnemy::MoveToTarget(AActor* target)
{
	if (AiController == nullptr || target == nullptr) return;

	FAIMoveRequest MoveReq;
	MoveReq.SetGoalActor(target);
	MoveReq.SetAcceptanceRadius(15.0f);
	AiController->MoveTo(MoveReq);
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(CurrentPatrolTarget, PatrolAcceptanceRadius))
	{
		ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 6.0f);
	}
}

void AEnemy::CheckCombatTarget()
{
	// Enemy loose interest against the player.
	if (!InTargetRange(EnemyTarget, CombatRadius)) {
		EnemyTarget = nullptr;
		if (WHealthBarComponent) {
			WHealthBarComponent->SetVisibility(false);
		}
		CurrentState = EEnemyActionState::EEAS_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		ChoosePatrolTarget();
		MoveToTarget(CurrentPatrolTarget);
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}

	else if (!InTargetRange(EnemyTarget, AttackRadius) && CurrentState != EEnemyActionState::EEAS_Chasing) {
		// outside the attack range, chase it.
		UE_LOG(LogTemp, Display, TEXT("Chasing"));
		CurrentState = EEnemyActionState::EEAS_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		GetWorldTimerManager().ClearTimer(AttackTimer);
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		MoveToTarget(EnemyTarget);
	}

	else if (InTargetRange(EnemyTarget, AttackRadius) && CurrentState != EEnemyActionState::EEAS_Engaging) {
		EnemyAttack();
	}
}

void AEnemy::EnemyAttack()
{
	if (CurrentState == EEnemyActionState::EEAS_Attacking) return;
	CurrentState = EEnemyActionState::EEAS_Engaging;
	UAnimInstance* anim{ GetMesh()->GetAnimInstance() };
	if (anim && this->AttackMontage) {

		anim->Montage_Play(AttackMontage);
		UE_LOG(LogTemp, Display, TEXT("Attack called"));
		int32 attNum{ FMath::RandRange(0,2) };
		FName sectionName{};
		
		switch (attNum)
		{
		case 0:
			sectionName = FName{ "EnemyAttack1" };
			break;
		case 1:
			sectionName = FName{ "EnemyAttack2" };
			break;
		case 2:
			sectionName = FName{ "EnemyAttack3" };
			break;
		default:
			sectionName = FName{ "EnemyAttack2" };
			break;
		}
		anim->Montage_JumpToSection(sectionName);
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::AttackTimerFinished, 2.0f + attNum);
		CurrentState = EEnemyActionState::EEAS_Attacking;
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::AttackTimerFinished()
{
	if (CurrentState != EEnemyActionState::EEAS_Attacking && CurrentState != EEnemyActionState::EEAS_Engaging) {
		// clear the timer.
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}
	else {
		CurrentState = EEnemyActionState::EEAS_Engaging;
		EnemyAttack();
	}
}

void AEnemy::SpawnWeapon()
{
	UWorld* world = GetWorld();
	if (world) {
		AWeapon* DefaultWeapon = world->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		equippedWeapon = DefaultWeapon;
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	// Enemy saw our character.
	if (SeenPawn->ActorHasTag(FName("Character")) && CurrentState != EEnemyActionState::EEAS_Chasing)
	{
		// clear the timer.
		GetWorldTimerManager().ClearTimer(PatrolTimer);

		// Enemy will run now.
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;

		// set enemy target.
		EnemyTarget = SeenPawn;

		if (CurrentState != EEnemyActionState::EEAS_Attacking) {
			CurrentState = EEnemyActionState::EEAS_Chasing;
			MoveToTarget(EnemyTarget);
		}
	}
}

void AEnemy::SetWeaponCollision(ECollisionEnabled::Type newCollision)
{
	if (equippedWeapon)
	{
		equippedWeapon->GetWeaponBox()->SetCollisionEnabled(newCollision);
		equippedWeapon->ActorsAlreadyHit.Empty();
	}
}