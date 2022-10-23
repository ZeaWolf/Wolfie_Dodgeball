// Fill out your copyright notice in the Description page of Project Settings.


#include "Dodgeball.h"

// Sets default values
ADodgeball::ADodgeball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
		CollisionComponent->InitSphereRadius(15.0f);
		// Set the root component to be the collision component.
		RootComponent = CollisionComponent;
	}

	if (!ProjectileMovementComponent)
	{
		// Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 100.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
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
	// InitialLifeSpan = 3.0f;

	OwnerType = CharacterType::None;
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
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void ADodgeball::SetOwnerType(CharacterType Type)
{
	OwnerType = Type;
}

// Function that is called when the projectile hits something.
void ADodgeball::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	check(GEngine != nullptr);

	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		if (OwnerType == CharacterType::None)
		{		
			OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
			check(GEngine != nullptr);

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit _ 1"));
		}
		Destroy();
	}

	if (OtherActor != this && !OtherComponent->IsSimulatingPhysics())
	{
		check(GEngine != nullptr);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HIH_2"));

		if (OwnerType == CharacterType::Player)
		{
			if (OtherActor->ActorHasTag("Wolfie"))
			{
				// OtherActor::GetAttack() 
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("[Wolfie] hit by [Player]"));
			}

			else if (OtherActor->ActorHasTag("Guard"))
			{
				// OtherActor::GetAttack()
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("[Guard] hit by [Player]"));
			}
			Destroy();
		}

		else if (OwnerType == CharacterType::Guard)
		{
			if (OtherActor->ActorHasTag("Player"))
			{
				// OtherActor::GetAttack()
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("[Player] hit by [Guard]"));
			}
			Destroy();
		}

		else if (OwnerType == CharacterType::None)
		{
			// Player & Wolfie & Guard have "Character" tag with their own tag.
			if (OtherActor->ActorHasTag("Character")) 
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Pick up the ball."));
			}
			Destroy();	
		}
	}
}