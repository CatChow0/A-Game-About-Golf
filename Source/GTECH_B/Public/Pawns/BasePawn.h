#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

// LAST INCLUDE 
#include "BasePawn.generated.h"

// FORWARD DECLARATION
class UBoxComponent;
class AProjectile;

UCLASS()
class GTECH_B_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	ABasePawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void RotateTurret(FVector LookAtTarget);

	void Fire();

	UPROPERTY(EditAnywhere, Category = "Canon Properties")
	float InterpSpeed = 5.f;
	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FVector ProjectileScale = FVector(2.f, 2.f, 2.f);

	// Sound effect when the pawn fires
	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* FireSound;
	
	bool bHasBPI_LaPoste = false;
};
