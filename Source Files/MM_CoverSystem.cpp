// This code belongs to Learning Machines.

#include "MM_CoverSystem.h"
#include "MonsieurMonet_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Engine/GameEngine.h"
#include "Components/BillboardComponent.h"
#include "Components/TextRenderComponent.h"


// Sets default values
AMM_CoverSystem::AMM_CoverSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	RootComponent = ArrowComponent;

	CoverTriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CoverTriggerVolume"));
	CoverTriggerVolume->SetupAttachment(ArrowComponent);
	CoverTriggerVolume->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);

	//Create and attach the billboard components to the end of the trigger volume.
	CoverEndPoint1 = CreateDefaultSubobject<UBillboardComponent>(TEXT("CoverEndPoint1"));
	CoverEndPoint1->SetupAttachment(CoverTriggerVolume);
	CoverEndPoint2 = CreateDefaultSubobject<UBillboardComponent>(TEXT("CoverEndPoint2"));
	CoverEndPoint2->SetupAttachment(CoverTriggerVolume);
	CoverMidPoint = CreateDefaultSubobject<UBillboardComponent>(TEXT("CoverMidPoint"));
	CoverMidPoint->SetupAttachment(CoverTriggerVolume);

	//Collision boxes to black the player movement upon covering.
	BlockingVol_Left = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingVolume_Left"));
	BlockingVol_Left->SetupAttachment(CoverTriggerVolume);
	BlockingVol_Right = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingVolume_Right"));
	BlockingVol_Right->SetupAttachment(CoverTriggerVolume);
	BlockingVolume_Front = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingVolume_Front"));
	BlockingVolume_Front->SetupAttachment(CoverTriggerVolume);

	//Hiding Spot camera to set up view blend targets.
	CoverCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CoverCamera"));
	CoverCamera->SetupAttachment(ArrowComponent);

	InteractText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("InteractText"));
	InteractText->SetupAttachment(ArrowComponent);
	InteractText->SetVisibility(false, false);
}

// Called when the game starts or when spawned
void AMM_CoverSystem::BeginPlay()
{
	Super::BeginPlay();

	if (CoverTriggerVolume)
	{
		CoverTriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AMM_CoverSystem::OnCompBeginOverlap);
		CoverTriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AMM_CoverSystem::OnCompEndOverlap);
	}
}

// Called every frame
void AMM_CoverSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter == nullptr)
	{
		return;
	}

	if (PlayerCharacter->bIsInCover == true)
	{
		//CoverCamera->SetRelativeRotation(FRotator(PlayerCharacter->CamPitch, PlayerCharacter->CamYaw + 180.0f, 0.0f));
	}
	else
	{
		/*APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		PlayerController->SetViewTargetWithBlend(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0), 0.73f, EViewTargetBlendFunction::VTBlend_EaseInOut, 4.6f, false);*/
	}
}

void AMM_CoverSystem::OnCompEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter)
	{
		if (OtherComp == PlayerCharacter->GetCapsuleComponent())
		{
			//Inform the player that he isn't capable of taking cover.
			AMonsieurMonet_Character* Character = Cast<AMonsieurMonet_Character>(OtherActor);
			Character->bCanTakeCover = false;
			Character->bIsInCover = false;
			InteractText->SetVisibility(false, false);
		}
	}
}

void AMM_CoverSystem::OnCompBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter)
	{
		if (OtherComp == PlayerCharacter->GetCapsuleComponent())
		{
			//Inform the player that he is capable of taking cover.
			PlayerCharacter->bCanTakeCover = true;
			/*InteractText->SetVisibility(true, true);
			InteractText->SetRelativeLocation(FVector((PlayerCharacter->GetActorForwardVector().X + 20.0f), (PlayerCharacter->GetActorForwardVector().Y), InteractText->RelativeLocation.Z));
			InteractText->SetRelativeRotation(FRotator(0.0f, (ArrowComponent->RelativeRotation.Yaw + 180), 0.0f));*/
		}
	}
}

void AMM_CoverSystem::OnConstruction(const FTransform & Transform)
{
	//User Construction Script properties.
	//Set Camera Location.
	CameraLocation = FVector(CameraLocation.X, CameraLocation.Y, CameraLocation.Z);
	CoverCamera->SetRelativeLocation(CameraLocation);

	//Set Camera Rotation.
	CameraRotation = FRotator(CameraRotation.Pitch, CameraRotation.Yaw, 0.0f);
	CameraRotation = FRotator(CameraRotation.Pitch, CameraRotation.Yaw, CoverCamera->RelativeRotation.Roll);
	CoverCamera->SetRelativeRotation(CameraRotation);

	//Set Camera properties.
	CameraFOV = 90.0f;
	CoverCamera->SetFieldOfView(CameraFOV);
}

void AMM_CoverSystem::OnInteract()
{
	CoverCamera->bUsePawnControlRotation = true;
	APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	PlayerController->SetViewTargetWithBlend(this, 0.73f, EViewTargetBlendFunction::VTBlend_EaseInOut, 4.6f, false);
	PlayerCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

