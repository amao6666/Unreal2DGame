// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperCharacterEnemySoldier.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacterBase.h"

APaperCharacterEnemySoldier::APaperCharacterEnemySoldier()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBoxCollision"));
	WeaponBox->SetupAttachment(GetSprite());
	WeaponBox->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("WeaponSprite"));
	WeaponSprite->SetupAttachment(GetSprite());
}

void APaperCharacterEnemySoldier::BeginPlay()
{
	Super::BeginPlay();
	WeaponSprite->SetLooping(false);
	WeaponSprite->SetHiddenInGame(true);
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &APaperCharacterEnemySoldier::OnWeaponBeginOverlap);
	WeaponSprite->OnFinishedPlaying.AddDynamic(this, &APaperCharacterEnemySoldier::OnMagicEnd);
}

void APaperCharacterEnemySoldier::OnAttack()
{
	Super::OnAttack();
	WeaponBox->SetCollisionProfileName(TEXT("OverlapAll"));
	WeaponSprite->SetHiddenInGame(false);
	FVector TempLocation = WeaponSprite->GetComponentLocation();
	TempLocation.Y = 10;
	WeaponSprite->SetWorldLocation(TempLocation);
	WeaponSprite->PlayFromStart();
}

void APaperCharacterEnemySoldier::OnAttackEnd()
{
	Super::OnAttackEnd();
	if (WeaponSprite->GetFlipbook()) return;
	WeaponBox->SetCollisionProfileName(TEXT("NoCollision"));
}

void APaperCharacterEnemySoldier::OnDead()
{
	if (WeaponBox) WeaponBox->SetCollisionProfileName(TEXT("NoCollision"));
	Super::OnDead();
}

void APaperCharacterEnemySoldier::OnMagicEnd()
{
	WeaponBox->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponSprite->SetHiddenInGame(true);
}

void APaperCharacterEnemySoldier::OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APaperCharacterBase* Player = Cast<APaperCharacterBase>(OtherActor);
	if (Player)
	{
		Player->OnHit(Damage);
	}
}
