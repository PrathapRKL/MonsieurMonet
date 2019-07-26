// This code belongs to Learning Machines.

#include "MM_FreezeProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/Classes/GameFramework/Character.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "AI/Regular_GuardController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI/Regular_GuardCharacter.h"
#include "AI/MM_PatrolPoints.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "MonsieurMonet/MonsieurMonet_Character.h"


// Sets default values
AMM_FreezeProjectile::AMM_FreezeProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionComponent->SetMobility(EComponentMobility::Movable);

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	Projectile->SetupAttachment(CollisionComponent);
	Projectile->SetMobility(EComponentMobility::Movable);
	Projectile->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMesh(TEXT("/Game/Projectile/Projectile_Freeze"));
	if (ProjectileMesh.Object)
		Projectile->SetStaticMesh(ProjectileMesh.Object);*/

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->InitialSpeed = 3000.0f;
	ProjectileComponent->MaxSpeed = 3000.0f;
	ProjectileComponent->bRotationFollowsVelocity = false;
	ProjectileComponent->bInitialVelocityInLocalSpace = true;
	ProjectileComponent->bShouldBounce = false;
	ProjectileComponent->bForceSubStepping = false;
	ProjectileComponent->bSimulationEnabled = true;
	ProjectileComponent->MaxSimulationTimeStep = 0.05f;
	ProjectileComponent->MaxSimulationIterations = 4;
	ProjectileComponent->bInterpMovement = false;
	ProjectileComponent->bInterpRotation = false;
	ProjectileComponent->InterpLocationTime = 0.1f;
	ProjectileComponent->InterpRotationTime = 0.05f;
	ProjectileComponent->InterpLocationMaxLagDistance = 300.0f;
	ProjectileComponent->InterpLocationSnapToTargetDistance = 500.0f;
	ProjectileComponent->Velocity = FVector(3000.0f, 0.0f, 0.0f);

	bCanBeDamaged = true;
	InitialLifeSpan = 2.75f;
}

// Called when the game starts or when spawned
void AMM_FreezeProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (CollisionComponent)
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMM_FreezeProjectile::OnComponentHit);
}

// Called every frame
void AMM_FreezeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMM_FreezeProjectile::EventHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool SelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, FHitResult Hit)
{
	ARegular_GuardCharacter* RegularGuard = Cast<ARegular_GuardCharacter>(Other);
	RegularGuard->Frozen();
	FVector ActorLocation = GetActorLocation();
	if (OtherComp->IsSimulatingPhysics(NAME_None) == true)
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 1000.0f, ActorLocation, NAME_None);
		Destroy(true);
	}
}

void AMM_FreezeProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	//AActor* HitActor = OtherActor;
	//FVector HitLocation = Hit.Location;
	//FVector ImpactNormal = Hit.ImpactNormal;
	//FName HitBoneName = Hit.BoneName;
	ARegular_GuardCharacter* RegularGuard = Cast<ARegular_GuardCharacter>(OtherActor);
	if (OtherActor == RegularGuard && RegularGuard != nullptr)
	{
		RegularGuard->Frozen();
		Destroy(true);
	}
	//else
	//{
	//	/*Destroy(false);*/
	//	return;
	//}
}

