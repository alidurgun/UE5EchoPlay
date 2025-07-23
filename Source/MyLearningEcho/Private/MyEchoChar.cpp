// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEchoChar.h"
#include "Treasure.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Animation/AnimInstance.h"
#include "Weapon.h"
#include "Components/BoxComponent.h"
#include <NiagaraComponent.h>

// Sets default values
AMyEchoChar::AMyEchoChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleMesh"));
	//RootComponent = capsule;

	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	arm->SetupAttachment(GetRootComponent());
	arm->TargetArmLength = 300.f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(arm);

	// for realistic hair.
	hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));

	// this attach hair to the skeletal mesh of the character.
	// GetMesh => returns skeletal mesh component of the character.(usually body)
	// hair will follow the mesh now.
	hair->SetupAttachment(GetMesh());

	// this specifies which socket/bone hair should attach to.
	// AttachmentName is used to define the specific point of attachment.
	// "hair" is the name of our socket/bone in the body.
	hair->AttachmentName = FString{ "head" };

	eyebrow = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrow"));
	eyebrow->SetupAttachment(GetMesh());
	eyebrow->AttachmentName = FString{"head"};

}

void AMyEchoChar::Disarm()
{
	AttachSwordToSocket(FName{ "spineSocket" });
}

void AMyEchoChar::Arm()
{
	UE_LOG(LogTemp, Display, TEXT("Calling arm"));
	AttachSwordToSocket(FName{ "hand_rSocket" });
}

void AMyEchoChar::SetWeaponCollision(ECollisionEnabled::Type newCollision)
{
	if (weaponEquipped && equippedWeapon)
	{
		equippedWeapon->GetWeaponBox()->SetCollisionEnabled(newCollision);
		equippedWeapon->ActorsAlreadyHit.Empty();
	}
}

// Called when the game starts or when spawned
void AMyEchoChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyEchoChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyEchoChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ForwardBack ws
	PlayerInputComponent->BindAxis(FName("ForwardBack"), this, &AMyEchoChar::MoveForwardBack);
	
	// RightLeft ad
	PlayerInputComponent->BindAxis(FName("RightLeft"), this, &AMyEchoChar::MoveRightLeft);
	
	// LookLeftRight mouse x
	PlayerInputComponent->BindAxis(FName("LookLeftRight"), this, &AMyEchoChar::LookRightLeft);

	// LookUpDown mouse y
	PlayerInputComponent->BindAxis(FName("LookUpDown"), this, &AMyEchoChar::LookUpDown);

	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(FName("EquipWeapon"), EInputEvent::IE_Pressed, this, &AMyEchoChar::EquipWeapon);

	PlayerInputComponent->BindAction(FName("Attack"), EInputEvent::IE_Pressed, this, &AMyEchoChar::Attack);

}

void AMyEchoChar::MoveForwardBack(float Value)
{
	if (Value != 0.f && Controller && ActionState == ECharacterActionState::ECAS_Unoccupied)
	{
		const FRotator ControllerRotation{ GetControlRotation() };
		const FRotator YawRotation{ 0.f, ControllerRotation.Yaw, 0.f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void AMyEchoChar::MoveRightLeft(float Value)
{
	if (Value != 0.f && Controller && ActionState == ECharacterActionState::ECAS_Unoccupied)
	{
		const FRotator ControlRotation{ GetControlRotation() };
		const FRotator YawRotation{ 0.f, ControlRotation.Yaw, 0.f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AMyEchoChar::LookRightLeft(float Value)
{
	if (Value != 0.f && Controller)
	{
		AddControllerYawInput(Value);
	}
}

void AMyEchoChar::LookUpDown(float Value)
{
	if (Value != 0.f && Controller)
	{
		AddControllerPitchInput(Value);
	}
}

void AMyEchoChar::Attack()
{
	if (ActionState == ECharacterActionState::ECAS_Unoccupied
		&& CharacterState != ECharacterState::ECS_Unequipped)
	{
		PlayMontage();
	}
}

void AMyEchoChar::PlayMontage()
{
	UAnimInstance* anim{ GetMesh()->GetAnimInstance() };

	if (anim && AttackMontage)
	{
		ActionState = ECharacterActionState::ECAS_Attacking;

		// to play montage.
		anim->Montage_Play(AttackMontage);

		// to make attack animation random.
		int32 selection{ FMath::RandRange(0,1) };

		// to specify section name.
		FName sectionName{};
		switch (selection) {
		case 0:
			sectionName = FName("Attack1");
			break;
		case 1:
			sectionName = FName("Attack2");
			break;
		default:
			sectionName = FName("Attack1");
			break;
		}
		anim->Montage_JumpToSection(sectionName, AttackMontage);
	}
	else
		UE_LOG(LogTemp, Error, TEXT("AttackMontage or anim is null"));
}

void AMyEchoChar::EquipWeapon()
{
	if (weaponEquipped) {
		// follow disarm or arm animations.
		UAnimInstance* anim{ GetMesh()->GetAnimInstance() };

		if (anim && ActionState == ECharacterActionState::ECAS_Unoccupied &&
			CharacterState == ECharacterState::ECS_Unequipped && ArmDisarmMontage) {
			// arm it
			anim->Montage_Play(ArmDisarmMontage);
			anim->Montage_JumpToSection("Arm", ArmDisarmMontage);
			CharacterState = ECharacterState::ECS_OneHandedWeaponEquipped;
		}

		else if (anim && ActionState == ECharacterActionState::ECAS_Unoccupied &&
			CharacterState != ECharacterState::ECS_Unequipped && ArmDisarmMontage) {
			// disarm it
			anim->Montage_Play(ArmDisarmMontage);
			anim->Montage_JumpToSection("Disarm", ArmDisarmMontage);
			CharacterState = ECharacterState::ECS_Unequipped;
		}
	}

	if (isWeaponInRange && !weaponEquipped) {
		UE_LOG(LogTemp, Display, TEXT("Trying to attach to component"));
		AttachSwordToSocket(FName{ "hand_rSocket" });
		weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterState = ECharacterState::ECS_OneHandedWeaponEquipped;
		weaponEquipped = true;

		if (UNiagaraComponent* embersEffect = equippedWeapon->GetEmberEffect()) {
			embersEffect->Deactivate();
		}

		// owner of the weapon is character now.
		equippedWeapon->SetOwner(this);

		// it's instigator is also our character now.
		equippedWeapon->SetInstigator(this);
	}
}

void AMyEchoChar::AttachSwordToSocket(const FName& socketName)
{
	FAttachmentTransformRules myRule{ EAttachmentRule::SnapToTarget, true };
	weaponMesh->AttachToComponent(GetMesh(), myRule, socketName);
}
