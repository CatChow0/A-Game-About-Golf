#include "TriggerBox/SandTriggerBox.h"
#include "Actor/GolfBall.h"
#include "Components/BoxComponent.h"

ASandTriggerBox::ASandTriggerBox()
{
	OnActorBeginOverlap.AddDynamic(this, &ASandTriggerBox::OnOverlapBegin);
}

void ASandTriggerBox::BeginPlay()
{
	Super::BeginPlay();
}

void ASandTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	AGolfBall* GolfBall = Cast<AGolfBall>(OtherActor);
	if (GolfBall)
	{
		// Call a method on the GolfBall
		GolfBall->EnterSandOverlap();
	}
}

void ASandTriggerBox::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	AGolfBall* GolfBall = Cast<AGolfBall>(OtherActor);
	if (GolfBall)
	{
		// Call a method on the GolfBall
		GolfBall->ExitSandOverlap();
	}
}