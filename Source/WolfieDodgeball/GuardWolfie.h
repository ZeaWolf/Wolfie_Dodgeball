// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WState.h"
#include "Dodgeball.h"
#include "WolfieDodgeballGameModeBase.h"
#include "WolfieCharacter.h"
#include "GuardWolfie.generated.h"

UCLASS()
class WOLFIEDODGEBALL_API AGuardWolfie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuardWolfie();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<ADodgeball> ProjectileClass;

	// Boolean whether character holds ball;
	bool isHolding;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Function that handles pick up.
	UFUNCTION()
		void PickUpBall();

	UFUNCTION()
		void ToggleHolding();

	UFUNCTION()
		bool GetHolding();

	UFUNCTION()
		void OnDamaged();

	UFUNCTION()
		void OnFire();

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

};
