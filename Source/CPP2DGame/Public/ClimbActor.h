// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbActor.generated.h"

UCLASS()
class CPP2DGAME_API AClimbActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClimbActor();

	class UPaperGroupedSpriteComponent* GroupedSpriteComp;
	class UBoxComponent* BoxCollision;
	class UBoxComponent* TopBoxOverlap;
	
	UPROPERTY(EditAnywhere)
	class UPaperSprite* ClimbT;

	UPROPERTY(EditAnywhere)
	class UPaperSprite* ClimbM;

	UPROPERTY(EditAnywhere)
	class UPaperSprite* ClimbB;

	UPROPERTY(EditAnywhere)
	int32 ClimbMiddleCount;

	float ClimbWidth;
	float ClimbHeight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void ConstructClimb();
	void ConstructBoxCollision();
	void ConstructTopBoxCollision();
	
	UFUNCTION()
	void OnClimbBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnClimbEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnClimbTopBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnClimbTopEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

