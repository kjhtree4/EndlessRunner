// Fill out your copyright notice in the Description page of Project Settings.


#include "BountyDashObject.h"

// Sets default values
ABountyDashObject::ABountyDashObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	
	RootComponent = Collider;
	Collider->SetCollisionProfileName("OverlapAllDyamic");

	OnActorBeginOverlap.AddDynamic(this, &ABountyDashObject::MyOnActorBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &ABountyDashObject::MyOnActorEndOverlap);
}

// Called when the game starts or when spawned
void ABountyDashObject::BeginPlay()
{
	Super::BeginPlay();
	
	MyGameMode = Cast<ABountyDashGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ABountyDashObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("aaaaaa"));

	float gameSpeed = MyGameMode->GetInvGameSpeed();

	AddActorWorldOffset(FVector(gameSpeed, 0.0f, 0.0f));

	if (GetActorLocation().X < KillPoint) {
		Destroy();
	}


}

void ABountyDashObject::SetKillPoint(float point)
{
	KillPoint = point;
}

float ABountyDashObject::GetKillPoint()
{
	return KillPoint;
}

void ABountyDashObject::MyOnActorBeginOverlap(AActor * OverlappedAActor, AActor * otherActor)
{
}

void ABountyDashObject::MyOnActorEndOverlap(AActor * OverlappedAActor, AActor * otherActor)
{
}

