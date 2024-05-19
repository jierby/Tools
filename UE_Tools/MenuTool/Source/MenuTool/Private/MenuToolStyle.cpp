// Copyright Epic Games, Inc. All Rights Reserved.

#include "MenuToolStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMenuToolStyle::StyleInstance = nullptr;

void FMenuToolStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMenuToolStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMenuToolStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MenuToolStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon50x50(50.0f, 50.0f);


TSharedRef< FSlateStyleSet > FMenuToolStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MenuToolStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MenuTool")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MenuTool.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("icon_190"), Icon16x16));

	return Style;
}

void FMenuToolStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMenuToolStyle::Get()
{
	return *StyleInstance;
}
