// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"CommonFunctionLibrary.h"
#include "SlateInterface.h"
#include "Widgets/SCompoundWidget.h"

class SItemPose;

/**
 * 
 
 */
//


class MENUTOOL_API SPercentHuman : public SCompoundWidget,public ISlateInterface
{
public:
	SLATE_BEGIN_ARGS(SPercentHuman)
	{}
	SLATE_END_ARGS()

		/** Constructs this widget with InArgs */
		void Construct(const FArguments& InArgs);
	
	
	//----------Func-Operate------//
	//数据初始化操作
	void HandleData_Pose();
	//绑定等操作
	void initOptions();
	//姿势数据修改后调用函数
	virtual void OnPoseDataValueCommitted(const FString& StrPose, float NewValue) override;
	//姿势数据传入
	virtual void OnPoseDataValueInput(FString PoseName, float NewValue) override;

	// virtual void CreateHumanByHWeight(float Height, float Weight) override;
	// virtual void CreateHumanByPercent(int32 Index_PercentHuman) override;

	
	//按钮-百分位创建事件
	FReply OnClicked_PercentCreate() ;
	//按钮-身高体重创建事件
	FReply OnClicked_HWeightCreate() ;
	//性别切换
	void OnClickSexValeChanged(int32 i );
	//肤色切换
	void OnClickSkinValeChanged(int32 i );

	//----------SLATE-Operate-----//
	//切换姿势
	FReply FirstClicked() { index_Switch = 0; return FReply::Handled(); };
	FReply SecondClicked() { index_Switch = 1; return FReply::Handled(); };
	FReply ThirdClicked() { index_Switch = 2; return FReply::Handled(); };
	void SetCurrentIndex_Switch(int32 index) { index_Switch = index; };
	//百分位下拉框生成
	TSharedRef<SWidget> GenerateComboBoxItem(TSharedPtr<FString> Item){	return SNew(STextBlock).Text(FText::FromString(*Item));}
	//百分位下拉框选项改变
	void HandleSelectionChanged(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo);
	
	FOnPoseValueCommitted OnPoseValueCommitted;
	FHumanCreate_HWeight OnHumanCreate_HW;
	FOnSexValueChanged OnSexValueChanged;
	FOnSkinColorChanged OnSkinValueChanged;
private:
	//控件类型
	ETypeSlate TypeSlate = ETypeSlate::Percent;

	//0:自定义，1：百分比
	int32 IndexWidget = 0;
	//百分位
	TArray<TSharedPtr<FString>> OptionsArray_Percent;
	TSharedPtr<STextBlock> TitleBlock_Percent;
	//标准
	TArray<TSharedPtr<FString>> OptionsArray_standard;
	TSharedPtr<STextBlock> TitleBlock_standard;
	//地区
	TArray<TSharedPtr<FString>> OptionsArray_Country;
	TSharedPtr<STextBlock> TitleBlock_Country;
	//人物选择
	TArray<TSharedPtr<FString>> OptionsArray_HumanSelected;
	TSharedPtr<STextBlock> TitleBlock_HumanSelected;
	TArray<TSharedPtr<AActor>> ActorArray_HumanSelected;

	//名称
	TSharedPtr<STextBlock> Text_Name;
	//身高
	float Height = 170.f;
	//体重
	float Weight = 120.f;
	//性别 0=男，1=女
	int32 SexIndex = 0;
	ESex SexSelected = ESex::Male;
	//肤色 0=黄，1=白，2=黑
	int32 ColorIndex = 0;
	ESkin_Color SkinColor = ESkin_Color::Yellow;


	TArray<FString> StrArray_ZhanZi = { TEXT("身高"), TEXT("眼高"),TEXT("肩高"),TEXT("会阴"), TEXT("胫骨点高") ,TEXT("上臂长"),TEXT("前臂长"), TEXT("大腿长"), TEXT("小腿长"), };
	TArray<FString> StrArray_GongGong = { TEXT("头宽"),TEXT("头长"),TEXT("头高"),TEXT("手长"),TEXT("掌长"),TEXT("手宽"),TEXT("食指长"),TEXT("拇指长"),TEXT("足长"),TEXT("足宽"),TEXT("胸宽"),TEXT("肩宽"),TEXT("肩最大宽"),TEXT("臀宽"),TEXT("胸厚"), };
	TArray<FString> StrArray_ZuoZi = { TEXT("坐高") ,TEXT("坐姿眼高") ,TEXT("坐姿肩高") ,TEXT("坐姿肘高") ,TEXT("坐姿膝高") ,TEXT("坐姿臀宽") ,TEXT("坐姿膝距") ,TEXT("两肘间宽") , };

	TMap<FString, float> Map_GongGong;
	TMap<FString, float> Map_ZhanZi;
	TMap<FString, float> Map_ZuoZi;
	TMap<FString, TSharedPtr<SItemPose>> Map_Item;
	int32 index_Switch = 0;
	int32 GetCurrentIndex_Switch()const { return index_Switch; };
	//FMySegmentedControlStyle SegmentStyle;

	//测试
	FString Str_Test;
	FString ImageFullPath;
	TSharedPtr< FSlateDynamicImageBrush > ImageBrush;
};
