// Tee.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tee.generated.h"

class AGolfBall;
class UBoxComponent;

UCLASS()
class GTECH_B_API ATee : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATee();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	// Scene component to mark the spawn point
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpawnPoint;

	// Box component for detection zone
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* DetectionZone;

	// GolfBall class to spawn
	UPROPERTY(EditAnywhere, Category = "Golf Ball")
	TSubclassOf<AGolfBall> GolfBallClass;

	// Golf ball spawned at the spawn point Reference
	AGolfBall* SpawnGolfBall;

	// Golf ball rotation
	UPROPERTY(EditAnywhere, Category = "Golf Ball")
	FRotator GolfBallRotation = FRotator(0.f, 0.f, 0.f);

	// Function to handle overlap events
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};