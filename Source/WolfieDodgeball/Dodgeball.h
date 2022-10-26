// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "WState.h"

#include "Dodgeball.generated.h"

UCLASS()
class WOLFIEDODGEBALL_API ADodgeball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADodgeball();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function that is called when the projectile hits something.
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Set Owner type (thrown by)
	UFUNCTION()
	void SetOwnerType(CharacterType type);
	
	// Sphere collision component.
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComponent;

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;

	// Projectile mesh
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;

	// Projectile material
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	UPROPERTY(EditAnyWhere)
	CharacterType OwnerType;

	UPROPERTY(VisibleAnywhere)
	UPrimitiveComponent* PrimitiveComponent;

	// Function that initializes the projectile's velocity in the shoot direction.
	void FireInDirection(const FVector& ShootDirection);

};
