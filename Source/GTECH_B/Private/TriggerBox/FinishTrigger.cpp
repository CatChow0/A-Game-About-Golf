#include "TriggerBox/FinishTrigger.h"
#include "Actor/GolfBall.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AFinishTrigger::AFinishTrigger()
{
	OnActorBeginOverlap.AddDynamic(this, &AFinishTrigger::OnOverlapBegin);
	
	// Create and configure the static mesh component
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	// Create and configure the static mesh component
	MeshComponent2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent2"));
	MeshComponent2->SetupAttachment(RootComponent);
	MeshComponent2->SetCollisionProfileName(TEXT("BlockAll"));
}

void AFinishTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AFinishTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	AGolfBall* GolfBall = Cast<AGolfBall>(OtherActor);
	if (GolfBall)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
	}
}