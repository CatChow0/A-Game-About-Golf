#include "Actor/GolfBall.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/PlayerChara.h"
#include "GameMode/BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsSettings.h"

AGolfBall::AGolfBall()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAll"));
	RootComponent = SphereComponent;

	// Enable continuous collision detection
	SphereComponent->BodyInstance.bUseCCD = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->ArrowSize = 5.0f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 150.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	// Secondary Sphere collider to use a a trigger box
	TriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerSphere"));
	TriggerSphere->InitSphereRadius(100.0f);
	TriggerSphere->SetCollisionProfileName(TEXT("Trigger"));
	TriggerSphere->SetupAttachment(RootComponent);
	

	CurrentForce = 0.f;
	bIsCharging = false;
	// Increase the physics sub-stepping
	UPhysicsSettings* PhysicsSettings = UPhysicsSettings::Get();
	if (PhysicsSettings)
	{
		PhysicsSettings->bSubstepping = true;
		PhysicsSettings->MaxSubstepDeltaTime = 1.0f / 60.0f;
		PhysicsSettings->MaxSubsteps = 8;
	}
}

void AGolfBall::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AGolfGameMod>(UGameplayStatics::GetGameMode(GetWorld()));
	ToggleArrowVisibility(false);
	InitialRotation = GetActorRotation();
	LastShotPosition = GetActorLocation();

	// Set the friction coefficient
	FrictionCoef = FrictionCoefficient;
	ForceScal = ForceScale;

	PlayerController = Cast<APlayerController>(GetController());
}

void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCharging)
	{
		CurrentForce += ForceChargeRateScale * ChargeRate * DeltaTime;
		if (CurrentForce > MaxForce)
		{
			CurrentForce = MaxForce;
		}
	}
	else if (CurrentForce > 0.f)
	{
		CurrentForce -= ForceChargeRateScale * ChargeRate * DeltaTime;
		if (CurrentForce < 0.f)
		{
			CurrentForce = 0.f;
		}
	}

	if (IsOnGround())
	{
		FVector Velocity = SphereComponent->GetPhysicsLinearVelocity();
		FVector NewVelocity = FMath::Lerp(Velocity, FVector::ZeroVector,  FrictionCoef * DeltaTime);
		SphereComponent->SetPhysicsLinearVelocity(NewVelocity);
	}
	else
	{
		FVector Velocity = SphereComponent->GetPhysicsLinearVelocity();
		FVector NewVelocity = FMath::Lerp(Velocity, FVector::ZeroVector, AirFrictionCoef * DeltaTime);
		SphereComponent->SetPhysicsLinearVelocity(NewVelocity);
	}

	// print the bool is on ground to the screen
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("IsOnGround: %s"), IsOnGround() ? TEXT("True") : TEXT("False")));
	
	if (GameMode)
	{
		float ForcePercentage = (CurrentForce / MaxForce) * 100.0f;
		// Print to screen the force percentage
		GameMode->UpdateBar(ForcePercentage);
	}

	// Update ArrowComponent pitch based on ClubAngle
	FRotator CurrentRotation = ArrowComponent->GetComponentRotation();
	FRotator NewRotation = FRotator(ClubAngle, CurrentRotation.Yaw, CurrentRotation.Roll);
	ArrowComponent->SetWorldRotation(NewRotation);


	FVector Velocity = SphereComponent->GetPhysicsLinearVelocity();
	
	// Check if the ball's speed is below the threshold
	if ((Velocity.Size() < MovementThreshold) && bHasBeenShot)
	{
		InactiveTime += DeltaTime;
		if (InactiveTime >= InactiveDuration)
		{
			// Stop the ball and reset its rotation
			ResetRound();
			LastShotPosition = GetActorLocation();
			
		}
	}
	else
	{
		InactiveTime = 0.0f;
	}
}

void AGolfBall::ResetRound()
{
	// Stop linear and angular velocity
	SphereComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	SphereComponent->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	// Force the body to sleep to prevent movement
	SphereComponent->PutRigidBodyToSleep();

	// Disable collision to prevent physical interactions
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetActorRotation(InitialRotation);
	InactiveTime = 0.0f;
	bHasBeenShot = false;
	ToggleArrowVisibility(true);
}

void AGolfBall::SetupInput()
{
	
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BallContext, 0);
		}
	}
}

void AGolfBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AGolfBall::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGolfBall::Look);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AGolfBall::Shoot);
		EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Started, this, &AGolfBall::StartCharging);
		EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Completed, this, &AGolfBall::StopCharging);
		EnhancedInputComponent->BindAction(SwitchClubAction, ETriggerEvent::Triggered, this, &AGolfBall::SwitchClub);
	}
}

void AGolfBall::ToggleControl()
{
	bIsControllingBall = !bIsControllingBall;

	if (bIsControllingBall)
	{
		// Switch to controlling the ball
		if (PlayerController)
		{
			PlayerController->Possess(this);
			SetupInput();
		}
	}
	else
	{
		// Switch to controlling the player
		if (PlayerController)
		{
			PlayerController->Possess(PlayerChara);
			PlayerChara->SetupInput();
		}
	}
}

void AGolfBall::SwitchClub(const FInputActionValue& Value)
{

	// Print to screen the switch club message when the action is triggered
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Switch Club"));
	
	if (PlayerChara)
	{
		
		PlayerChara->SwitchClubMessage(this);
	}
}

void AGolfBall::Move(const FInputActionValue& Value)
{
	if (bHasBeenShot) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// Interpolate the rotation smoothly
		FRotator CurrentRotation = GetActorRotation();
		FRotator TargetRotation = CurrentRotation;
		TargetRotation.Yaw += MovementVector.X * RotationSpeed * GetWorld()->GetDeltaSeconds();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f); // 5.0f is the interpolation speed
		SetActorRotation(NewRotation);
	}
}

void AGolfBall::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (Controller)
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void AGolfBall::Shoot(const FInputActionValue& Value)
{
	if (bHasBeenShot) return;

	const bool bIsShooting = Value.Get<bool>();

	if (bIsShooting)
	{
		// Re-enable collision to allow physical interactions
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		FVector ForwardVector = ArrowComponent->GetForwardVector();
		FVector UpVector = FVector::UpVector;

		// Calculate the force vector using the angle
		FVector ForceVector = ForwardVector * FMath::Cos(FMath::DegreesToRadians(ClubAngle)) + UpVector * FMath::Sin(FMath::DegreesToRadians(ClubAngle));
		ForceVector *= CurrentForce * ForceScal;

		SphereComponent->AddImpulse(ForceVector, NAME_None, true);
		CurrentForce = 0.f;

		bHasBeenShot = true;

		ToggleArrowVisibility(false);
	}
}
void AGolfBall::StartCharging(const FInputActionValue& Value)
{
	bIsCharging = true;
}

void AGolfBall::StopCharging(const FInputActionValue& Value)
{
	bIsCharging = false;
}

void AGolfBall::HandleWaterOverlap()
{
	// Implement the logic to handle the overlap with water
	ResetRound();
	SetActorLocation(LastShotPosition);
}

void AGolfBall::EnterSandOverlap()
{
	FrictionCoef = SandFrictionCoefficient;
	ForceScal = SandForceScale;
}

void AGolfBall::ExitSandOverlap()
{
	FrictionCoef = FrictionCoefficient;
	ForceScal = ForceScale;
}

void AGolfBall::DisableFriction()
{
	SphereComponent->SetLinearDamping(0.0f);
	SphereComponent->SetAngularDamping(0.0f);
}

void AGolfBall::EnableFriction()
{
	SphereComponent->SetLinearDamping(FrictionCoef);
	SphereComponent->SetAngularDamping(FrictionCoef);
}

bool AGolfBall::IsOnGround() const
{
	TArray<FHitResult> OutHits;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, SphereComponent->GetScaledSphereRadius() + 1.f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	return GetWorld()->LineTraceMultiByChannel(OutHits, Start, End, ECC_Visibility, CollisionParams);
}