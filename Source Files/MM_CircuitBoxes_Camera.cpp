// This code belongs to Learning Machines.

#include "MM_CircuitBoxes_Camera.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MonsieurMonet/MonsieurMonet_Character.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMM_CircuitBoxes_Camera::AMM_CircuitBoxes_Camera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = Scene;

	CBMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CircuitBoxMesh"));
	CBMesh->SetupAttachment(Scene);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComp->SetupAttachment(CBMesh);
}

// Called when the game starts or when spawned
void AMM_CircuitBoxes_Camera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMM_CircuitBoxes_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMM_CircuitBoxes_Camera::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter && PlayerCharacter != nullptr)
	{
		PlayerCharacter->bTriggered = true;
	}
	else
	{
		return;
	}
}

void AMM_CircuitBoxes_Camera::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter && PlayerCharacter != nullptr)
	{
		PlayerCharacter->CanTakeDown = false;
		PlayerCharacter->IsAttacking = false;
		PlayerCharacter->bTriggered = false;
	}
	else
	{
		return;
	}
}

