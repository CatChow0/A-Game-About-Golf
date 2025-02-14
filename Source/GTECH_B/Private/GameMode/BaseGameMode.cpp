#include "GameMode/BaseGameMode.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "Widget/BallHud.h"

AGolfGameMod::AGolfGameMod()
{
	// Set default pawn class to your custom pawn (if any)
	// DefaultPawnClass = AMyPawn::StaticClass();
}

void AGolfGameMod::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass)
	{
		WidgetInstance = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	}

	StartGame();
}

void AGolfGameMod::StartGame()
{
	// Logic to start the game
	UE_LOG(LogTemp, Warning, TEXT("Game Started"));
}

void AGolfGameMod::EndGame()
{
	// Logic to end the game
	UE_LOG(LogTemp, Warning, TEXT("Game Ended"));
}

void AGolfGameMod::ShowHUD()
{
	if (WidgetInstance)
	{
		WidgetInstance->AddToViewport();
		UpdateBar(0);
	}
}

void AGolfGameMod::HideHUD()
{
	if (WidgetInstance)
	{
		WidgetInstance->RemoveFromParent();
	}
}

void AGolfGameMod::UpdateBar(float Value)
{
	if (WidgetInstance)
	{
		UBallHud* BallHud = Cast<UBallHud>(WidgetInstance);
		if (BallHud)
		{
			BallHud->UpdateBar(Value/100);
		}
	}
}
