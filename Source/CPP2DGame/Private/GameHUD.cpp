// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AGameHUD::AGameHUD()
{
	//static ConstructorHelpers::FClassFinder<UUserWidget> MainUiObj(TEXT("WidgetBlueprint'/Game/UI/UI_MainUI.UI_MainUI_c'"));
	MainUIClass = LoadClass<UUserWidget>(this, TEXT("WidgetBlueprint'/Game/UI/UI_MainUI.UI_MainUI_c'"));
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	if (MainUIClass)
	{
		MainUI = CreateWidget<UUserWidget>(GetWorld(), MainUIClass);
		if (MainUI) MainUI->AddToViewport();
	}
}
