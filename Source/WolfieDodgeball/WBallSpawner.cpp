// Fill out your copyright notice in the Description page of Project Settings.


#include "WBallSpawner.h"

// Sets default values
AWBallSpawner::AWBallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
}

// Called when the game starts or when spawned
void AWBallSpawner::BeginPlay()
{
	Super::BeginPlay();	
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AWBallSpawner::SpawnBall, 1.0f, true, 2.0f);


}

// Called every frame
void AWBallSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWBallSpawner::SpawnBall()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	FVector RandomLocation = SpawnLocation + (FMath::VRand()*500);
	ADodgeball* Ball = GetWorld()->SpawnActor<ADodgeball>(RandomLocation, SpawnRotation);
	
	if (Ball != nullptr)
	{
		Ball->SetOwnerType(CharacterType::Spawner);
	}

	check(GEngine);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Spawn ball"));
}
