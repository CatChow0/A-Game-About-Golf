// BasePlayerChara.cpp
#include "Character/BasePlayerChara.h"

// Sets default values
ABasePlayerChara::ABasePlayerChara()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasePlayerChara::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABasePlayerChara::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABasePlayerChara::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}