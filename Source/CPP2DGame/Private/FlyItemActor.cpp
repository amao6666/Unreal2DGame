// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyItemActor.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"
#include "PaperCharacterEnemyBase.h"

AFlyItemActor::AFlyItemActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bCanMove = false;
	CurrentTime = 0.f;
	bIsHited = false;
	ItemType = EStaticItemType::Trap;
	HitSprite = nullptr;
}

void AFlyItemActor::BeginPlay()
{
	Super::BeginPlay();
	if (GetRenderComponent()->GetFlipbook()) FlySprite = GetRenderComponent()->GetFlipbook(); 
	StartLocation = GetActorLocation();
	GetRenderComponent()->OnFinishedPlaying.AddDynamic(this, &AFlyItemActor::OnHitPlayFinished);
}

void AFlyItemActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentTime < DelaySecond)
	{
		CurrentTime += DeltaSeconds;
	}
	else
	{
		bCanMove = true;
	}
	if (bCanMove && !bIsHited) AddActorWorldOffset(Direction * 3, true);
}

void AFlyItemActor::OnAfterHit(AActor* OtherActor)
{
	Super::OnAfterHit(OtherActor);
	APaperCharacterEnemyBase* Enemy = Cast<APaperCharacterEnemyBase>(OtherActor);
	AStaticItemActor* ItemObj = Cast<AStaticItemActor>(OtherActor);
	if (Enemy || ItemObj) return;
	bCanMove = false;
	bIsHited = true;
	if (BoxCollision) BoxCollision->SetCollisionProfileName(TEXT("NoCollision"));
	if (HitSprite)
	{
		GetRenderComponent()->SetFlipbook(HitSprite);
		GetRenderComponent()->SetLooping(false);
		GetRenderComponent()->Play();
	}
}

void AFlyItemActor::OnHitPlayFinished()
{
	SpawnSelfToStartLocation();
	Destroy();
}

void AFlyItemActor::SpawnSelfToStartLocation()
{
	FTransform SpawnLocationAndRotation = FTransform(GetActorRotation(), StartLocation, FVector::OneVector);
	AFlyItemActor* NewSelf = GetWorld()->SpawnActorDeferred<AFlyItemActor>(AFlyItemActor::StaticClass(), SpawnLocationAndRotation);

	NewSelf->BoxCollisionSize = BoxCollisionSize;
	NewSelf->BoxCollisionPosition = BoxCollisionPosition;
	NewSelf->Direction = Direction;
	NewSelf->DelaySecond = DelaySecond;
	NewSelf->HitSprite = HitSprite;
	NewSelf->GetRenderComponent()->SetFlipbook(FlySprite);
	NewSelf->FinishSpawning(SpawnLocationAndRotation);
}
