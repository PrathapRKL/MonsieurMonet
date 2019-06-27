// This code belongs to Learning Machines.

#include "MM_HidingSpots.h"
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

// Sets default values
AMM_HidingSpots::AMM_HidingSpots()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);

	HidingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HidingSppotMesh"));
	HidingMesh->SetupAttachment(CollisionComponent);
}

// Called when the game starts or when spawned
void AMM_HidingSpots::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMM_HidingSpots::OnBeginOverlap_HS);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMM_HidingSpots::OnEndOverlap_HS);
}

// Called every frame
void AMM_HidingSpots::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMM_HidingSpots::OnBeginOverlap_HS(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter)
	{
		PlayerCharacter->IsInteracting = true;
	}
}

void AMM_HidingSpots::OnEndOverlap_HS(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter)
	{
		PlayerCharacter->IsInteracting = false;
	}
}

void AMM_HidingSpots::OnInteract()
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		OnInteractEvent();
}



