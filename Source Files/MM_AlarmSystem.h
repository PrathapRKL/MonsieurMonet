// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_SecuritySystemInterface.h"
#include "MM_AlarmSystem.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UPointLightComponent;
class USpotLightComponent;
class UAkAudioEvent;
class UMaterialInstanceDynamic;
class UAkComponent;
class AMM_SecurityCamera;
class UMaterialInterface;

UCLASS()
class MONSIEURMONET_API AMM_AlarmSystem : public AActor, public IMM_SecuritySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMM_AlarmSystem();

	//Interface Functions.
	UFUNCTION(BlueprintCallable)
	virtual void TripAlarm_Implementation(AActor* NewActor) override;

	UFUNCTION(BlueprintCallable)
	virtual void ResetAlarm_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual bool SetFailConditions_Implementation(bool FailOnAlarmTripped) override;

	UFUNCTION(BlueprintCallable)
	virtual void DisableSecuritySystem_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual void EnableSecuritySystem_Implementation() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* AlarmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpotLightComponent* AlarmLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPointLightComponent* StandbyLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAkComponent* AlarmAudio;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMM_SecurityCamera* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LightRotationSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TestAlarm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor AlarmOffLightColor = FLinearColor(0.6f, 1.0f, 0.87f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor AlarmOnLightColor = FLinearColor(1.0f, 0.103f, 0.0267f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AlarmTripped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool SecuritySystemEnabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInstanceDynamic* AlarmMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterialInterface* Source;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAkAudioEvent* AlarmAudioEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AlarmLightIntensity = 20000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AlarmLightAttenuationRadius = 2600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AlarmLightVolumetricScatteringIntensity = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UseAlarmSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsGateOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsGateClosed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bHasAlerted;

	UFUNCTION(BlueprintCallable)
		void ManualAlarm();

	UFUNCTION(BlueprintCallable)
		void ResetAlarmSys();

	UFUNCTION(BlueprintCallable)
		void TripAlarmSys();

	UFUNCTION(BlueprintCallable)
		void DisableSecuritySys();

	UFUNCTION(BlueprintCallable)
		void EnableSecuritySys();
};
