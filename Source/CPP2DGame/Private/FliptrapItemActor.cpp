// Fill out your copyright notice in the Description page of Project Settings.


#include "FliptrapItemActor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"

AFliptrapItemActor::AFliptrapItemActor()
{
	NormalSprite = nullptr;
	MovementSprite = nullptr;
	CurrentTime = 0.f;
}

void AFliptrapItemActor::BeginPlay()
{
	Super::BeginPlay();
	if (NormalSprite) GetRenderComponent()->SetFlipbook(NormalSprite);
	if (BoxCollision) BoxCollision->SetCollisionProfileName(TEXT("NoCollision"));
	GetRenderComponent()->OnFinishedPlaying.AddDynamic(this, &AFliptrapItemActor::OnMovementPlayFinished);
}

void AFliptrapItemActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentTime < DelaySecond)
	{
		CurrentTime += DeltaSeconds;
	}
	else
	{
		OnMovementStart();
		CurrentTime = 0.f;
	}
}

void AFliptrapItemActor::OnMovementStart()
{
	if (BoxCollision) BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	if (!MovementSprite) return;
	GetRenderComponent()->SetFlipbook(MovementSprite);
	GetRenderComponent()->SetLooping(false);
	GetRenderComponent()->Play();
}

void AFliptrapItemActor::OnMovementPlayFinished()
{
	if (BoxCollision) BoxCollision->SetCollisionProfileName(TEXT("NoCollision"));
	if (!NormalSprite) return;
	GetRenderComponent()->SetFlipbook(NormalSprite);
	GetRenderComponent()->SetLooping(true);
	GetRenderComponent()->Play();
}
