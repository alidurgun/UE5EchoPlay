// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "BaseCharacter.h"
#include "MyEchoChar.generated.h"

// Forward Decleration
//class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UPrimitiveComponent;
class UAnimMontage;
class AWeapon;
class UCharacterAttributes;

UCLASS()
class MYLEARNINGECHO_API AMyEchoChar : public ABaseCharacter
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

	void Attack();

	UPROPERTY(VisibleAnywhere)
	bool isWeaponInRange{ false };

	bool weaponEquipped{ false };

	//void PlayMontage();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* weaponMesh;

	AWeapon* equippedWeapon;

	ECharacterState CharacterState{ ECharacterState::ECS_Unequipped };

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState{ ECharacterActionState::ECAS_Unoccupied };

public:
	// Sets default values for this character's properties
	AMyEchoChar();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EquipWeapon();

	void AttachSwordToSocket(const FName& socketName);

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* ArmDisarmMontage;

	FORCEINLINE void setWeaponInRange(bool status) { isWeaponInRange = status; }
	FORCEINLINE void setWeapon(AWeapon* currentWeapon) { equippedWeapon = currentWeapon; }
	FORCEINLINE void setWeaponMesh(UStaticMeshComponent* weaponMesh_) { weaponMesh = weaponMesh_; }
	FORCEINLINE ECharacterState getCharacterState() const { return CharacterState; }
	
	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type newCollision);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
