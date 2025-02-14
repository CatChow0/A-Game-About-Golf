#include "Widget/BallHud.h"

void UBallHud::UpdateBar(float Value)
{
	if (ProgressBar_0)
	{
		ProgressBar_0->SetPercent(Value);
	}
}