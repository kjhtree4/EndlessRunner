// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/TargetPoint.h"
#include "Floor.h"
#include "EngineUtils.h"
#include "PowerUpObject.h"
#include "PowerUpItem.h"
#include "CoinSpawner.h"

// Sets default values
ACoinSpawner::ACoinSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	MaxSetCoins = 5;
	MinSetCoins = 3;
	CoinSetTimeInterval = 4.0f;
	CoinTimeInterval = 0.5f;
	MovementTimeInterval = 1.0f;
}

// Called when the game starts or when spawned
void ACoinSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter) {
		SpawnTransforms.Add(*TargetIter);
	}

	for (TActorIterator<AFloor> FloorIter(GetWorld()); FloorIter; ++FloorIter) {
		if (FloorIter->GetWorld() == GetWorld()) {
			KillPoint = FloorIter->GetKillPoint();
			SpawnPoint = FloorIter->GetSpawnPoint();
		}
	}
	
	FTimerManager &worldTimeManager = GetWorld()->GetTimerManager(); //타이머 생성

	worldTimeManager.SetTimer(CoinSetTimerHandle, this, &ACoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);//몇초후에 한 번 코인 세트 하는 것. 딜레이 역할 해줌 c++에 딜레이 없음
	//타이머 핸들, 제공되는 함수를 호출할 오브젝트, 타이머에 연결하고자 하는 함수의 핸들, 타이머 간격, 타이머 반복여부
	worldTimeManager.SetTimer(SpawnMoveTimerHandle, this, &ACoinSpawner::MoveSpawner,MovementTimeInterval, true);//몇초마다 바닥 몇번째줄에 코인 스폰할건지 랜덤으로 정해주는 타이머
	//이거 정리노트 해둘 것


}

void ACoinSpawner::SpawnCoin()
{

	if (FMath::Rand() % 100 < PowerUpChance) {
		SpawnPowerUp();
		NumCoinsToSpawn--;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("the item spawned"));
		//poweUpChance 디테일패널에서 지금 30으로 설정해줬으니까 30퍼센의 확률로 생성이 되는지 확인해 볼 것.
	}
	else{
	FActorSpawnParameters spawnParams;

	FTransform CoinTransform = SpawnTransforms[TargetLoc]->GetTransform();

	CoinTransform.SetLocation(FVector(SpawnPoint, CoinTransform.GetLocation().Y, CoinTransform.GetLocation().Z));

	ACoin *SpawnedCoin = GetWorld()->SpawnActor<ACoin>(CoinObject, CoinTransform, spawnParams);

	if (SpawnedCoin) {
		SpawnedCoin->SetKillPoint(KillPoint);
		USphereComponent *CoinSphere = Cast<USphereComponent>(SpawnedCoin->GetComponentByClass(USphereComponent::StaticClass()));

			if (CoinSphere) {
				float offset = CoinSphere->GetUnscaledSphereRadius();
				SpawnedCoin->AddActorLocalOffset(FVector(0.0f, 0.0f, offset));
			}
			NumCoinsToSpawn--;
		}
	}
	if (NumCoinsToSpawn <= 0) {
		FTimerManager &worldTimeManager = GetWorld()->GetTimerManager();
		worldTimeManager.SetTimer(CoinSetTimerHandle, this, &ACoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);
		worldTimeManager.ClearTimer(CoinTimerHandle);
	}
}

void ACoinSpawner::SpawnCoinSet()//몇초후에 한 번 코인 세트 하는 것. 딜레이 역할 해줌 c++에 딜레이 없음
{
	NumCoinsToSpawn = FMath::RandRange(MinSetCoins, MaxSetCoins);

	FTimerManager &worldTimeManager = GetWorld()->GetTimerManager();

	worldTimeManager.ClearTimer(CoinSetTimerHandle);

	worldTimeManager.SetTimer(CoinTimerHandle, this, &ACoinSpawner::SpawnCoin, CoinTimeInterval, true);//실제 코인을 몇초마다 루프해서 스폰하는 것

	
}

void ACoinSpawner::MoveSpawner()//몇초마다 바닥 몇번째줄에 코인 스폰할건지 랜덤으로 정해주는 타이머에 연결된 함수
{
	TargetLoc = FMath::Rand() % SpawnTransforms.Num();
}

void ACoinSpawner::SpawnPowerUp()
{
	FActorSpawnParameters spawnParams;

	FTransform myTrans= SpawnTransforms[TargetLoc]->GetTransform();

	myTrans.SetLocation(FVector(SpawnPoint, myTrans.GetLocation().Y, myTrans.GetLocation().Z));

	APowerUpObject *newObs = GetWorld()->SpawnActor<APowerUpObject>(PowerUpItem, myTrans, spawnParams);

	if (newObs) {
		newObs->SetKillPoint(KillPoint);
		USphereComponent *PowerUpSphere = Cast<USphereComponent>(newObs->GetComponentByClass(USphereComponent::StaticClass()));

		if (PowerUpSphere) {
			
			float offset = PowerUpSphere->GetUnscaledSphereRadius();
			newObs->AddActorLocalOffset(FVector(0.0f, 0.0f, offset));
		}
	}
}



// Called every frame
void ACoinSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

