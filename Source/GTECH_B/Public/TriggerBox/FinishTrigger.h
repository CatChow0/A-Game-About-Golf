#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"


#include "FinishTrigger.generated.h"

UCLASS()
class GTECH_B_API AFinishTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	AFinishTrigger();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
protected:
	virtual void BeginPlay() override;

private:

	// static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent2;

	// Level to load
	UPROPERTY(EditAnywhere, Category = "Level")
	FName LevelToLoad;
	
};