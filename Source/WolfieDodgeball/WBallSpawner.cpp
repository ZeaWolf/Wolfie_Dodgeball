// Fill out your copyright notice in the Description page of Project Settings.


#include "WBallSpawner.h"

// Sets default values
AWBallSpawner::AWBallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	SpawnVolume->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AWBallSpawner::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AWBallSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// AWolfieDodgeballGameModeBase* gameMode = Cast<AWolfieDodgeballGameModeBase>(UGameplayStatics::GetGameMode(this));
	// if (GameMode)
	// {
	// 	GameMode.CurBallCount++;
	// }
}

void AWBallSpawner::SpawnBall()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	GetWorld()->SpawnActor<ADodgeball>(SpawnLocation, SpawnRotation);
}
