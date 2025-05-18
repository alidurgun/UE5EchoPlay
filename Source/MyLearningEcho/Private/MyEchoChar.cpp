// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEchoChar.h"
#include "Treasure.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Components/PrimitiveComponent.h"

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

}

void AMyEchoChar::MoveForwardBack(float Value)
{
	if (Value != 0.f && Controller)
	{
		const FRotator ControllerRotation{ GetControlRotation() };
		const FRotator YawRotation{ 0.f, ControllerRotation.Yaw, 0.f };
		const FVector Direction{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void AMyEchoChar::MoveRightLeft(float Value)
{
	if (Value != 0.f && Controller)
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

void AMyEchoChar::EquipWeapon()
{
	if (isWeaponInRange) {
		UE_LOG(LogTemp, Display, TEXT("Trying to attach to component"));
		FAttachmentTransformRules myRule{ EAttachmentRule::SnapToTarget, true };
		weaponMesh->AttachToComponent(GetMesh(), myRule, FName("hand_rSocket"));
		weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterState = ECharacterState::ECS_OneHandedWeaponEquipped;
	}
}