// Copyright Epic Games, Inc. All Rights Reserved.


#include "WolfieDodgeballGameModeBase.h"

void AWolfieDodgeballGameModeBase::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is Wolfie Dodgeball Game Mode!"));

	Score = 0;
	CurBallCount = 0;
	MaxBallCount = 20;

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AWBallSpawner::SpawnBall, 1.0f, true, 2.0f);
}

void AWolfieDodgeballGameModeBase::GameEnd()
{
	// Game End Event
}

void AWolfieDodgeballGameModeBase::AddScore(int32 point)
{
	Score = Score + point;
}

void AWolfieDodgeballGameModeBase::GenerateBall()
{

}