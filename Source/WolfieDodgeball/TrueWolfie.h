// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WState.h"
#include "TrueWolfie.generated.h"

UCLASS()
class WOLFIEDODGEBALL_API ATrueWolfie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATrueWolfie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
