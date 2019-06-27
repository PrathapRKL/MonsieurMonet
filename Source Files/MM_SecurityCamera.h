// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "MM_SecurityCamera.generated.h"

class UStaticMeshComponent;
class USpotLightComponent;
class UTimelineComponent;
class USceneCaptureComponent2D;
class AMonsieurMonet_Character;

UCLASS()
class MONSIEURMONET_API AMM_SecurityCamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMM_SecurityCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Base_CamMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Main_CamMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* LightMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpotLightComponent* CameraSpotLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scanning")
		float CameraMaxYaw = 70.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scanning")
		float CameraPitch = -45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scanning")
		float TestYaw = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scanning")
		FRotator DefaultRotation = FRotator(0.0f, -45.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpotLight")
		float Range = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpotLight")
		float ConeAngle = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpotLight")
		float SpotLightBrightness = 60000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpotLight")
		FLinearColor LightColorAlarmOff = FLinearColor(0.36f, 1.0f, 0.702f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpotLight")
		FLinearColor LightColorAlarmOn = FLinearColor(1.0f, 0.129f, 0.0237f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpotLight")
		float ViewConeVisibility = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpotLight")
		float VolumetricScatteringIntensity = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpotLight")
		float EndScaleModifier = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpotLight")
		float LengthScaleModifier = 0.933f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		bool TargetLost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
		bool TestAlarmMaterialSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTextureRenderTarget2D* SceneCaptureTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float SceneCaptureViewDistance = 512;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* ViewConeMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* LensMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* AlarmInteractors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* PotentialTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMonsieurMonet_Character* TargettedCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasLOS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool WithinCone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool SecuritySystemEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FName> SocketsToTarget;

	UPROPERTY(EditAnywhere)
		float ConeEndRadiusUnscaled = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UAudioComponent* AudioScanner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Audio1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* Audio2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* ScannerAlarmSoundWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundBase* ScannerAlarmReset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneCaptureComponent2D* SceneCapture;

	float ConeMeshScaleXY;
	float ConeMeshScaleZ;
	float ConeLengthUnscaled = 128.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UCurveFloat* EnDisCurve;

	bool Timelinebegin;
	bool ReadyState;
	float TimelineValue;
	float CurveFloatVal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTimelineComponent* SecCamTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurveFloatValue;

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> TimelineDirection;

	UFUNCTION()
		void TimelineCallback(float val);

	UFUNCTION()
		void TimelineFinishedCallback();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHasTarget;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void DetectPlayer();

	UFUNCTION(BlueprintImplementableEvent)
		void OverlapBeginEvent();

	UFUNCTION(BlueprintImplementableEvent)
		void OverlapEndEvent();

	UFUNCTION(BlueprintCallable)
		void SetCameraScanningRotation(float Alpha);

public:
	UFUNCTION(BlueprintCallable)
		bool CheckOverlapActorisTargettable(AActor* Actor, bool DesiredTarget);

	UFUNCTION(BlueprintCallable)
		bool CheckVisibilityToOverlapActor(AActor* Actor, bool HasLOStoTarget);

	UFUNCTION(BlueprintCallable)
		void CameraTrippedFeedback(bool AcquiredTarget);

	UFUNCTION(BlueprintCallable)
		void EnableCamera();

	UFUNCTION(BlueprintCallable)
		void DisableCamera();

	UFUNCTION(BlueprintCallable)
		void PlayCameraScanningAnimation();
};
