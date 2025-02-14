#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Interface/LaPoste.h"
#include "BaseGameMode.generated.h"

class AGolfBall;

UCLASS()
class GTECH_B_API AGolfGameMod : public AGameModeBase, public ILaPoste
{
	GENERATED_BODY()

public:
	AGolfGameMod();

	void UpdateBar(float Value);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> WidgetClass;

private:
	UUserWidget* WidgetInstance;
	

	void StartGame();
	void EndGame();

public:
	virtual void ShowHUD() override;
	virtual void HideHUD() override;
	virtual void StartGolfing(AGolfBall* GolfBall) override { };
	virtual void SwitchClubMessage(AGolfBall* GolfBall) override { };
	virtual void HandleWaterOverlap() override { };
	virtual void EnterSandOverlap() override { };
	virtual void ExitSandOverlap() override { };
};