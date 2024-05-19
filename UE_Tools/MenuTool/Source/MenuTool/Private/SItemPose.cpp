// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemPose.h"
#include "SlateOptMacros.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SSpinBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SItemPose::Construct(const FArguments& InArgs)
{
	NamePose = InArgs._NamePose;
	ValuePoseCommitted = InArgs._ValuePose;
	OnFloatValueCommitted = InArgs._OnFloatValueCommitted;
	FSlateColor Color_Border = FSlateColor(FLinearColor::Gray * 1.f);
	FSlateFontInfo FontCustom = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/DroidSansFallback.ttf"), 12);
	InputEnable = InArgs._InputEnable;
	NumBox = SNew(SNumericEntryBox<float>)
				.IsEnabled(InputEnable)
				.MinValue(1.0f)
				.MaxValue(500.0f)
				.MaxSliderValue(500.f)
				.MinSliderValue(TAttribute< TOptional<float> >(1.0f))
				.MaxSliderValue(TAttribute< TOptional<float> >(500.0f))
				.Delta(0.5f)
				
				.Value(ValuePoseCommitted)
				//.AllowSpin(true)
				//.AllowWheel(true)
				.Font(FontCustom)
				.Value_Raw(this, &SItemPose::GetCommittedValue)
				/*.Value_Lambda([this]()-> TOptional<float> {
					UE_LOG(LogTemp, Warning, TEXT("Committed--  %s : %f"), *NamePose, ValuePoseCommitted);
					 return ValuePoseCommitted; })*/
				.OnValueCommitted(this, &SItemPose::HandlePoseValueCommitted)
				.OnValueChanged(this, &SItemPose::HandlePoseValueChanged)
				//.Value_Lambda([this] { return ValuePose; })
				//.OnValueChanged_Lambda([this](float NewValue) {UE_LOG(LogTemp, Warning, TEXT("value-%d"), NewValue); HandlePoseValueChanged(NewValue); })
			/*SNew(SSpinBox<float>)
				.Value(this, &SItemPose::GetValue)
				.MinValue(1)
				.MaxValue(500.f)
				.OnValueChanged(this, &SItemPose::HandlePoseValueChanged)
				.OnValueCommitted(this, &SItemPose::HandlePoseValueCommitted)
				.IsEnabled(InputEnable)*/


		;
	ChildSlot
	[
		SNew(SBorder)
		 .ForegroundColor(Color_Border)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.FillWidth(1.0)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(100.0f)
				[
					SNew(STextBlock)
				
					.Text(FText::FromString(NamePose))
					.Font(FontCustom)
				]
			]
	+ SHorizontalBox::Slot()
		.MaxWidth(200)
		.FillWidth(1.0)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.WidthOverride(70.0f)
		[
			
				NumBox.ToSharedRef()
				
				]
			]
		]
	];
	
}


void SItemPose::SetValue(float Newvalue)
{
	ValuePoseCommitted = Newvalue;
}

void SItemPose::HandlePoseValueChanged(float NewValue)
{
	
		if (NewValue<=1)
		{
			ValuePoseChanged = 1;
		}		
		else if (NewValue >= 500)
		{
			ValuePoseChanged = 500;
		}
		else
		{
			ValuePoseChanged = NewValue;
		}
		UE_LOG(LogTemp, Warning, TEXT("Changed--  %s : %f"), *NamePose, ValuePoseChanged);
	
	
}

void SItemPose::HandlePoseValueCommitted(float NewValue, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter/* || CommitType == ETextCommit::Default || CommitType == ETextCommit::OnUserMovedFocus*/)
	{
		if (NewValue <= 1)
		{
			ValuePoseCommitted = 1;
		}
		else if (NewValue >= 500)
		{
			ValuePoseCommitted = 500;
		}
		else
		{
			ValuePoseCommitted = NewValue;
		}
		ValuePoseChanged = ValuePoseCommitted;
		UE_LOG(LogTemp, Warning, TEXT("Committed--  %s : %f"), *NamePose, ValuePoseCommitted);
		OnFloatValueCommitted.ExecuteIfBound(NamePose, ValuePoseCommitted);
	}

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
