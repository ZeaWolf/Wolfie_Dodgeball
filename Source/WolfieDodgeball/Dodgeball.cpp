// Fill out your copyright notice in the Description page of Project Settings.


#include "Dodgeball.h"
#include "WolfieCharacter.h"
#include "GuardWolfie.h"
#include "TrueWolfie.h"

// Sets default values
ADodgeball::ADodgeball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!PrimitiveComponent)
	{
		PrimitiveComponent = CreateDefaultSubobject<UPrimitiveComponent>(TEXT("PrimitiveComponent"));
	}
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if (!CollisionComponent)
	{
		// Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision profile name to "Projectile".
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Dodgeball"));
		// Event called when component hits something.
		CollisionComponent->OnComponentHit.AddDynamic(this, &ADodgeball::OnHit);
		// Set the sphere's collision radius.
		CollisionComponent->InitSphereRadius(20.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 30000.0f; // initially 0
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 2.0f;
	}



	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Sphere.Sphere'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
	}

	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/SphereMaterial.SphereMaterial'"));
	if (Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	// Delete the projectile after 3 seconds.
	InitialLifeSpan = 10.0f;

	OwnerType = CharacterType::Player;
}

// Called when the game starts or when spawned
void ADodgeball::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADodgeball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function that initializes the projectile's velocity in the shoot direction.
void ADodgeball::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection* ProjectileMovementComponent->InitialSpeed;
}

void ADodgeball::SetOwnerType(CharacterType Type)
{
	if (Type == CharacterType::Spawner)
	{
		ProjectileMovementComponent->SetVelocityInLocalSpace(*(new FVector(0.0f, 0.0f, 0.0f)));
	}
	OwnerType = Type;
}

// Function that is called when the projectile hits something.
void ADodgeball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	check(GEngine != nullptr);

	UWorld* World = GetWorld();
	AWolfieCharacter* myPawn = nullptr;
	AWolfieDodgeballGameModeBase* myGameMode = nullptr;
	if (World)
	{
		myGameMode = (AWolfieDodgeballGameModeBase*)World->GetAuthGameMode();
		// myPawn = Cast<AWolfieCharacter>(myGameMode->DefaultPawnClass.GetDefaultObject());
		myPawn = Cast<AWolfieCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}

	if (OtherActor != nullptr)
	{
		// Player throw the ball.
		if (OwnerType == CharacterType::Player && OtherActor->IsValidLowLevelFast())
		{
			// Hit Main Wolfie.
			if (OtherActor->ActorHasTag(FName(TEXT("Wolfie"))))
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("[Wolfie] hit by [Player]"));
				if (myPawn->IsValidLowLevel()) {
					myPawn->AddPoint();
				}

				 if (ATrueWolfie* Wolfie = Cast<ATrueWolfie>(OtherActor))
				 {
				 	Wolfie->OnDamaged();
					this->SetOwnerType(CharacterType::None);
				 }
			}
			// Hit Guard Wolfie.
			else if (OtherActor->ActorHasTag(FName(TEXT("Guard"))))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("[Guard] hit by [Player]"));

				if (myPawn) {
					myPawn->AddPoint();
				}
				

				if (AGuardWolfie* Guard = Cast<AGuardWolfie>(OtherActor))
				{
					Guard->OnDamaged();
					this->SetOwnerType(CharacterType::None);
				}

				Destroy();
			}
		}

		// Guard Wolfie throw the ball.
		else if (OwnerType == CharacterType::Guard && OtherActor->IsValidLowLevelFast())
		{
			// Hit Player.
			if (OtherActor->ActorHasTag(FName(TEXT("Player"))))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("[Player] hit by [Guard Wolfie]"));
				if (AWolfieCharacter* Player = Cast<AWolfieCharacter>(OtherActor))
				{
					Player->OnDamaged();
					this->SetOwnerType(CharacterType::None);
				}
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("[Player] hit by [Guard]"));
				//Destroy();
			}
		}

		// Pick up the ball.
		else if (OwnerType == CharacterType::None && OtherActor->IsValidLowLevelFast())
		{
			if (OtherActor->ActorHasTag(FName(TEXT("Player")))) 
			{
				if (AWolfieCharacter* Player = Cast<AWolfieCharacter>(OtherActor))
				{
					if (!Player->GetHolding()) {
						Player->PickUpBall();
						Destroy();
					}
				}
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("[Player] Pick up the ball."));
			}
			else if (OtherActor->ActorHasTag(FName(TEXT("Guard"))))
			{
				if (AGuardWolfie* Guard = Cast<AGuardWolfie>(OtherActor))
				 {
					if (!Guard->GetHolding()) {
						Guard->PickUpBall();
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("[Guard] Pick up the ball."));
						Destroy();
					}
				 }
			}
			else if (OtherActor->ActorHasTag(FName(TEXT("Wolfie"))))
			{
				//GEngine->AddOnScreenDebugMessage(01, 5.0f, FColor::Blue, TEXT("[Wolfie] Pick up the ball."));
				//Destroy();
			}
		}

		else
		{
			this->SetOwnerType(CharacterType::None);
		}
	}
}

