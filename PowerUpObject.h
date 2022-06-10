// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PowerUpItemEnum.h"
#include "BountyDashObject.h"
#include "PowerUpObject.generated.h"

/**
 * 
 */

UCLASS()
class BOUNTYDASH_API APowerUpObject : public ABountyDashObject
{
	GENERATED_BODY()

	APowerUpObject();

	virtual void Tick(float DeltaTime) override;

	virtual void MyOnActorBeginOverlap(AActor *OverlappedAActor, AActor *otherActor);//여기에 아이템당 기능 적고.

public:
	bool isSpawnAbove;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent *Mesh;

	UPROPERTY(BlueprintReadWrite)
		EPowerUp Type;

	UFUNCTION()//이거 블루프린트에서 오버라이드 해서 스태틱메쉬 적용 할 수 있도록 하자.
		void MySetStaticMesh(EPowerUp StaticMeshTypeEnum);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh *SPEEDMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh *MAGNETMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMesh *SMASHMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial *SPEEDMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial *MAGNETMat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial *SMASHMat;

	UFUNCTION()
		void SetItemType();
	
};
