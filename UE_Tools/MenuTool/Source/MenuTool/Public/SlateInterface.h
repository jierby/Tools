// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SlateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USlateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MENUTOOL_API ISlateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Test(){};
	virtual  void OnPoseDataValueInput(FString PoseName, float NewValue) {};
	virtual  void OnPoseDataValueCommitted(const FString& StrPose, float NewValue){};
	virtual  void CreateHumanByHWeight(float Height,float Weight) {};
	virtual  void CreateHumanByPercent(int32 Index_PercentHuman){}
	// /**/// virtual  void 
};
