#include "Pawns/BasePawn.h"
#include "Pawns/Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
// #include "BPI_LaPoste.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base mesh"));
	BaseMesh->SetupAttachment(BoxComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}

void ABasePawn::BeginPlay()
{
	Super::BeginPlay();

	 // if (GetClass()->ImplementsInterface(UBPI_LaPoste::StaticClass()))
	 // {
	 // 	bHasBPI_LaPoste = true;
	 // }
}

void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation =  FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(),
			LookAtRotation,
			UGameplayStatics::GetWorldDeltaSeconds(this),
			InterpSpeed));
}

void ABasePawn::Fire()
{
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("Fire"));
	FVector ProjectileSpawnPointLocation = ProjectileSpawnPoint->GetComponentLocation();
	FRotator ProjectileSpawnPointRotation = ProjectileSpawnPoint->GetComponentRotation();
	FVector scale = ProjectileScale;

	FTransform SpawnTransform(ProjectileSpawnPointRotation, ProjectileSpawnPointLocation, scale);

	if (ProjectileClass != nullptr)
	{
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnTransform);
		Projectile->SetOwner(this);
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, ProjectileSpawnPointLocation);
	}
}