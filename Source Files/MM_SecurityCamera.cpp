// This code belongs to Learning Machines.

#include "MM_SecurityCamera.h"
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

// Sets default values
AMM_SecurityCamera::AMM_SecurityCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	RootComponent = ArrowComponent;

	Base_CamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera_Base"));
	Base_CamMesh->SetupAttachment(ArrowComponent);
	Main_CamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera_MainMesh"));
	Main_CamMesh->SetupAttachment(Base_CamMesh);

	CameraSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	CameraSpotLight->SetupAttachment(Main_CamMesh);

	LightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ViewCone"));
	LightMesh->SetupAttachment(CameraSpotLight);
	LightMesh->SetVisibility(false, false);
	LightMesh->OnComponentBeginOverlap.AddDynamic(this, &AMM_SecurityCamera::OnOverlapBegin);
	LightMesh->OnComponentEndOverlap.AddDynamic(this, &AMM_SecurityCamera::OnOverlapEnd);

	AudioScanner = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioScanner"));
	AudioScanner->SetupAttachment(LightMesh);

	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture->SetupAttachment(Main_CamMesh);

	//Construction Script.
	SceneCapture->MaxViewDistanceOverride = SceneCaptureViewDistance;
	CameraSpotLight->SetIntensity(SpotLightBrightness);
	CameraSpotLight->SetLightColor(LightColorAlarmOff, true);
	CameraSpotLight->SetAttenuationRadius(Range);
	CameraSpotLight->SetOuterConeAngle(ConeAngle);
	CameraSpotLight->SetInnerConeAngle(ConeAngle * 0.7f);

	//Compute cone end point radius.
	float ConeEndRadusScaled = EndScaleModifier * ((Range * FMath::Tan(ConeAngle)) / ConeEndRadiusUnscaled);
	ConeMeshScaleXY = ConeEndRadusScaled;

	//Compute cone length.
	float ConeLengthScaled = (Range / ConeLengthUnscaled) * LengthScaleModifier;
	ConeMeshScaleZ = ConeLengthScaled;
	LightMesh->SetRelativeScale3D(FVector(ConeMeshScaleXY, ConeMeshScaleXY, ConeMeshScaleZ));
	CameraSpotLight->SetVolumetricScatteringIntensity(VolumetricScatteringIntensity);

	//Pitch and Yaw Constraints for the camera.
	float LerpValue = FMath::LerpStable(CameraMaxYaw * (-1), CameraMaxYaw, TestYaw);
	FRotator CameraPY = FRotator(0.0f, CameraPitch, LerpValue);
	Main_CamMesh->SetRelativeRotation(CameraPY);
	DefaultRotation = Main_CamMesh->GetComponentRotation();

	//Set-up dynamic material instances for camera and the viewcone mesh.
	UMaterialInterface* Mat_View = LightMesh->GetMaterial(0);
	ViewConeMaterial = LightMesh->CreateDynamicMaterialInstance(0, Mat_View);

	if (ViewConeMaterial != nullptr)
	{
		ViewConeMaterial->SetScalarParameterValue("Intensity", ViewConeVisibility);
	}
	UMaterialInterface* Mat_Lens = Main_CamMesh->GetMaterial(0);
	LensMaterial = Main_CamMesh->CreateDynamicMaterialInstance(0, Mat_Lens);

	//Timeline Component.
	Timelinebegin = false;
	ReadyState = true;
	static ConstructorHelpers::FObjectFinder<UCurveFloat>Curve(TEXT("/Game/Curves/C_CFloatAlpha_SC"));
	check(Curve.Succeeded());
	EnDisCurve = Curve.Object;
}

// Called when the game starts or when spawned
void AMM_SecurityCamera::BeginPlay()
{
	Super::BeginPlay();

	TimelineValue = 16.0f;
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishedCallback;
	if (EnDisCurve != nullptr)
	{
		SecCamTimeline = NewObject<UTimelineComponent>(this, FName("CameraRotation"));
		SecCamTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		this->BlueprintCreatedComponents.Add(SecCamTimeline);
		SecCamTimeline->SetNetAddressable();
		SecCamTimeline->SetPropertySetObject(this);
		SecCamTimeline->SetDirectionPropertyName(FName("ScanRotation"));
		SecCamTimeline->SetLooping(true);
		SecCamTimeline->SetTimelineLength(16.0f);
		SecCamTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
		SecCamTimeline->SetPlaybackPosition(0.0f, false);
		SecCamTimeline->AddInterpFloat(EnDisCurve, TimelineCallback);
		SecCamTimeline->RegisterComponent();
		SecCamTimeline->SetAutoActivate(true);

		//Timeline events.
		TimelineCallback.BindUFunction(this, FName{ TEXT("TimelineCallback") });
		TimelineFinishedCallback.BindUFunction(this, FName{ TEXT("TimelineFinishedCallback") });
	}
}

// Called every frame
void AMM_SecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SecCamTimeline != nullptr)
	{
		SecCamTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, NULL);
	}

	if (Enabled == true)
	{
		int TickCount = 0;
		PlayCameraScanningAnimation();
		CheckVisibilityToOverlapActor(PotentialTarget, HasLOS);

		if (HasLOS && WithinCone)
		{
			TickCount = 1;
			if (TickCount == 1)
			{
				Target = PotentialTarget;
			}
		}
		else
		{
			TickCount = 0;
		}
	}
}

bool AMM_SecurityCamera::CheckOverlapActorisTargettable(AActor * Actor, bool DesiredTarget)
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(Actor);
	bool DesiredTargetLocal = false;
	if (PlayerCharacter->GetClass() == TargettedCharacterClass->GetClass())
	{
		DesiredTargetLocal = true;
	}
	DesiredTarget = DesiredTargetLocal;
	return DesiredTarget;
}

bool AMM_SecurityCamera::CheckVisibilityToOverlapActor(AActor * Actor, bool HasLOStoTarget)
{
	AActor* ActorLocal = Cast<AActor>(Actor);
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(ActorLocal);
	USceneComponent* Mesh = PlayerCharacter->GetMesh();
	FVector Loc;
	FVector StartLoc = CameraSpotLight->GetComponentLocation();
	bool HasLOStoTargetLocal = false;
	FCollisionQueryParams CQP;
	FHitResult HitResult;
	CQP.AddIgnoredActor(this);

	if (ActorLocal != nullptr)
	{
		for (FName var : SocketsToTarget)
		{
			if (PlayerCharacter)
				Loc = Mesh->GetSocketLocation(var);
			else
				Loc = ActorLocal->GetActorLocation();
			
			GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, Loc, ECC_Visibility, CQP);
			HitResult.bBlockingHit = !HasLOStoTargetLocal;
			if (HasLOStoTargetLocal == true)
			{
				break;
			}
		}
		HasLOStoTarget = HasLOStoTargetLocal;
		return HasLOStoTarget;
	}
	return HasLOStoTarget;
}

void AMM_SecurityCamera::CameraTrippedFeedback(bool AcquiredTarget)
{
	if (Enabled == true)
	{
		FVector Loc = Main_CamMesh->GetComponentLocation();
		AudioScanner->Activate(false);
		if (AcquiredTarget == true)
		{
			UGameplayStatics::PlaySoundAtLocation(nullptr, Audio1, Loc, 1.0f, 1.0f, 0.0f, nullptr, nullptr);
			AudioScanner->SetSound(ScannerAlarmSoundWave);
			AudioScanner->Activate(false);
			CameraSpotLight->SetLightColor(LightColorAlarmOn, true);
			ViewConeMaterial->SetScalarParameterValue("AlarmState", 1.0f);
			LensMaterial->SetScalarParameterValue("AlarmState", 1.0f);
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(nullptr, Audio2, Loc, 1.0f, 1.0f, 0.0f, nullptr, nullptr);
			AudioScanner->SetSound(ScannerAlarmReset);
			AudioScanner->Activate(false);
			CameraSpotLight->SetLightColor(LightColorAlarmOff, true);
			ViewConeMaterial->SetScalarParameterValue("AlarmState", 0.0f);
			LensMaterial->SetScalarParameterValue("AlarmState", 0.0f);
		}
	}
	else
	{
		AudioScanner->Deactivate();
	}
}

void AMM_SecurityCamera::EnableCamera()
{
	Enabled = true;
	SetActorTickEnabled(true);
	bHasTarget = false;
	CameraSpotLight->SetHiddenInGame(false, true);
	LightMesh->SetHiddenInGame(false, true);
	CameraTrippedFeedback(bHasTarget);
	CurveFloatVal = 16 * EnDisCurve->GetFloatValue(TimelineValue);
	if (!bHasTarget == true)
	{
		if (SecCamTimeline != nullptr)
		{
			OverlapBeginEvent();
			SecCamTimeline->PlayFromStart();
		}
		SetCameraScanningRotation(CurveFloatVal);
	}
	else
	{
		OverlapEndEvent();
		SecCamTimeline->Stop();
		SetCameraScanningRotation(CurveFloatVal);
	}
}

void AMM_SecurityCamera::DisableCamera()
{
	Enabled = false;
	SetActorTickEnabled(false);
	bHasTarget = false;
	CameraSpotLight->SetHiddenInGame(true, true);
	LightMesh->SetHiddenInGame(true, true);
	CameraTrippedFeedback(bHasTarget);
	CurveFloatVal = 16 * EnDisCurve->GetFloatValue(TimelineValue);
	if (!bHasTarget == true)
	{
		if (SecCamTimeline != nullptr)
		{
			OverlapBeginEvent();
			SecCamTimeline->PlayFromStart();
		}
		SetCameraScanningRotation(CurveFloatVal);
	}
	else
	{
		OverlapEndEvent();
		SecCamTimeline->Stop();
		SetCameraScanningRotation(CurveFloatVal);
	}
}

void AMM_SecurityCamera::PlayCameraScanningAnimation()
{
	SetCameraScanningRotation(CurveFloatVal);
}

void AMM_SecurityCamera::TimelineCallback(float val)
{
	// This function is called for every tick in the timeline.
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, TEXT("The Timeline Works"));
}

void AMM_SecurityCamera::TimelineFinishedCallback()
{
	// This function is called when the timeline finishes playing.
}

void AMM_SecurityCamera::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	bool DesiredTarget = false;
	CheckOverlapActorisTargettable(OtherActor, DesiredTarget);
	if (DesiredTarget == true)
	{
		PotentialTarget = OtherActor;
		WithinCone = true;
		OverlapBeginEvent();
	}
}

void AMM_SecurityCamera::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	bool DesiredTarget = false;
	CheckOverlapActorisTargettable(OtherActor, DesiredTarget);
	if (DesiredTarget == true)
	{
		PotentialTarget = nullptr;
		WithinCone = false;
		OverlapEndEvent();
	}
}

void AMM_SecurityCamera::DetectPlayer()
{
	CameraSpotLight->LightColor = FColor::Red;
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->IsDetected = true;
}

void AMM_SecurityCamera::SetCameraScanningRotation(float Alpha)
{
	/*float CameraPitch = -45.0f;
	float CameraMaxYaw = 70.0f;*/
	float AlphaLocal = Alpha;
	FRotator LerpedValue = FMath::Lerp(FRotator(0.0f, CameraPitch, CameraMaxYaw), FRotator(0.0f, CameraPitch, CameraMaxYaw * (-1)), AlphaLocal);

	FVector VIT = FMath::VInterpTo(Main_CamMesh->GetComponentRotation().Vector(), Target->GetActorLocation() - Main_CamMesh->GetComponentLocation(), GetWorld()->GetDeltaSeconds(), 0.02f);
	FRotator DefaultRotation = FRotator(0.0f, -45.0f, 0.0f);
	float PitchAngle = FMath::ClampAngle(UKismetMathLibrary::MakeRotFromX(VIT).Pitch, DefaultRotation.Pitch - UKismetSystemLibrary::MakeLiteralFloat(80.0f), DefaultRotation.Pitch + UKismetSystemLibrary::MakeLiteralFloat(80.0f));
	float YawAngle = FMath::ClampAngle(UKismetMathLibrary::MakeRotFromX(VIT).Yaw, DefaultRotation.Yaw - UKismetSystemLibrary::MakeLiteralFloat(80.0f), DefaultRotation.Yaw + UKismetSystemLibrary::MakeLiteralFloat(80.0f));

	if (!bHasTarget == true)
	{
		Main_CamMesh->SetRelativeRotation(FMath::RInterpTo(Main_CamMesh->RelativeRotation, LerpedValue, GetWorld()->GetDeltaSeconds(), 0.3f), false, nullptr, ETeleportType::None);
	}
	else
	{
		Main_CamMesh->SetWorldRotation(FRotator(0.0f, PitchAngle, YawAngle), false, nullptr, ETeleportType::None);
	}
}

