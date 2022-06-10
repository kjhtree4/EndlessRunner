// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once


#include "BountyDash.h"
#include "GameFramework/GameModeBase.h"
#include "BountyDashGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class BOUNTYDASH_API ABountyDashGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
		float gameSpeed;

	UPROPERTY()
		int32 gameLevel;

public:
	ABountyDashGameModeBase();

	void CharScoreUp(unsigned int charScore);

	UFUNCTION()
		float GetInvGameSpeed();

	UFUNCTION()
		float GetGameSpeed();

	UFUNCTION()
		int32 GetGameLevel();


	void ReduceGameSpeed();

	float GetRunTime();

	UFUNCTION()
		virtual void Tick(float DeltaTime) override;


	UFUNCTION()
		bool GetGameOver();

	UFUNCTION()
		void GameOver();

	UFUNCTION()
		void SetGamePaused(bool gamePaused);



protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 numCoinsForSpeedIncrease;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float gameSpeedIncrease;
	UPROPERTY()
		float Runtime;

	UPROPERTY()
		bool bGameOver;

	UPROPERTY()
		bool startGameOverCount;

	UPROPERTY()
		float timeTillGameOver;

	UPROPERTY()
		float gameOverTimer;

};

