// Copyright Epic Games, Inc. All Rights Reserved.

#include "MenuToolCommands.h"

#define LOCTEXT_NAMESPACE "FMenuToolModule"

void FMenuToolCommands::RegisterCommands()
{
	//UI_COMMAND(OpenPluginWindow, "MenuTool", "Bring up MenuTool window", EUserInterfaceActionType::None, FInputChord());
	UI_COMMAND(OpenWindow_PercentHuman, "创建百分位人体", "创建百分位人体", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenWindow_CustomHuman, "创建自定义人体", "创建自定义人体", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenWindow_PoseLib, "姿态库", "姿态库", EUserInterfaceActionType::Button, FInputChord());
	//动捕
	UI_COMMAND(OpenWindow_Recording, "录制", "录制", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(OpenWindow_DongBuSetting, "动捕设置", "动捕设置", EUserInterfaceActionType::Button, FInputChord());


}

#undef LOCTEXT_NAMESPACE
