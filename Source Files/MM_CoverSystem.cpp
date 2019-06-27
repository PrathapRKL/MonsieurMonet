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

}

void AMM_CoverSystem::OnCompEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMonsieurMonet_Character>())
	{
		//Inform the player that he isn't capable of taking cover.
		AMonsieurMonet_Character* Character = Cast<AMonsieurMonet_Character>(OtherActor);
		Character->bCanTakeCover = false;
		Character->bIsInCover = false;
	}
}

void AMM_CoverSystem::OnCompBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA<AMonsieurMonet_Character>())
	{
		//Inform the player that he is capable of taking cover.
		AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
		PlayerCharacter->bCanTakeCover = true;
	}
}

