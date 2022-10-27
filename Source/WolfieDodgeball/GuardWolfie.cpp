// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardWolfie.h"

// Sets default values
AGuardWolfie::AGuardWolfie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	isHolding = false;

}

// Called when the game starts or when spawned
void AGuardWolfie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGuardWolfie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetHolding()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("Fire W"));
		OnFire();
	}

}

// Called to bind functionality to input
void AGuardWolfie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AGuardWolfie::GetHolding() {
	return isHolding;
}
void AGuardWolfie::ToggleHolding()
{
	if (GetHolding())
		isHolding = false;
	else
		isHolding = true;
}

void AGuardWolfie::PickUpBall()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("[Guard Wolfie] pick up the ball."));
	ToggleHolding();
}

void AGuardWolfie::OnDamaged()
{
	//Send the score?
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("[Guard Wolfie] hit by [Player]"));
	// Destroy
	Destroy();
}

void AGuardWolfie::OnFire()
{
	// Attempt to fire a projectile.
	if (ProjectileClass && GetHolding())
	{
		// Get the camera transform.
		FVector GuardLocation;
		FRotator GuardRotation;
		GetActorEyesViewPoint(GuardLocation, GuardRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = GuardLocation + FTransform(GuardRotation).TransformVector(MuzzleOffset);


		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = GuardRotation;
		MuzzleRotation.Pitch += 30.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			class AWolfieDodgeballGameModeBase* myGameMode = (AWolfieDodgeballGameModeBase*)World->GetAuthGameMode();
			class AWolfieCharacter* myPawn = Cast<AWolfieCharacter>(myGameMode->DefaultPawnClass.GetDefaultObject());
			FVector PlayerLocation;
			FRotator PlayerRotation;
			myPawn->GetActorEyesViewPoint(PlayerLocation, PlayerRotation);
			//UE_LOG(LogTemp, Warning, TEXT("player: %s"), *PlayerLocation.ToString());
			//UE_LOG(LogTemp, Warning, TEXT("player: %s"), *GuardLocation.ToString());
	
			// Spawn the projectile at the muzzle.
			ADodgeball* Projectile = World->SpawnActor<ADodgeball>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->SetOwnerType(CharacterType::Guard);
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = (GuardLocation - PlayerLocation);

				//UE_LOG(LogTemp, Warning, TEXT("launch: %s"), *LaunchDirection.ToString());
				Projectile->FireInDirection(LaunchDirection);

				ToggleHolding();
			}
		}
	}
}
