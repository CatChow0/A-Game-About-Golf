#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Pawn.h"
#include "Interface/LaPoste.h"
#include "GolfBall.generated.h"

class APlayerChara;
class AGolfGameMod;
class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;
class USphereComponent;
class UStaticMeshComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class GTECH_B_API AGolfBall : public APawn, public ILaPoste
{
	GENERATED_BODY()

public:
	AGolfBall();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetupInput();

	void SetClubAngle(float Angle) { ClubAngle = Angle; }
	void ToggleArrowVisibility(bool value) const {ArrowComponent->SetVisibility(value);}
	void SetPlayerChara(APlayerChara* Player) { PlayerChara = Player; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* BallContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ChargeAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SwitchClubAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void StartCharging(const FInputActionValue& Value);
	void StopCharging(const FInputActionValue& Value);
	void SwitchClub(const FInputActionValue& Value);

	void ResetRound();
	void EnableFriction();
	void DisableFriction();
	bool IsOnGround() const;

public:

	virtual void StartGolfing(AGolfBall* GolfBall) override { };
	virtual void ShowHUD() override { };
	virtual void HideHUD() override { };
	virtual void SwitchClubMessage(AGolfBall* GolfBall) override { };
	virtual void HandleWaterOverlap() override;
	virtual void EnterSandOverlap() override;
	virtual void ExitSandOverlap() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* TriggerSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed = 100.f;

	UPROPERTY(EditAnywhere, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float MaxForce = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float ChargeRate = 500.f;

	UPROPERTY(EditAnywhere, Category="Shooting", meta= (AllowPrivateAccess = "true"))
	float ForceChargeRateScale = 3.f;

	UPROPERTY(EditAnywhere, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float ForceScale = 1.f;

	UPROPERTY(EditAnywhere, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float FrictionCoefficient = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float SandFrictionCoefficient = 1.4f;

	UPROPERTY(EditAnywhere, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float SandForceScale = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float AirFrictionCoef = 0.1f;

	float FrictionCoef;
	float ForceScal;
	bool AirFriction;
	
	float CurrentForce;
	bool bIsCharging;

	float ClubAngle;

	FRotator InitialRotation;
	FVector LastShotPosition;

	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float MovementThreshold = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Behavior", meta = (AllowPrivateAccess = "true"))
	float InactiveDuration = 2.0f;
	float InactiveTime = 0.0f;
	bool bHasBeenShot = false;

	AGolfGameMod* GameMode;

	APlayerChara* PlayerChara;

	APlayerController* PlayerController;

	bool bIsControllingBall;

	void ToggleControl();
};