// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

DECLARE_DELEGATE_TwoParams(FOnValueCommitted, const FString&, float);

/**
 * 
 */
class MENUTOOL_API SItemPose : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SItemPose)
		:_NamePose(TEXT("Name-Pose"))
		,_ValuePose(170.f)
		,_InputEnable(true)
		//,_OnFloatValueChanged(FOnValueChanged)
	{}
	SLATE_ARGUMENT(FString, NamePose)
	SLATE_ARGUMENT(float, ValuePose)
	SLATE_ARGUMENT(bool, InputEnable)

	SLATE_EVENT(FOnValueCommitted, OnFloatValueCommitted)


	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	/*TOptional<float> GetValue() const 
	{
		//UE_LOG(LogTemp, Warning, TEXT("GetValue %s-%f"), *NamePose.ToString(), ValuePoseChanged);
		return ValuePoseChanged;
	};*/
	TOptional<float>  GetCommittedValue() const
	{
		//UE_LOG(LogTemp, Warning, TEXT("--ֵ--  %s : %f"), *NamePose, ValuePoseCommitted);
		return ValuePoseCommitted;
	}
	FString GetNamePose() { return NamePose; };
	void SetValue(float Newvalue);
private:
	bool InputEnable = true;
	FString NamePose;
	float ValuePoseCommitted=170.0f;
	float ValuePoseChanged = 170;
	FOnValueCommitted OnFloatValueCommitted;
	TSharedPtr<SNumericEntryBox<float>> NumBox;
	void HandlePoseValueChanged(float NewValue );
	void HandlePoseValueCommitted(float NewValue, ETextCommit::Type CommitType);



};
