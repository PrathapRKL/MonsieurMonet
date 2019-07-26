// This code belongs to Learning Machines.

#include "MM_AlarmSystem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Classes/Components/SpotLightComponent.h"
#include "MonsieurMonet_Character.h"
#include "Classes/AkComponent.h"
#include "Classes/AkAudioEvent.h"
#include "Components/TextRenderComponent.h"
#include "Classes/AkGameplayStatics.h"
#include "Classes/Engine/StaticMeshSocket.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/TimelineComponent.h"
#include "Classes/Engine/TextureRenderTarget.h"
#include "Classes/Engine/TextureRenderTarget2D.h"
#include "Classes/Materials/MaterialInstanceDynamic.h"
#include "Components/PrimitiveComponent.h"
#include "ConstructorHelpers.h"
#include "Components/AudioComponent.h"
#include "AI/Swift_GuardCharacter.h"
#include "AI/Regular_GuardCharacter.h"
#include "AI/Regular_GuardController.h"
#include "AI/Brute_GuardCharacter.h"
#include "AI/Brute_GuardController.h"
#include "AI/Swift_GuardController.h"
#include "Components/SceneCaptureComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Classes/AkComponent.h"
#include "Classes/AkAudioEvent.h"
#include "Classes/AkGameplayStatics.h"
#include "Components/SceneComponent.h"
#include "MM_SecuritySystemInterface.h"

// Sets default values
AMM_AlarmSystem::AMM_AlarmSystem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IMM_SecuritySystemInterface* SecSysInterface = Cast<IMM_SecuritySystemInterface>(this->GetClass());

	//Initializing components for the blueprints.
	//Setting up the scene component.
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	//Setting up the alarm system's mesh and point light params.
	AlarmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AlarmMesh"));
	AlarmMesh->SetupAttachment(SceneRoot);

	AlarmLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("AlarmLight"));
	AlarmLight->SetupAttachment(AlarmMesh);

	//Set light color and intensity.
	StandbyLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("StandbyLight"));
	StandbyLight->SetupAttachment(AlarmMesh);
	StandbyLight->SetLightColor(AlarmOffLightColor, true);
	AlarmLight->SetLightColor(AlarmOffLightColor, true);
	AlarmLight->SetIntensity(AlarmLightIntensity);
	AlarmLight->SetAttenuationRadius(AlarmLightAttenuationRadius);
	AlarmLight->SetVolumetricScatteringIntensity(AlarmLightVolumetricScatteringIntensity);

	//Dynamic Material Instance setup.
	AlarmMaterial = UMaterialInstanceDynamic::Create(AlarmMesh->GetMaterial(0), nullptr);
	/*AlarmMaterial = AlarmMesh->CreateDynamicMaterialInstance(0, nullptr, NAME_None);*/

	AlarmAudio = CreateDefaultSubobject<UAkComponent>(TEXT("AlarmAudio"));
	AlarmAudio->SetupAttachment(AlarmMesh);
	AlarmAudio->AkAudioEvent = AlarmAudioEvent;

}

void AMM_AlarmSystem::TripAlarm_Implementation(AActor * NewActor)
{
	AlarmTripped = true;
	ManualAlarm();
}

void AMM_AlarmSystem::ResetAlarm_Implementation()
{
	AlarmTripped = false;
	ManualAlarm();
}

bool AMM_AlarmSystem::SetFailConditions_Implementation(bool FailOnAlarmTripped)
{
	return false;
}

void AMM_AlarmSystem::DisableSecuritySystem_Implementation()
{
	SecuritySystemEnabled = false;
	AlarmTripped = false;
}

void AMM_AlarmSystem::EnableSecuritySystem_Implementation()
{
	SecuritySystemEnabled = true;
	AlarmTripped = false;
	ManualAlarm();
}

// Called when the game starts or when spawned
void AMM_AlarmSystem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMM_AlarmSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TestAlarm == true)
	{
		AlarmLight->SetRelativeRotation(FRotator(AlarmLight->RelativeRotation.Pitch, AlarmLight->RelativeRotation.Yaw + (DeltaTime * LightRotationSpeed), AlarmLight->RelativeRotation.Roll), false, nullptr, ETeleportType::None);
	}
	else
	{
		if (IsGateOpen == true)
		{
			AlarmLight->SetRelativeRotation(FRotator(AlarmLight->RelativeRotation.Pitch, AlarmLight->RelativeRotation.Yaw + (DeltaTime * LightRotationSpeed), AlarmLight->RelativeRotation.Roll), false, nullptr, ETeleportType::None);
		}
	}
}

void AMM_AlarmSystem::ManualAlarm()
{
	float ScalarValue = 0.0f;
	if (AlarmTripped == true)
	{
		SetActorTickEnabled(true);
		AlarmLight->SetVisibility(true, false);
		StandbyLight->SetVisibility(false, false);
		ScalarValue = 1.0f;
		/*AlarmAudio->PostAkEvent(AlarmAudioEvent, "Play_Alarm");*/
		AlarmAudio->SetOutputBusVolume(0.7f);
		IsGateOpen = true;
		AlarmLight->SetLightColor(AlarmOnLightColor, true);
	}
	else if (AlarmTripped == false)
	{
		SetActorTickEnabled(true);
		AlarmLight->SetVisibility(false, false);
		ScalarValue = 0.0f;
		StandbyLight->SetVisibility(true, false);
		/*AlarmAudio->Stop();*/
		IsGateOpen = false;
		StandbyLight->SetLightColor(AlarmOffLightColor, true);
		AlarmLight->SetLightColor(AlarmOffLightColor, true);
	}

	/*AlarmMaterial->SetScalarParameterValue("Alarm State", ScalarValue);*/
}

void AMM_AlarmSystem::ResetAlarmSys()
{
	AlarmTripped = false;
	bHasAlerted = false;
	ManualAlarm();
}

void AMM_AlarmSystem::TripAlarmSys()
{
	AlarmTripped = true;
	bHasAlerted = true;
	ManualAlarm();
}

void AMM_AlarmSystem::DisableSecuritySys()
{
	SecuritySystemEnabled = false;
	AlarmTripped = false;
}

void AMM_AlarmSystem::EnableSecuritySys()
{
	SecuritySystemEnabled = true;
	AlarmTripped = false;
	ManualAlarm();
}

