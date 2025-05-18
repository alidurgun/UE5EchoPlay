// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEchoAnimations.h"
#include "MyEchoChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMyEchoAnimations::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	MyChar = Cast<AMyEchoChar>(TryGetPawnOwner());

	if (MyChar)
	{
		CharMovement = MyChar->GetCharacterMovement();
	}
}

void UMyEchoAnimations::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (CharMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharMovement->Velocity);
		IsFalling = CharMovement->IsFalling();
		CharacterState = MyChar->getCharacterState();
	}
}