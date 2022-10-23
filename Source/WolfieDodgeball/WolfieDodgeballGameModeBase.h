// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WBallSpawner.h"
#include "WState.h"
#include "WolfieDodgeballGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WOLFIEDODGEBALL_API AWolfieDodgeballGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;

public:

	UFUNCTION()
	void GameEnd();

	UFUNCTION()
	void AddScore(int32 point);

	UFUNCTION()
	void GenerateBall();

	UPROPERTY(VisibleAnywhere)
	int32 Score;
	
	UPROPERTY(VisibleAnywhere)
	int32 MaxBallCount;

	UPROPERTY(EditAnywhere)
	int32 CurBallCount;
};
