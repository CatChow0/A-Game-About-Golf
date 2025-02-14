#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LaPoste.generated.h"

class AGolfBall;

UINTERFACE(MinimalAPI)
class ULaPoste : public UInterface
{
	GENERATED_BODY()
};

class GTECH_B_API ILaPoste
{
	GENERATED_BODY()

public:
	// Func to start golfing
	virtual void StartGolfing(AGolfBall* GolfBall) = 0;

	// Func to show HUD
	virtual void ShowHUD() = 0;

	// Func to hide HUD
	virtual void HideHUD() = 0;

	virtual void SwitchClubMessage(AGolfBall* GolfBall) = 0;

	virtual void HandleWaterOverlap() = 0;

	virtual void EnterSandOverlap() = 0;

	virtual void ExitSandOverlap() = 0;
};