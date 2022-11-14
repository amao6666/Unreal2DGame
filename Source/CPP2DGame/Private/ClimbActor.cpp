// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbActor.h"

#include "PaperCharacterBase.h"
#include "PaperSprite.h"
#include "PaperGroupedSpriteComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AClimbActor::AClimbActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GroupedSpriteComp = CreateDefaultSubobject<UPaperGroupedSpriteComponent>(TEXT("GroupedSpriteComp"));
	SetRootComponent(GroupedSpriteComp);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AClimbActor::OnClimbBeginOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AClimbActor::OnClimbEndOverlap);
	BoxCollision->SetupAttachment(GroupedSpriteComp);

	TopBoxOverlap = CreateDefaultSubobject<UBoxComponent>(TEXT("TopBoxOverlap"));
	TopBoxOverlap->SetCollisionProfileName(TEXT("OverlapAll"));
	TopBoxOverlap->OnComponentBeginOverlap.AddDynamic(this, &AClimbActor::OnClimbTopBeginOverlap);
	TopBoxOverlap->OnComponentEndOverlap.AddDynamic(this, &AClimbActor::OnClimbTopEndOverlap);
	TopBoxOverlap->SetupAttachment(GroupedSpriteComp);
}

// Called when the game starts or when spawned
void AClimbActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClimbActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClimbActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ConstructClimb();
	ConstructBoxCollision();
	ConstructTopBoxCollision();
}

void AClimbActor::ConstructClimb()
{
	if (!GroupedSpriteComp) return;
	GroupedSpriteComp->ClearInstances();
	// Generate Top Climb
	if (!ClimbT) return;
	float ClimbItemHeight = ClimbT->GetSourceSize().Y;
	ClimbWidth = ClimbT->GetSourceSize().X;
	ClimbHeight = ClimbItemHeight;
	FTransform TempTransform(FRotator::ZeroRotator,FVector(0.f, 0.f, 0.f),FVector::OneVector);
	FVector TempLocation = TempTransform.GetLocation();
	GroupedSpriteComp->AddInstance(TempTransform, ClimbT);
	// Generate Middle Climb
	if (!ClimbM) return;
	for (int32 i = 0; i < ClimbMiddleCount; i++)
	{
		TempLocation.Z -= ClimbItemHeight;
		ClimbHeight += ClimbItemHeight;
		TempTransform.SetLocation(TempLocation);
		GroupedSpriteComp->AddInstance(TempTransform, ClimbM);
		ClimbItemHeight = ClimbM->GetSourceSize().Y;
	}
	// Generate Bottom Climb
	if (!ClimbB) return;
	TempLocation.Z -= ClimbItemHeight;
	ClimbHeight += ClimbItemHeight;
	TempTransform.SetLocation(TempLocation);
	GroupedSpriteComp->AddInstance(TempTransform, ClimbB);
}

void AClimbActor::ConstructBoxCollision()
{
	if (!BoxCollision || !ClimbT || !ClimbM) return;
	BoxCollision->SetBoxExtent(FVector(ClimbWidth / 2, 50.f , ClimbHeight / 2));
	FTransform TempTransform;
	GroupedSpriteComp->GetInstanceTransform(0, TempTransform);
	FVector TempLocation = TempTransform.GetLocation();
	TempLocation.Z -= ClimbT->GetSourceSize().Y / 2 + (ClimbM->GetSourceSize().Y * ClimbMiddleCount) / 2;
	BoxCollision->SetRelativeLocation(TempLocation);
}

void AClimbActor::ConstructTopBoxCollision()
{
	if (!TopBoxOverlap || !ClimbT || !ClimbM) return;
	TopBoxOverlap->SetBoxExtent(FVector(ClimbWidth / 2, 50.f , 5.f));
	FTransform TempTransform;
	GroupedSpriteComp->GetInstanceTransform(0, TempTransform);
	FVector TempLocation = TempTransform.GetLocation();
	TempLocation.Z += ClimbT->GetSourceSize().Y / 2;
	TopBoxOverlap->SetRelativeLocation(TempLocation);
}

void AClimbActor::OnClimbBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APaperCharacterBase* Player = Cast<APaperCharacterBase>(OtherActor);
	if (Player) Player->OnBeginOverlapClimb(this);
}

void AClimbActor::OnClimbEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	APaperCharacterBase* Player = Cast<APaperCharacterBase>(OtherActor);
	if (Player) Player->OnEndOverlapClimb(this);
}

void AClimbActor::OnClimbTopBeginOverlap(UPrimitiveComponent* Comp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APaperCharacterBase* Player = Cast<APaperCharacterBase>(OtherActor);
	if (Player) Player->bIsOnClimbTop = true;
}

void AClimbActor::OnClimbTopEndOverlap(UPrimitiveComponent* Comp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	APaperCharacterBase* Player = Cast<APaperCharacterBase>(OtherActor);
	if (Player) Player->bIsOnClimbTop = false;
}

