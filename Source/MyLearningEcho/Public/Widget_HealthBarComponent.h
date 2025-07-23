// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Widget_HealthBarComponent.generated.h"

class UHealthBar;

/**
 * 
 */
UCLASS()
class MYLEARNINGECHO_API UWidget_HealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

private:
	UHealthBar* WidgetHealthBar;

public:
	void SetHealthPercent(float HealthPercent);
	
};
