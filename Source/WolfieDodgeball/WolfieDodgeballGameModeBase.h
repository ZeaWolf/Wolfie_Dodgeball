// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WolfieDodgeballGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WOLFIEDODGEBALL_API AWolfieDodgeballGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;
};
