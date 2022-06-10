// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDash.h"
#include "GameFramework/Actor.h"
#include "BountyDashGameModeBase.h"
#include "Floor.generated.h"

UCLASS()
class BOUNTYDASH_API AFloor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloor();

	float GetKillPoint();
	float GetSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
		TArray<USceneComponent*> FloorMeshScene;
	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*> FloorMeshes;
	UPROPERTY(EditAnywhere)
		class UBoxComponent *CollisionBox;

	int32 NumRepeatingMesh;
	float KillPoint;
	float SpawnPoint;
	
	ABountyDashGameModeBase* MyGameMode;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};