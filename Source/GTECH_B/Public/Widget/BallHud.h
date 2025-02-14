#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "BallHud.generated.h"

UCLASS()
class GTECH_B_API UBallHud : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateBar(float Value);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBar_0;
};