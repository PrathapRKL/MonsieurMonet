// This code belongs to Learning Machines.

#include "MyMM_LureItems.h"
#include "MonsieurMonet_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "MM_LureItems_Base.h"
#include "Components/SphereComponent.h"
#include "Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Components/BillboardComponent.h"

AMyMM_LureItems::AMyMM_LureItems()
{
	PrimaryActorTick.bCanEverTick = false;
	/*LureItemMesh->SetPhysicsLinearVelocity(TossVelocity, false, NAME_None);*/
	CollisionComponentL = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponentL->SetupAttachment(RootComponent);

	LureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lure Item Mesh"));
	LureMesh->SetupAttachment(CollisionComponentL);

	Lure_Name = FString("Please Interact");
	LureHelpText = FString("Please Interact");

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1982.0f;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bSimulationEnabled = true;
	ProjectileMovement->Velocity = FVector(1.0f, 0.0f, 0.5f);
	/*ProjectileMovement->bAutoActivate = true;
	ProjectileMovement->bAutoUpdateTickRegistration = true;
	ProjectileMovement->bAutoRegisterUpdatedComponent = true;*/
}

void AMyMM_LureItems::BeginPlay()
{
	LureHelpText = FString::Printf(TEXT("PleaseInteract %s"), *Lure_Name);
}

void AMyMM_LureItems::LThrow()
{
	/*AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (PlayerCharacter->AddItemToInventory(this))
	{
		OnPickedUp();
	}*/
}

void AMyMM_LureItems::OnPickedUp()
{
	LureMesh->SetVisibility(false);
	LureMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
