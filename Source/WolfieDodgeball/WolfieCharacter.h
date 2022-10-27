// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Dodgeball.h"
#include "WolfieCharacter.generated.h"

UCLASS()
class WOLFIEDODGEBALL_API AWolfieCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWolfieCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile class to spawn.
	UPROPERTY(EditDefaultsOnly, Category = Dodgeball)
	TSubclassOf<class ADodgeball> ProjectileClass;

	// Boolean whether character holds ball;
	bool isHolding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WScore;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles input for moving forward and backward.
	UFUNCTION()
	void MoveForward(float Value);

	// Handles input for moving right and left.
	UFUNCTION()
	void MoveRight(float Value);

	// Sets jump flag when key is pressed.
	UFUNCTION()
	void StartJump();

	// Clears jump flag when key is released.
	UFUNCTION()
	void StopJump();

	// Function that handles firing projectiles.
	UFUNCTION()
	void Fire();

	//Function that handles interact.
	//UFUNCTION()
	//void Interact();

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
	void AddPoint();

	//UFUNCTION(BlueprintCallable)
	//	void GameLose();

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;

	// First-person mesh (arms), visible only to the owning player.
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FPSMesh;

	// Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

};
