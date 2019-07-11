// This code belongs to Learning Machines.

#include "MM_Pickups.h"
#include "MM_Interactables_Base.h"
#include "MonsieurMonet_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

AMM_Pickups::AMM_Pickups()
{
	PrimaryActorTick.bCanEverTick = false;
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));

	Pickup_Name = FString("Please Interact");
	Pickup_Value = 0;
}

void AMM_Pickups::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT("PleaseInteract %s"), *Pickup_Name);
}

void AMM_Pickups::PUInteract_Implementation()
{
	AMonsieurMonet_Character* MonsieurMonet = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
		OnPickedUp();
}

void AMM_Pickups::PUDiscard_Implementation()
{
	InteractableMesh->DestroyComponent();
	Pickup_Thumbnail = nullptr;
	Pickup_Value = 00;
}

void AMM_Pickups::OnPickedUp()
{
	InteractableMesh->SetVisibility(false);
	InteractableMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnPickedupEvent();
}
