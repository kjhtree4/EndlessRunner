// Fill out your copyright notice in the Description page of Project Settings.

#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "Floor.h"
//#include "Obstacle.h"
#include "ObstacleSpawner.h"

// Sets default values
AObstacleSpawner::AObstacleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	check(Scene);
	RootComponent = Scene;

	SpawnTimer = 1.5f;
}

// Called when the game starts or when spawned
void AObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();

	MyGameMode = Cast<ABountyDashGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter) {
		SpawnTargets.Add(*TargetIter);
	}//타겟포인트 가져오기

	for (TActorIterator<AFloor> FloorIter(GetWorld()); FloorIter; ++FloorIter) {
			KillPoint = FloorIter->GetKillPoint();
			SpawnPoint = FloorIter->GetSpawnPoint();
	}

	TimeSinceLastSpawn = SpawnTimer;
}

void AObstacleSpawner::SpawnObstacle()
{
	if (SpawnTargets.Num() > 0 && ObstaclesToSpawn.Num() > 0) {
		short Spawner = FMath::Rand() % SpawnTargets.Num();
		short Obstical = FMath::Rand() % ObstaclesToSpawn.Num();
		float CapsuleOffset = 0.0f;

		FActorSpawnParameters SpawnInfo;

		FTransform myTrans = SpawnTargets[Spawner]->GetTransform();
		myTrans.SetLocation(FVector(SpawnPoint, myTrans.GetLocation().Y, myTrans.GetLocation().Z));

		AObstacle *newObs = GetWorld()->SpawnActor<AObstacle>(ObstaclesToSpawn[Obstical],myTrans,SpawnInfo);
		//이부분 노트정리해둘것.
		//TSubclassOf로 클래스 변수를 만들고 그 변수를 생성하는 기법임. 이 변수를 EditAnywhere로 선언해서 디테일패널에서 드래그로 끌어넣을 수 있음
		//나는 AObstacle::StaticClass()이렇게 하는 방법만 알았음
		

		if (newObs) {
			newObs->SetKillPoint(KillPoint);

			USphereComponent *obsSphere = Cast<USphereComponent>(newObs->GetComponentByClass(USphereComponent::StaticClass()));

			if (obsSphere) {
				newObs->AddActorLocalOffset(FVector(0.0f,0.0f,obsSphere->GetUnscaledSphereRadius()));
			}
		}
	}
}

// Called every frame
void AObstacleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastSpawn += DeltaTime;

	float trueSpawnTime = SpawnTimer / float(MyGameMode->GetGameLevel()); //레벨에따라 스폰이 빨라짐

	if (TimeSinceLastSpawn > trueSpawnTime) {
		TimeSinceLastSpawn = 0.0f;
		SpawnObstacle();
	}

}

