// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDash.h"
#include "GameFramework/Actor.h"
#include "BountyDashGameModeBase.h"
#include "BountyDashObject.generated.h"

UCLASS()
class BOUNTYDASH_API ABountyDashObject : public AActor
{
	GENERATED_BODY()

		float KillPoint;

public:	
	// Sets default values for this actor's properties
	ABountyDashObject();

	void SetKillPoint(float point);
	float GetKillPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void MyOnActorBeginOverlap(AActor *OverlappedAActor, AActor *otherActor);
	UFUNCTION()
		virtual void MyOnActorEndOverlap(AActor *OverlappedAActor, AActor * otherActor);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent *Collider;

	ABountyDashGameModeBase* MyGameMode;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
