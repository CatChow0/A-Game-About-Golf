// Club.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Club.generated.h"

UCLASS()
class GTECH_B_API AClub : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AClub();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Public variable to store the angle of the club
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Club")
	float ClubAngle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Static mesh component to represent the club
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ClubMesh;
};