// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "MyEchoAnimations.generated.h"

// Forward Decleration
class AMyEchoChar;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class MYLEARNINGECHO_API UMyEchoAnimations : public UAnimInstance
{
	GENERATED_BODY()

public:
	// begin play.
	virtual void NativeInitializeAnimation() override;
	
	// like a tick function. But it'll evaluate even the game is not
	// running so always check against nullptr
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	AMyEchoChar* MyChar;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* CharMovement;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | State")
	ECharacterState CharacterState;
};
