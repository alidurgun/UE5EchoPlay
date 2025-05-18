// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "MyEchoChar.generated.h"

// Forward Decleration
//class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UPrimitiveComponent;

UCLASS()
class MYLEARNINGECHO_API AMyEchoChar : public ACharacter
{
	GENERATED_BODY()
private:
	//UPROPERTY(EditAnywhere)
	//UCapsuleComponent* capsule;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* arm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* camera;

	 UPROPERTY(EditAnywhere)
	 UGroomComponent* hair;

	 UPROPERTY(EditAnywhere)
	 UGroomComponent* eyebrow;

	UFUNCTION()
	void MoveForwardBack(float Value);

	UFUNCTION()
	void MoveRightLeft(float Value);

	UFUNCTION()
	void LookRightLeft(float Value);

	UFUNCTION()
	void LookUpDown(float Value);

	UPROPERTY(VisibleAnywhere)
	bool isWeaponInRange{ false };

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* weaponMesh;

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

public:
	// Sets default values for this character's properties
	AMyEchoChar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EquipWeapon();

	FORCEINLINE void setWeaponInRange(bool status) { isWeaponInRange = status; }
	FORCEINLINE void setWeaponMesh(UStaticMeshComponent* weaponMesh_) { weaponMesh = weaponMesh_; }
	FORCEINLINE ECharacterState getCharacterState() const { return CharacterState; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
