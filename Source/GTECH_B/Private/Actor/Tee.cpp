// Tee.cpp

#include "Actor/Tee.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "Actor/GolfBall.h"
#include "Character/PlayerChara.h"
#include "Interface/LaPoste.h"

// Sets default values
ATee::ATee()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create and configure the static mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;

	// Create and configure the spawn point component
	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);

	// Create and configure the detection zone component
	DetectionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectionZone"));
	DetectionZone->SetupAttachment(RootComponent);
	DetectionZone->SetCollisionProfileName(TEXT("Trigger"));
	DetectionZone->OnComponentBeginOverlap.AddDynamic(this, &ATee::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ATee::BeginPlay()
{
	Super::BeginPlay();

	if (GolfBallClass)
	{
		FVector SpawnLocation = SpawnPoint->GetComponentLocation();
		SpawnGolfBall = GetWorld()->SpawnActor<AGolfBall>(GolfBallClass, SpawnLocation, GolfBallRotation);
	}
}

// Called every frame
void ATee::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Handle overlap events
void ATee::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerChara* PlayerChara = Cast<APlayerChara>(OtherActor);
	if (PlayerChara)
	{
		ILaPoste* LaPosteInterface = Cast<ILaPoste>(PlayerChara);
		if (LaPosteInterface && SpawnGolfBall)
		{
			LaPosteInterface->StartGolfing(SpawnGolfBall);
		}
	}
}