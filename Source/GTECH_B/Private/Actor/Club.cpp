// Club.cpp

#include "Actor/Club.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AClub::AClub()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and configure the static mesh component
	ClubMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClubMesh"));
	RootComponent = ClubMesh;

	// Initialize the club angle
	ClubAngle = 0.0f;
}

// Called when the game starts or when spawned
void AClub::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AClub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}