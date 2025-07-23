// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_HealthBarComponent.h"
#include <HealthBar.h>
#include <Components/ProgressBar.h>

void UWidget_HealthBarComponent::SetHealthPercent(float HealthPercent)
{
	if (WidgetHealthBar == nullptr) {
		WidgetHealthBar = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (WidgetHealthBar && WidgetHealthBar->HealthBar) {
		WidgetHealthBar->HealthBar->SetPercent(HealthPercent);
	}
}
