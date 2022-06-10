// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDashObject.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class BOUNTYDASH_API ACoin : public ABountyDashObject
{
	GENERATED_BODY()

	ACoin();
	 

	virtual void Tick(float DeltaTime) override;

	virtual void MyOnActorBeginOverlap(AActor *OverlappedAActor, AActor *otherActor);

public:
	bool BeingPulled;//�ޱ׳� ȿ���� ���� ����������� ���� �ο�

	bool isSpawnAbove;
	int test;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent *Mesh;

	virtual void BeginPlay() override;

	
	
};
