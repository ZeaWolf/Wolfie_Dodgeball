// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dodgeball.h"
#include "WState.h"
#include "WBallSpawner.generated.h"

UCLASS()
class WOLFIEDODGEBALL_API AWBallSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWBallSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnBall();

	UPROPERTY()
	FTimerHandle MemberTimerHandle;
};
