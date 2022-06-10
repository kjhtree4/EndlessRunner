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
	bool BeingPulled;//메그넷 효과에 의해 당겨지는지에 대한 부울

	bool isSpawnAbove;
	int test;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent *Mesh;

	virtual void BeginPlay() override;

	
	
};
