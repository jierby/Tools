// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MenuToolStyle.h"

class FMenuToolCommands : public TCommands<FMenuToolCommands>
{
public:

	FMenuToolCommands()
		: TCommands<FMenuToolCommands>(TEXT("MenuTool"), NSLOCTEXT("Contexts", "MenuTool", "MenuTool Plugin"), NAME_None, FMenuToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	// TSharedPtr< FUICommandInfo > OpenPluginWindow;
	TSharedPtr<FUICommandInfo> OpenWindow_PercentHuman;
	TSharedPtr<FUICommandInfo> OpenWindow_CustomHuman;
	TSharedPtr<FUICommandInfo> OpenWindow_PoseLib;
	//动捕
	TSharedPtr<FUICommandInfo> OpenWindow_Recording;
	TSharedPtr<FUICommandInfo> OpenWindow_DongBuSetting;

};