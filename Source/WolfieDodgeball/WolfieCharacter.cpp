// Fill out your copyright notice in the Description page of Project Settings.


#include "WolfieCharacter.h"
#include "WolfieDodgeballGameModeBase.h"

// Sets default values
AWolfieCharacter::AWolfieCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a first person camera component.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// Attach the camera component to our capsule component.
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));

	// Position the camera slightly above the eyes.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

	// Enable the pawn to control camera rotation.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Create a first person mesh component for the owning player.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);

	// Only the owning player sees this mesh.
	FPSMesh->SetOnlyOwnerSee(true);

	// Attach the FPS mesh to the FPS camera.
	FPSMesh->SetupAttachment(FPSCameraComponent);

	// Disable some environmental shadows to preserve the illusion of having a single mesh.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// The owning player doesn't see the regular (third-person) body mesh.
	GetMesh()->SetOwnerNoSee(true);

	Score = 0;
	isHolding = false;
}

// Called when the game starts or when spawned
void AWolfieCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(GEngine != nullptr);

	// Display a debug message for five seconds. 
	// The -1 "Key" value argument prevents the message from being updated or refreshed.
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using WolfieCharacter."));
}

// Called every frame
void AWolfieCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWolfieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AWolfieCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWolfieCharacter::MoveRight);

	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("Turn", this, &AWolfieCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AWolfieCharacter::AddControllerPitchInput);

	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AWolfieCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AWolfieCharacter::StopJump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AWolfieCharacter::Fire);

	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AWolfieCharacter::Interact);
}


void AWolfieCharacter::MoveForward(float Value)
{
	// Find out which way is "forward" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AWolfieCharacter::MoveRight(float Value)
{
	// Find out which way is "right" and record that the player wants to move that way.
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AWolfieCharacter::StartJump()
{
	bPressedJump = true;
}

void AWolfieCharacter::StopJump()
{
	bPressedJump = false;
}

void AWolfieCharacter::Fire()
{
	//UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), Score);
	// UE_LOG(LogTemp, Warning, TEXT("Current: %p"), ProjectileClass);
	// Attempt to fire a projectile.
	if (ProjectileClass && GetHolding())
	{
		// Get the camera transform.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// Spawn the projectile at the muzzle.
			ADodgeball* Projectile = World->SpawnActor<ADodgeball>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				Projectile->SetOwnerType(CharacterType::Player);
				// Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);

				ToggleHolding();
			}
		}
	}
}

//void Awolfiecharacter::interact()
//{
//	gengine->addonscreendebugmessage(-1, 5.0f, fcolor::red, text("interacting"));
//	//ue_log(logtemp, warning, text("interacting"));
//	fvector cameralocation;
//	frotator camerarotation;
//	getactoreyesviewpoint(cameralocation, camerarotation);
//
//	fvector end = cameralocation + camerarotation.vector() * 500.0f;
//
//	fhitresult hitresult;
//	fcollisionqueryparams params;
//	params.addignoredactor(this);
//
//
//	if (getworld()->linetracesinglebychannel(hitresult, cameralocation, end, ecollisionchannel::ecc_worldstatic))
//	{
//		//gengine->addonscreendebugmessage(-1, 5.0f, fcolor::red, text("hit actor %s"), *hitresult.getactor()->getname());
//		ue_log(logtemp, warning, text("hit actor %s"), *hitresult.getactor()->getname());
//	}
//}
bool AWolfieCharacter::GetHolding() {
	return isHolding;
}
void AWolfieCharacter::ToggleHolding()
{
	if (GetHolding())
		isHolding = false;
	else
		isHolding = true;
}

void AWolfieCharacter::PickUpBall()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("[Player] pick up the ball."));
	ToggleHolding();
}


void AWolfieCharacter::OnDamaged()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("[Player] hit by [Guard Wolfie]"));

	// Lose

}

void AWolfieCharacter::AddPoint()
{
	Score = Score + 1;
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Score: %f", myGameMode->Score;));
	UE_LOG(LogTemp, Warning, TEXT("Current Score in charater: %d"), Score);
}