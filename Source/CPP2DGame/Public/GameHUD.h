// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class CPP2DGAME_API AGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	AGameHUD();
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainUIClass;
	
	class UUserWidget* MainUI;

	virtual void BeginPlay() override;
};
