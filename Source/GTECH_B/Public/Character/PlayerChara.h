// PlayerChara.h

#pragma once

#include "CoreMinimal.h"
#include "Character/BasePlayerChara.h"
#include "Interface/LaPoste.h"



#include "PlayerChara.generated.h"

class AGolfGameMod;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class AGolfBall;
class AClub;

UCLASS()
class GTECH_B_API APlayerChara : public ABasePlayerChara, public ILaPoste
{
	GENERATED_BODY()

public:
	APlayerChara();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetupInput();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CameraDistAction;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void UpdateCamera();
	void UpdateCameraDist(const FInputActionValue& Value);

public:
	virtual void StartGolfing(AGolfBall* GolfBall) override;
	virtual void ShowHUD() override {};
	virtual void HideHUD() override	{};
	virtual void SwitchClubMessage(AGolfBall* GolfBall) override;
	virtual void HandleWaterOverlap() override {};
	virtual void EnterSandOverlap() override {};
	virtual void ExitSandOverlap() override {};

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 900.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TArray<int> CameraDistances;

	int CameraDistanceIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	int CameraDistanceOffset = 850;

	bool bUpdateCameraPosition = true;
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = "Golf Ball", meta = (AllowPrivateAccess = "true"))
	AGolfBall* Ball;

	AGolfGameMod* GameMode;

	UPROPERTY(EditAnywhere, Category = "Club", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ClubComponent;

	AClub* Club;

	UPROPERTY(EditAnywhere, Category = "Club", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AClub>> ClubClasses;

	int CurrentClubIndex = 0;

	APlayerChara* PlayerChara;

	void ToggleControl();

	bool bIsControllingBall;
};