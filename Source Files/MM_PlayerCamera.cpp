// This code belongs to Learning Machines.

#include "MM_PlayerCamera.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "MonsieurMonet_Character.h"

// Sets default values
AMM_PlayerCamera::AMM_PlayerCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create a camera component and set-up its initial properties.
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	PlayerContCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerContCamera->SetupAttachment(Scene);
	PlayerContCamera->bUsePawnControlRotation = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void AMM_PlayerCamera::BeginPlay()
{
	Super::BeginPlay();
	
	/* Delegates */
	//Delegate function for OnComponentBeginOverlap.
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AMM_PlayerCamera::OnOverlapBegin);
	//Delegate function for OnComponentEndOverlap.
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &AMM_PlayerCamera::OnOverlapEnd);
}

// Called every frame
void AMM_PlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMM_PlayerCamera::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult & SweepResult)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(OtherActor);
	if (OtherActor == PlayerCharacter)
	{
		APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		PlayerController->SetViewTargetWithBlend(this, 1.0f, VTBlend_EaseInOut, 4.6f, false);
	}
}

void AMM_PlayerCamera::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{

}

