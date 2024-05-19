// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SCustomHuman.h"
#include "Modules/ModuleManager.h"
#include "SPercentHuman.h"

class FToolBarBuilder;
class FMenuBuilder;
class UEditorUtilityWidgetBlueprint;

class FMenuToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	//按钮功能-自定义人体：
	void OnClicked_CustomCreate();
	//按钮功能-百分位人体：
	void Onclicked_PercentCreate();
	void Onclicked_PoseLib();
	//动捕
	void Onclicked_DongbuSetting();
	void Onclicked_Recording();
	
	void OnCommittedPoseValue(const FString& NamePose, float ValuePoe);

	void RegisterMenus();

	/*TSharedRef<class SWindow>*/void  OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	void OnClicked();

	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr<SCustomHuman>CustomHuman_Slate;
	//自定义人体窗口
	UEditorUtilityWidgetBlueprint* CustomHuman_Widget;
	TSharedPtr<SWindow>CustomHuman_Window;
	//百分位人体窗口
	UEditorUtilityWidgetBlueprint* PercentHuman_Widget;
	TSharedPtr<SWindow>PercentHuman_Window;
	//姿态库窗口
	UEditorUtilityWidgetBlueprint* PoseLib_Widget;
	TSharedPtr<SWindow>PoseLib_Window;
	//FReply OnButtonClick_Test();
	//TSharedRef<SWindow> MenuWindow;
	//动捕设置
	UEditorUtilityWidgetBlueprint* DongBuSetting_Widget;
	TSharedPtr<SWindow>DongBuSetting_Window;
	//录制窗口
	UEditorUtilityWidgetBlueprint* Recording_Widget;
	TSharedPtr<SWindow>Recording_Window;
};
