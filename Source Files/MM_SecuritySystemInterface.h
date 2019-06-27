// This code belongs to Learning Machines.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MM_SecuritySystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UMM_SecuritySystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MONSIEURMONET_API IMM_SecuritySystemInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void TripAlarm(AActor* NewActor);
	UFUNCTION()
	virtual void TripAlarm_Implementation(AActor* NewActor) = 0;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void ResetAlarm();
	UFUNCTION()
	virtual void ResetAlarm_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		bool SetFailConditions(bool FailOnAlarmTripped);
	UFUNCTION()
	virtual bool SetFailConditions_Implementation(bool FailOnAlarmTripped) = 0;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DisableSecuritySystem();
	UFUNCTION()
	virtual void DisableSecuritySystem_Implementation() = 0;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void EnableSecuritySystem();
	UFUNCTION()
	virtual	void EnableSecuritySystem_Implementation() = 0;
};
