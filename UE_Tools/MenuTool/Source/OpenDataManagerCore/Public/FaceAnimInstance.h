// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FaceAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPENDATAMANAGERCORE_API UFaceAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	// UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,Category="Human Custom")
	void ResetHumanData();
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Human Custom",meta=(DisplayName="头高",ClampMin = 22.33f, ClampMax = 27.77f))
	double Nan_Gao;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Human Custom",meta=(DisplayName="头长",ClampMin = 18.4f, ClampMax = 24.5f))
	double Nan_Chang;
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Human Custom",meta=(DisplayName="头宽",ClampMin = 18.4f, ClampMax = 22.58f))
	double Nan_Kuan;
};
