// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletBaseActor.h"

#include "PaperCharacterEnemyBase.h"
#include "PaperFlipbookComponent.h"
#include "StaticItemActor.h"
#include "PaperCharacterBase.h"

// Sets default values
ABulletBaseActor::ABulletBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	
	BulletSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("SpriteComp"));
	BulletSprite->SetupAttachment(RootComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComp"));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollision->SetupAttachment(RootComponent);

	BulletFlyFlipbook = nullptr;
	BulletHitFlipbook = nullptr;
	Creator = nullptr;
	BulletFlyDirection = FVector::ZeroVector;
	bIsStop = false;
}

// Called when the game starts or when spawned
void ABulletBaseActor::BeginPlay()
{
	Super::BeginPlay();
	if (BulletFlyFlipbook) BulletSprite->SetFlipbook(BulletFlyFlipbook);
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABulletBaseActor::TimerCallback, 5.f, false);
	if (BulletSprite) BulletSprite->OnFinishedPlaying.AddDynamic(this, &ABulletBaseActor::OnHitAnimationPlayEnd);
	if (BoxCollision) BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABulletBaseActor::OnBulletBeginOverlap);
}

// Called every frame
void ABulletBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsStop) return;
	AddActorWorldOffset(BulletFlyDirection * 4.5, false);
}

void ABulletBaseActor::TimerCallback()
{
	// Destroy after 5 seconds
	Destroy();
}

void ABulletBaseActor::OnHitAnimationPlayEnd()
{
	Destroy();
}

void ABulletBaseActor::OnBulletBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AStaticItemActor* ItemActor = Cast<AStaticItemActor>(OtherActor);
	if (ItemActor) return;
	APaperCharacterEnemyBase* Enemy = Cast<APaperCharacterEnemyBase>(OtherActor);
	if (Enemy)
	{
		if (Creator)
		{
			Enemy->OnHit(Creator->Damage);
		}
	}
	bIsStop = true;
	if (BoxCollision) BoxCollision->SetCollisionProfileName(TEXT("NoCollision"));
	if (BulletHitFlipbook)
	{
		BulletSprite->SetFlipbook(BulletHitFlipbook);
		BulletSprite->SetLooping(false);
		BulletSprite->Play();
	}
}

