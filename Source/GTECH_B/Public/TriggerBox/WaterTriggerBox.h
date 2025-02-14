#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Interface/LaPoste.h"
#include "WaterTriggerBox.generated.h"

UCLASS()
class GTECH_B_API AWaterTriggerBox : public ATriggerBox, public ILaPoste
{
	GENERATED_BODY()

public:
	AWaterTriggerBox();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void StartGolfing(AGolfBall* GolfBall) override { };
	virtual void ShowHUD() override { };
	virtual void HideHUD() override { };
	virtual void SwitchClubMessage(AGolfBall* GolfBall) override { };
	virtual void HandleWaterOverlap() override { };
	virtual void EnterSandOverlap() override { };
	virtual void ExitSandOverlap() override { };
	
};