// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "BountyDashCharacter.h"
#include "BountyDashHUD.h"
#include "BountyDashGameModeBase.h"


ABountyDashGameModeBase::ABountyDashGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ABountyDashCharacter::StaticClass();

	numCoinsForSpeedIncrease = 5;
	gameSpeed = 10.0f;
	gameSpeedIncrease = 10.0f;
	gameLevel = 1;

	HUDClass = ABountyDashHUD::StaticClass();

	Runtime = 0.0f;
	bGameOver = false;
	startGameOverCount = false;
	timeTillGameOver = 2.0f;
	gameOverTimer = 0.0f;

}

void ABountyDashGameModeBase::CharScoreUp(unsigned int charScore)
{

	if (charScore != 0 && charScore%numCoinsForSpeedIncrease == 0) {//코인을 5개씩 모을때마다 스피드와 레벨 증가
		gameSpeed += gameSpeedIncrease;
		gameLevel++;
	}

}

float ABountyDashGameModeBase::GetInvGameSpeed()
{
	return -gameSpeed;
}

float ABountyDashGameModeBase::GetGameSpeed()
{
	return gameSpeed;
}

int32 ABountyDashGameModeBase::GetGameLevel()
{
	return gameLevel;
}

void ABountyDashGameModeBase::ReduceGameSpeed() {
	if (gameSpeed > 10.0f) {
		gameSpeed -= gameSpeedIncrease;
		gameLevel--;
	}
}

float ABountyDashGameModeBase::GetRunTime()
{
	return Runtime;
}

void ABountyDashGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("game On"));
	if(!startGameOverCount){
		Runtime += DeltaTime;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("game On"));
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("REALgameover1"));
		gameOverTimer += DeltaTime;
		if (gameOverTimer >= timeTillGameOver) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("REALgameover2"));
			bGameOver = true;
		}
	}

}

void ABountyDashGameModeBase::GameOver()
{
	startGameOverCount = true;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("gameovertrue"));
}

void ABountyDashGameModeBase::SetGamePaused(bool gamePaused)
{
	APlayerController *myPlayer = GetWorld()->GetFirstPlayerController();
	if (myPlayer != nullptr) {
		myPlayer->SetPause(gamePaused);
	}
}

bool ABountyDashGameModeBase::GetGameOver()
{
	return bGameOver;
}