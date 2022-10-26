// Fill out your copyright notice in the Description page of Project Settings.


#include "TrueWolfie.h"


// Sets default values
ATrueWolfie::ATrueWolfie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrueWolfie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrueWolfie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrueWolfie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ATrueWolfie::OnDamaged()
{
	Destroy();

	//End the Game;
}
