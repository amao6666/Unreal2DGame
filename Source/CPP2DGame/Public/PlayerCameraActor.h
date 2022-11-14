// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCameraActor.generated.h"

UCLASS()
class CPP2DGAME_API APlayerCameraActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere)
	float LeftLocationX;
	
	UPROPERTY(EditAnywhere)
	float RightLocationX;
	
	UPROPERTY(EditAnywhere)
	USpringArmComponent* ArmComp;

	UPROPERTY(EditAnywhere)
	UCameraComponent* MainCameraComp;

	class APaperCharacterBase* CurrentPlayerCharacter;
	APlayerCameraActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FollowingCharacter();
};
