// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include"CommonFunctionLibrary.h"
#include "Widgets/Input/SSegmentedControl.h"


/**
 * 
 */

// // 在 自定义 SegmentStyle
//USTRUCT(BlueprintType)
//struct FMySegmentedControlStyle : public FSlateWidgetStyle
//{
//	GENERATED_USTRUCT_BODY()
//
//		FMySegmentedControlStyle()
//		: ActiveSegmentBrush(FSlateBrush())
//		, InactiveSegmentBrush(FSlateBrush())
//	{
//		ActiveSegmentBrush.TintColor = FLinearColor::Green; // 设置选中选项的颜色
//		InactiveSegmentBrush.TintColor = FLinearColor::Gray; // 设置非选中选项的颜色
//	}
//
//	UPROPERTY(EditAnywhere, Category = Appearance)
//		FSlateBrush ActiveSegmentBrush; // 定义选中选项的样式
//
//	UPROPERTY(EditAnywhere, Category = Appearance)
//		FSlateBrush InactiveSegmentBrush; // 定义非选中选项的样式
//};





class MENUTOOL_API SToolWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SToolWidget)
		:_IndexWidget(0)
	{}
	SLATE_ARGUMENT(int32, IndexWidget)
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	FReply FirstClicked()	{ index_Switch = 0; return FReply::Handled(); };
	FReply SecondClicked()	{ index_Switch = 1; return FReply::Handled(); };
	FReply ThirdClicked()	{ index_Switch = 2; return FReply::Handled(); };
	//重置事件
	FReply OnClicked_Reset()	{ UE_LOG(LogTemp,Display,TEXT("点击：重置")); return FReply::Handled(); };
	//创建事件
	FReply OnClicked_Create()	{ UE_LOG(LogTemp, Display, TEXT("点击：创建")); return FReply::Handled(); };

	void SetCurrentIndex_Switch(int32 index) { index_Switch = index; };
	//数据初始化操作
	void HandleData_Pose();
	//绑定等操作
	void initOptions();

	//百分位下拉框生成
	TSharedRef<SWidget> GenerateComboBoxItem(TSharedPtr<FString> Item)
		{
			return SNew(STextBlock).Text(FText::FromString(*Item));
		}

	//百分位下拉框选项改变
	void HandleSelectionChanged(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
	{
		if (SelectedItem.IsValid())
		{
			FString SelectedOption = *SelectedItem;
			TitleBlock_Percent->SetText(FText::FromString(SelectedOption));
			// 处理选中项的逻辑-----|
		}
	}
	//姿势数据修改后调用函数
	void OnPoseDataValueCommitted(const FString& StrPose, float NewValue);
	FOnPoseValueCommitted OnPoseValueCommitted;
private:
	//0:自定义，1：百分比
	int32 IndexWidget = 0;
	//百分位
	TArray<TSharedPtr<FString>> OptionsArray_Percent;
	TSharedPtr<STextBlock> TitleBlock_Percent;
	//标准
	TArray<TSharedPtr<FString>> OptionsArray_standard;
	TSharedPtr<STextBlock> TitleBlock_standard;
	//地区
	TArray<TSharedPtr<FString>> OptionsArray_Conutry;
	TSharedPtr<STextBlock> TitleBlock_Conutry;
	//人物选择
	TArray<TSharedPtr<FString>> OptionsArray_HumanSelected;
	TSharedPtr<STextBlock> TitleBlock_HumanSelected;
	//名称
	TSharedPtr<STextBlock> Text_Name;
	//身高
	float Height = 170.f;
	//体重
	float Weight = 120.f;
	//性别 0=男，1=女
	int32 SexIndex = 0;
	//肤色 0=黄，1=白，2=黑
	int32 ColorIndex = 0;


	TArray<FString> StrArray_ZhanZi =	{ TEXT("身高"), TEXT("眼高"),TEXT("肩高"),TEXT("会阴"), TEXT("胫骨点高") ,TEXT("上臂长"),TEXT("前臂长"), TEXT("大腿长"), TEXT("小腿长"),  };
	TArray<FString> StrArray_GongGong = { TEXT("头宽"),TEXT("头长"),TEXT("头高"),TEXT("手长"),TEXT("掌长"),TEXT("手宽"),TEXT("食指长"),TEXT("拇指长"),TEXT("足长"),TEXT("足宽"),TEXT("胸宽"),TEXT("肩宽"),TEXT("肩最大宽"),TEXT("臀宽"),TEXT("胸厚"), };
	TArray<FString> StrArray_ZuoZi =	{ TEXT("坐高") ,TEXT("坐姿眼高") ,TEXT("坐姿肩高") ,TEXT("坐姿肘高") ,TEXT("坐姿膝高") ,TEXT("坐姿臀宽") ,TEXT("坐姿膝距") ,TEXT("两肘间宽") ,};

	TMap<FString, float> Map_GongGong ;
	TMap<FString, float> Map_ZhanZi;
	TMap<FString, float> Map_ZuoZi;
	int32 index_Switch=0;
	int32 GetCurrentIndex_Switch()const { return index_Switch; };
	//FMySegmentedControlStyle SegmentStyle;

	//测试
	FString Str_Test;
	FString ImageFullPath;
	TSharedPtr< FSlateDynamicImageBrush > ImageBrush;
};
