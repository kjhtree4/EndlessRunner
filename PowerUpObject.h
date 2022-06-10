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

	virtual void MyOnActorBeginOverlap(AActor *OverlappedAActor, AActor *otherActor);//���⿡ �����۴� ��� ����.

public:
	bool isSpawnAbove;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent *Mesh;

	UPROPERTY(BlueprintReadWrite)
		EPowerUp Type;

	UFUNCTION()//�̰� �������Ʈ���� �������̵� �ؼ� ����ƽ�޽� ���� �� �� �ֵ��� ����.
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
