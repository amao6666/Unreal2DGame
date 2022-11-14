// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "BulletBaseActor.generated.h"

UCLASS()
class CPP2DGAME_API ABulletBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBaseActor();
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere)
	class UPaperFlipbookComponent* BulletSprite;
	UPROPERTY(EditAnywhere)
	class UPaperFlipbook* BulletFlyFlipbook;
	UPROPERTY(EditAnywhere)
	class UPaperFlipbook* BulletHitFlipbook;
	class APaperCharacterBase* Creator;
	UPROPERTY(EditAnywhere)
	FVector BulletFlyDirection;

	bool bIsStop;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void TimerCallback();

	UFUNCTION()
	void OnHitAnimationPlayEnd();

	UFUNCTION()
	void OnBulletBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
