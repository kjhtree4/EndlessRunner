// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDash.h"
#include "GameFramework/Actor.h"
#include "Obstacle.h"
#include "BountyDashGameModeBase.h"
#include "ObstacleSpawner.generated.h"

UCLASS()
class BOUNTYDASH_API AObstacleSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacleSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SpawnObstacle();

	ABountyDashGameModeBase *MyGameMode;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<AObstacle>> ObstaclesToSpawn;//책 틀림 TSubclassOf<>템플릿 안에는 포인터형식이 들어갈 수 없으므로 전방선언은 할 수 없음
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ATargetPoint*> SpawnTargets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SpawnTimer;
	UPROPERTY()
		USceneComponent * Scene;

private:
	float KillPoint;
	float SpawnPoint;
	float TimeSinceLastSpawn;
};
