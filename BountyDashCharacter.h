// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDash.h"
#include "PowerUpItemEnum.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"
#include "BountyDashGameModeBase.h"
#include "BountyDashCharacter.generated.h"

UCLASS()
class BOUNTYDASH_API ABountyDashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABountyDashCharacter();

	void ScoreUp();

	UPROPERTY(EditAnywhere, Category = Logic)
		TArray<class ATargetPoint*> TargetArray;

	UPROPERTY(EditAnywhere, Category = Logic)
		float CharSpeed;

	UPROPERTY(EditAnywhere, Category = Sound)
		class UAudioComponent *hitObstacleSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		class UAudioComponent *dingSound;

	UFUNCTION()
		void PowerUp(EPowerUp Type);

	int GetScore();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveRight();
	void MoveLeft();

	UFUNCTION()
		void MyOnComponentOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult&SweepResult);

	UFUNCTION()
		void MyOnComponentEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CameraBoom;//穿号識情

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camere)
		class UCameraComponent *FollowCamera;//穿号識情

	UPROPERTY(BlueprintReadOnly)
		int32 Score;


	//powerups below
	UPROPERTY(EditAnywhere, Category = PowerUps)
		float SmashTime;

	UPROPERTY(EditAnywhere, Category = PowerUps)
		float MagnetTime;

	UPROPERTY(EditAnywhere, Category = PowerUps)
		float MagnetReach;

	
	UFUNCTION()
		void StopSmash();
	UFUNCTION()
		void StopMagnet();

	void CoinMagnet();

	UFUNCTION()
		void Reset();
	

	ABountyDashGameModeBase *MyGameMode;
private:
	short CurrentLocation;
	FVector desiredLocation;
	bool bBeingPushed;


	bool CanSmash;
	bool CanMagnet;

	float Killpoint;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
