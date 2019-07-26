// This code belongs to Learning Machines.

#include "MM_LureItems_Base.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MonsieurMonet_Character.h"
#include "Classes/AkComponent.h"
#include "Classes/AkAudioEvent.h"
#include "Classes/AkGameplayStatics.h"

void AMM_LureItems_Base::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	PickedUpOnTouch(Cast<AMonsieurMonet_Character>(OtherActor));
}

bool AMM_LureItems_Base::CanBePickedUp(AMonsieurMonet_Character * PlayerPawn) const
{
	return PlayerPawn;
}

// Sets default values
AMM_LureItems_Base::AMM_LureItems_Base()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	LureHelpText = FString("Please Interact");

	CollisionComponentL = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComponentL->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	CollisionComponentL->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponentL->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponentL->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = CollisionComponentL;

	LureItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LureItemMesh"));
	LureItemMesh->SetupAttachment(CollisionComponentL);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1982.0f;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bSimulationEnabled = true;
	ProjectileMovement->Velocity = FVector(1.0f, 0.0f, 0.5f);

	bIsActive = false;
	PickedUpBy = nullptr;
	NoiseImpact = 0;

	Name = "Name not set";
	Action = "Interact";
}

// Called when the game starts or when spawned
void AMM_LureItems_Base::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMM_LureItems_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMM_LureItems_Base::PickedUpOnTouch(AMonsieurMonet_Character * Pawn)
{
	if (bIsActive && Pawn && !IsPendingKill())
	{
		if (CanBePickedUp(Pawn))
		{
			PickedUpBy = Pawn;

			if (!IsPendingKill())
			{
				bIsActive = false;
				OnPickedUp();
			}
		}
	}
}

void AMM_LureItems_Base::OnPickedUp()
{
	if (PickedUpSound)
	{
		UAkGameplayStatics::SpawnAkComponentAtLocation(GetWorld(), PickedUpSound, nullptr, GetActorLocation(), GetActorRotation(), true, "", "", true);
	}
	OnPickedupEvent();
	GetWorld()->DestroyActor(this);
	MakeNoiseEve(NoiseImpact, PickedUpBy);
}

void AMM_LureItems_Base::LThrow()
{

}

void AMM_LureItems_Base::MakeNoiseEve(int32 NoiseAmount, APawn * Pawn)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(Pawn);
	if (PlayerCharacter != nullptr)
	{
		NoiseAmount = NoiseImpact;
	}
}

void AMM_LureItems_Base::Interact(AMonsieurMonet_Character* PlayerCharacter)
{

}