// PlayerChara.cpp

#include "Character/PlayerChara.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actor/Club.h"
#include "Actor/GolfBall.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode/BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ChildActorComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APlayerChara::APlayerChara()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	ClubComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Club"));
	ClubComponent->SetupAttachment(GetMesh(), TEXT("ClubSocket"));

	RootComponent->SetWorldScale3D(FVector(2.f));
}

// Called when the game starts or when spawned
void APlayerChara::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}

	GameMode = Cast<AGolfGameMod>(UGameplayStatics::GetGameMode(this));
	
	if (ClubComponent)
	{
		ClubComponent->SetChildActorClass(ClubClasses[CurrentClubIndex]);
		Club = Cast<AClub>(ClubComponent->GetChildActor());

		if (Club)
		{
			Club->SetActorHiddenInGame(true);
		}
	}
}

// Called every frame
void APlayerChara::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateCamera();
}

void APlayerChara::UpdateCamera()
{
	if (PlayerController)
	{
		float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, CameraDistances[CameraDistanceIndex] + CameraDistanceOffset, DeltaTime, 6.f);
	}
}

void APlayerChara::UpdateCameraDist(const FInputActionValue& Value)
{
	CameraDistanceIndex = FMath::Clamp(CameraDistanceIndex + Value.Get<float>(), 0, CameraDistances.Num() - 1);
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("Camera Distance Index: %d"), CameraDistanceIndex));
	bUpdateCameraPosition = false;
}

// Called to bind functionality to input
void APlayerChara::SetupInput()
{
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}
	}
}

void APlayerChara::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerChara::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerChara::Look);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &APlayerChara::Run);
		EnhancedInputComponent->BindAction(CameraDistAction, ETriggerEvent::Triggered, this, &APlayerChara::UpdateCameraDist);
	}
}

void APlayerChara::ToggleControl()
{
	bIsControllingBall = !bIsControllingBall;

	if (bIsControllingBall)
	{
		// Switch to controlling the ball
		if (PlayerController)
		{
			PlayerController->Possess(Ball);
			Ball->SetupInput();
		}
	}
	else
	{
		// Switch to controlling the player
		if (PlayerController)
		{
			PlayerController->Possess(this);
			SetupInput();
		}
	}
}

void APlayerChara::SwitchClubMessage(AGolfBall* GolfBall)
{
	
	if (ClubClasses.Num() > 0)
	{
		CurrentClubIndex = (CurrentClubIndex + 1) % ClubClasses.Num();
		ClubComponent->SetChildActorClass(ClubClasses[CurrentClubIndex]);
		Club = Cast<AClub>(ClubComponent->GetChildActor());
		if (Club)
		{
			Club->SetActorHiddenInGame(false);
			GolfBall->SetClubAngle(Club->ClubAngle);
		}
	}
}

void APlayerChara::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{

		// Print the movement vector to the screen
		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, FString::Printf(TEXT("Movement Vector: %s"), *MovementVector.ToString()));
		
		AddMovementInput(GetActorForwardVector(), MovementVector.Y * WalkSpeed * UGameplayStatics::GetWorldDeltaSeconds(this));
		AddMovementInput(GetActorRightVector(), MovementVector.X * WalkSpeed * UGameplayStatics::GetWorldDeltaSeconds(this));
	}
}

void APlayerChara::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerPitchInput(LookAxisVector.Y);
		AddControllerYawInput(LookAxisVector.X);
	}
}

void APlayerChara::Run(const FInputActionValue& Value)
{
	const bool bIsRunning = Value.Get<bool>();

	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

// Implement the StartGolfing function
void APlayerChara::StartGolfing(AGolfBall* GolfBall) 
{

	if (PlayerController)
	{
		GolfBall->SetPlayerChara(this);
		
		GetCharacterMovement()->DisableMovement();
		Club->SetActorHiddenInGame(false);
		
		// Switch the camera to the GolfBall's camera with a smooth blend
		PlayerController->SetViewTargetWithBlend(GolfBall, 1.0f, EViewTargetBlendFunction::VTBlend_Cubic, 0.0f, false);

		// Set the player's input to the GolfBall
		PlayerController->Possess(GolfBall);
		GolfBall->SetupInput();
		GolfBall->SetClubAngle(Club->ClubAngle);
		GolfBall->ToggleArrowVisibility(true);

		if (GameMode)
		{
			GameMode->ShowHUD();
		}

	}
}
