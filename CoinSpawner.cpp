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
	
	FTimerManager &worldTimeManager = GetWorld()->GetTimerManager(); //Ÿ�̸� ����

	worldTimeManager.SetTimer(CoinSetTimerHandle, this, &ACoinSpawner::SpawnCoinSet, CoinSetTimeInterval, false);//�����Ŀ� �� �� ���� ��Ʈ �ϴ� ��. ������ ���� ���� c++�� ������ ����
	//Ÿ�̸� �ڵ�, �����Ǵ� �Լ��� ȣ���� ������Ʈ, Ÿ�̸ӿ� �����ϰ��� �ϴ� �Լ��� �ڵ�, Ÿ�̸� ����, Ÿ�̸� �ݺ�����
	worldTimeManager.SetTimer(SpawnMoveTimerHandle, this, &ACoinSpawner::MoveSpawner,MovementTimeInterval, true);//���ʸ��� �ٴ� ���°�ٿ� ���� �����Ұ��� �������� �����ִ� Ÿ�̸�
	//�̰� ������Ʈ �ص� ��


}

void ACoinSpawner::SpawnCoin()
{

	if (FMath::Rand() % 100 < PowerUpChance) {
		SpawnPowerUp();
		NumCoinsToSpawn--;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("the item spawned"));
		//poweUpChance �������гο��� ���� 30���� �����������ϱ� 30�ۼ��� Ȯ���� ������ �Ǵ��� Ȯ���� �� ��.
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

void ACoinSpawner::SpawnCoinSet()//�����Ŀ� �� �� ���� ��Ʈ �ϴ� ��. ������ ���� ���� c++�� ������ ����
{
	NumCoinsToSpawn = FMath::RandRange(MinSetCoins, MaxSetCoins);

	FTimerManager &worldTimeManager = GetWorld()->GetTimerManager();

	worldTimeManager.ClearTimer(CoinSetTimerHandle);

	worldTimeManager.SetTimer(CoinTimerHandle, this, &ACoinSpawner::SpawnCoin, CoinTimeInterval, true);//���� ������ ���ʸ��� �����ؼ� �����ϴ� ��

	
}

void ACoinSpawner::MoveSpawner()//���ʸ��� �ٴ� ���°�ٿ� ���� �����Ұ��� �������� �����ִ� Ÿ�̸ӿ� ����� �Լ�
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

