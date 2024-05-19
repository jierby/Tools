// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CustomAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class OPENDATAMANAGERCORE_API UCustomAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,Category="Human Custom")
	void ResetHumanData();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,Category="Human Custom")
	void SetActiveIndex(int32 index);
	
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="头"))
	FTransform HeadCtrl;
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="头部旋转"))
	FRotator HeadRotator;
	
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="左手"))
	FTransform Hand_L_C;
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="左手旋转"))
	FRotator LHandRotator;
	
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="右手"))
	FTransform Hand_R_C;
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="右手旋转"))
	FRotator RHandRotator;
	
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="腰"))
	FTransform Pelvis_C;
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="腰旋转"))
	FRotator PelvisRotator;
	
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="左脚"))
	FTransform Ball_L;
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="左脚旋转"))
	FRotator LFootRotator;
	
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="右脚"))
	FTransform Ball_R;
	UPROPERTY(BlueprintReadWrite,Category="CtrlRig",meta=(DisplayName="右脚旋转"))
	FRotator RFootRotator;
	
	UPROPERTY(BlueprintReadWrite,Category="Human Custom",meta=(DisplayName="体重",ClampMin = 1.0f, ClampMax = 1.6f))
	// UPROPERTY(BlueprintReadWrite,Category="Human Custom",meta=(DisplayName="体重"))
	double Weight;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom",meta=(DisplayName="胸厚"))
	double ChestDepth;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom",meta=(DisplayName="肩宽"))
	double ShoulderWidth;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom",meta=(DisplayName="肩最大宽"))
	double MaxShoulderWidth;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom",meta=(DisplayName="胸宽"))
	double ChestWidth;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom",meta=(DisplayName="坐姿臀膝距"))
	double SittingHipKneeDistance;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom",meta=(DisplayName="臀宽R"))
	FVector HipWidthR;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom",meta=(DisplayName="臀宽L"))
	FVector HipWidthL;
	
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	int32 ActiveChildIndex;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	double pelvis;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	double calf;
	
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector spine_1;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector spine_2;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector spine_3;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector spine_4;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector spine_5;
	
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	double neck_01;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	double neck_02;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	double head;

	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector index_meta_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector index_01_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector index_02_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector index_03_l;
	
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector middle_meta_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector middle_01_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector middle_02_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector middle_03_l;
	
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector thumb_01_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector thumb_02_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector thumb_03_l;
	
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector index_meta_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector index_01_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector index_02_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector index_03_r;

	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector middle_meta_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector middle_01_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector middle_02_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector middle_03_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector thumb_01_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector thumb_02_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector thumb_03_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ball_location;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ball_scale;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	double foot;
	//3.21--------------------------------
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector pinky_meta_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector pinky_01_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector pinky_02_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector pinky_03_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ring_meta_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ring_01_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ring_02_r;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ring_03_r;

	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector pinky_meta_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector pinky_01_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector pinky_02_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector pinky_03_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ring_meta_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ring_01_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ring_02_l;
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector ring_03_l;
	//----------------------------------
	UPROPERTY(BlueprintReadWrite,Category="Human Custom")
	FVector Translation;

	UPROPERTY(BlueprintReadWrite,Category="上肢")
	FVector lowerarm_r;
	UPROPERTY(BlueprintReadWrite,Category="上肢")
	FVector hand_r;
	UPROPERTY(BlueprintReadWrite,Category="上肢")
	FVector hand_l;
	UPROPERTY(BlueprintReadWrite,Category="上肢")
	FVector lowerarm_l;

	//3.19FXL
	UPROPERTY(BlueprintReadWrite,Category="下肢")
	FVector calf_twist_01_r;
	UPROPERTY(BlueprintReadWrite,Category="下肢")
	FVector calf_twist_01_l;
	UPROPERTY(BlueprintReadWrite,Category="下肢")
	FVector calf_twist_02_r;
	UPROPERTY(BlueprintReadWrite,Category="下肢")
	FVector calf_twist_02_l;

	//3.20FXL
	UPROPERTY(BlueprintReadWrite,Category="下肢")
	FVector UpperArm_01_r;
	UPROPERTY(BlueprintReadWrite,Category="下肢")
	FVector UpperArm_l;
	UPROPERTY(BlueprintReadWrite,Category="下肢")
	FVector Forearm_02_r;
	UPROPERTY(BlueprintReadWrite,Category="下肢")
	FVector Forearm_02_l;
};

