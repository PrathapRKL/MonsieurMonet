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
#include "Components/TextRenderComponent.h"

// Sets default values
AMM_HidingSpots::AMM_HidingSpots()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	HidingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HidingSppotMesh"));
	HidingMesh->SetupAttachment(CollisionComponent);

	//Hiding Spot camera to set up view blend targets.
	HSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HidingSpotCamera"));
	HSCamera->SetupAttachment(CollisionComponent);

	InteractText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("InteractText"));
	InteractText->SetupAttachment(HidingMesh);
	InteractText->SetVisibility(false, false);
}

// Called when the game starts or when spawned
void AMM_HidingSpots::BeginPlay()
{
	Super::BeginPlay();
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMM_HidingSpots::OnBeginOverlap_HS);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMM_HidingSpots::OnEndOverlap_HS);

	//Set location var for the player to use when hiding.
	PlayerHidingLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 10.0f);
}

// Called every frame
void AMM_HidingSpots::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}

	if (IsInVolume == true)
	{
		if (IsTriggering == true)
		{
			AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			HSCamera->SetRelativeRotation(FRotator(PlayerCharacter->CamPitch, PlayerCharacter->CamYaw, 0.0f));

			if (isCurrentlyHid == false)
			{
				HidePlayer();
				isCurrentlyHid = true;
			}
		}
		if (IsTriggering_1 == true && isCurrentlyHid == true)
		{
			UnHidePlayer();
			APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			PlayerController->SetViewTargetWithBlend(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), 0.2f, EViewTargetBlendFunction::VTBlend_Linear, 4.6f, false);
			isCurrentlyHid = false;
		}
	}
}

void AMM_HidingSpots::OnBeginOverlap_HS(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter)
	{
		if (OtherComp == PlayerCharacter->GetCapsuleComponent())
		{
			InitialLocation = FVector((PlayerCharacter->GetActorLocation().X - 130.0f), PlayerCharacter->GetActorLocation().Y, (PlayerCharacter->GetActorLocation().Z - 60.0f));
			PlayerCharacter->IsInteracting = true;
			IsInVolume = true;
			InteractText->SetVisibility(true, false);
			InteractText->SetRelativeLocation(FVector((PlayerCharacter->GetActorForwardVector().X + 20.0f), (PlayerCharacter->GetActorForwardVector().Y + 20.0f), InteractText->RelativeRotation.Yaw));
			InteractText->SetRelativeRotation(FRotator(0.0f, 0.0f, PlayerCharacter->GetActorRotation().Yaw + 0.0f));
		}
	}
}

void AMM_HidingSpots::OnEndOverlap_HS(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter)
	{
		if (OtherComp == PlayerCharacter->GetCapsuleComponent())
		{
			PlayerCharacter->IsInteracting = false;
			IsInVolume = false;
			IsTriggering = false;
			IsTriggering_1 = false;
			InteractText->SetVisibility(false, false);
		}
	}
}

void AMM_HidingSpots::OnInteract()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		PlayerController->SetViewTargetWithBlend(this, 0.2f, EViewTargetBlendFunction::VTBlend_Linear, 4.6f, false);
		HSCamera->bUsePawnControlRotation = true;
		OnInteractEvent();
		//PlayerCharacter->IsHiding = false;
	}
}

void AMM_HidingSpots::HidePlayer()
{
	if (CanHide == false)
	{
		AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		PlayerCharacter->IsHiding = true;
		PlayerCharacter->GetCharacterMovement()->DisableMovement();
		OnInteract();
		PlayerCharacter->SetActorLocation(PlayerHidingLocation, false, nullptr, ETeleportType::None);
		/*HidingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);*/
		PlayerCharacter->SetActorHiddenInGame(true);
		PlayerCharacter->canAimCPP = false;
		PlayerCharacter->NoiseParticle_Idle(PlayerCharacter);
		PlayerCharacter->NoiseRadiusComp->SetSphereRadius(0.0f, true);
	}
}

void AMM_HidingSpots::UnHidePlayer()
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->IsHiding = false;
	PlayerCharacter->SetActorLocation(InitialLocation, true, nullptr, ETeleportType::None);
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking, 0);
	/*HidingMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);*/
	PlayerCharacter->SetActorHiddenInGame(false);
	PlayerCharacter->canAimCPP = true;

}

void AMM_HidingSpots::OnConstruction(const FTransform & Transform)
{
	//User Construction Script properties.
	//Set Camera Location.
	CameraLocation = FVector(CameraLocation.X, CameraLocation.Y, CameraLocation.Z);
	HSCamera->SetRelativeLocation(CameraLocation, false, nullptr, ETeleportType::None);

	//Set Camera Rotation.
	CameraRotation = FRotator(CameraRotation.Pitch, CameraRotation.Yaw, 0.0f);
	CameraRotation = FRotator(CameraRotation.Pitch, CameraRotation.Yaw, CameraRotation.Roll);
	HSCamera->SetRelativeRotation(CameraRotation, false, nullptr, ETeleportType::None);

	//Set Camera properties.
	CameraFOV = 90.0f;
	HSCamera->SetFieldOfView(CameraFOV);
}



