// Fill out your copyright notice in the Description page of Project Settings.

#include "BountyDashCharacter.h"
#include "BountyDashGameModeBase.h"
#include "Engine/Canvas.h"
#include "BountyDashHUD.h"

ABountyDashHUD::ABountyDashHUD() {
	
	static ConstructorHelpers::FObjectFinder<UFont> OurHudFont(TEXT("/Game/BountyDashFont.BountyDashFont"));
	HUDFont = OurHudFont.Object;
	
}

void ABountyDashHUD::DrawHUD()
{
	
	Super::DrawHUD();

	FVector2D ScreenDimenstions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	ABountyDashCharacter *DashCharacter = Cast<ABountyDashCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	DashGameMode= Cast<ABountyDashGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	FString HUDString = FString::Printf(TEXT("Score: %d    Runtime: %.4f"), DashCharacter->GetScore(), DashGameMode->GetRunTime());
	DrawText(HUDString, FColor::Yellow, 1000, 50, HUDFont);

	if (DashGameMode->GetGameOver()) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("GAMEOVERTEXT"));
		FVector2D GameOverSize;
		GetTextSize(TEXT("GAME OVER!!   Press R to Restart!"), GameOverSize.X, GameOverSize.Y, HUDFont);
		DrawText(TEXT("GAME OVER!!   Press R to Restart!"), FColor::Yellow, (ScreenDimenstions.X - GameOverSize.X) / 2.0f, (ScreenDimenstions.Y - GameOverSize.Y) / 2.0f, HUDFont);

		DashGameMode->SetGamePaused(true);
	}
	
}
