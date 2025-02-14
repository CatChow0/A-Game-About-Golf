#include "TriggerBox/WaterTriggerBox.h"
#include "Actor/GolfBall.h"
#include "Components/BoxComponent.h"

AWaterTriggerBox::AWaterTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &AWaterTriggerBox::OnOverlapBegin);
}

void AWaterTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void AWaterTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	AGolfBall* GolfBall = Cast<AGolfBall>(OtherActor);
	if (GolfBall)
	{
		// Call a method on the GolfBall
		GolfBall->HandleWaterOverlap();
	}
}