// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticItemActor.h"
#include "PaperCharacterBase.h"
#include "PaperFlipbookComponent.h"
#include "Components/BoxComponent.h"

AStaticItemActor::AStaticItemActor()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AStaticItemActor::OnBoxCollisionBeginOverlap);
	BoxCollision->SetupAttachment(GetRenderComponent());
	SetRootComponent(GetRenderComponent());
}

void AStaticItemActor::BeginPlay()
{
	Super::BeginPlay();
}

void AStaticItemActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AStaticItemActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (BoxCollision)
	{
		BoxCollision->SetBoxExtent(FVector(BoxCollisionSize.X, 50.f, BoxCollisionSize.Y));
		BoxCollision->SetRelativeLocation(FVector(BoxCollisionPosition.X, 0.f, BoxCollisionPosition.Y));
	}
}

void AStaticItemActor::OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APaperCharacterBase* Player = Cast<APaperCharacterBase>(OtherActor);
	if (Player) Player->OnBeginOverlapStaticItem(ItemType);
	switch (ItemType)
	{
	case EStaticItemType::Coin:
		// TODO: Play sound
	case EStaticItemType::Heart:
		// TODO: Play sound
	case EStaticItemType::BloodPag:
		// TODO: Play sound
		if (Player) Destroy();
		break;
	}
	OnAfterHit(OtherActor);
}

void AStaticItemActor::OnAfterHit(AActor* OtherActor)
{
}
