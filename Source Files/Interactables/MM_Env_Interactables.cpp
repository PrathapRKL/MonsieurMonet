// This code belongs to Learning Machines.

#include "MM_Env_Interactables.h"
#include "MonsieurMonet_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "MM_LureItems_Base.h"
#include "Components/BoxComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Components/BillboardComponent.h"
#include "Classes/AkComponent.h"
#include "Classes/AkAudioEvent.h"
#include "Classes/AkGameplayStatics.h"
#include "AI/Swift_GuardCharacter.h"
#include "AI/Brute_GuardCharacter.h"
#include "AI/Regular_GuardCharacter.h"

// Sets default values
AMM_Env_Interactables::AMM_Env_Interactables()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponentE = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponentE->SetupAttachment(RootComponent);

	EnvMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnvironmentInterMesh"));
	EnvMesh->SetupAttachment(CollisionComponentE);

	EnvHelpText = FString("Please Interact");

	/*OnInteracted.AddDynamic(this, &AMM_Env_Interactables::OnInteractEvent);*/
}

// Called when the game starts or when spawned
void AMM_Env_Interactables::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMM_Env_Interactables::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMM_Env_Interactables::OnInteract()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		OnInteracted.Broadcast(this);
		if (InteractSound)
			InteractableEvent = UAkGameplayStatics::SpawnAkComponentAtLocation(GetWorld(), InteractSound, nullptr, GetActorLocation(), GetActorRotation(), true, "", "", true);
		IsMakingNoise = true;
		OnInteractEvent();
		//Raytrace to find and alert the nearest enemy.
		/*FVector StartLocation = GetActorLocation();
		FVector EndLocation;
		FHitResult OutHits;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FCollisionObjectQueryParams EICQ;
		EICQ.AllDynamicObjects;
		FCollisionShape SCollisionShape = FCollisionShape::MakeSphere(SearchRadius);

		World->SweepSingleByObjectType(OutHits, StartLocation, EndLocation, FQuat::Identity, EICQ, SCollisionShape, QueryParams);
		ASwift_GuardCharacter* SwiftGuard = Cast<ASwift_GuardCharacter>(OutHits.Actor);
		ABrute_GuardCharacter* BruteGuard = Cast<ABrute_GuardCharacter>(OutHits.Actor);
		ARegular_GuardCharacter* RegularGuard = Cast<ARegular_GuardCharacter>(OutHits.Actor);
		if (OutHits.Actor == SwiftGuard || OutHits.Actor == RegularGuard || OutHits.Actor == BruteGuard)
		{
			 
		}*/
	}
}

void AMM_Env_Interactables::OnBeginOverlap_Env()
{
}

