// Copyright Epic Games, Inc. All Rights Reserved.

#include "MenuTool.h"

//#include "UMG/Public/EditorUtilityWidget.h"
#include "MenuToolStyle.h"
#include "MenuToolCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/SWindow.h"
//#include "UMG/Public/Blueprint/UserWidget.h"
#include "Subsystems/EditorAssetSubsystem.h"
#include "EditorScriptingUtilities/Public/EditorAssetLibrary.h"
#include "Blutility/Classes/EditorUtilityWidgetBlueprint.h"


static const FName MenuToolTabName("MenuTool");

#define LOCTEXT_NAMESPACE "FMenuToolModule"

void FMenuToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMenuToolStyle::Initialize();
	FMenuToolStyle::ReloadTextures();

	FMenuToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);
	
	//PluginCommands->MapAction(
	//	FMenuToolCommands::Get().OpenPluginWindow,
	//	FExecuteAction::CreateRaw(this, &FMenuToolModule::PluginButtonClicked),
	//	FCanExecuteAction());
	PluginCommands->MapAction(
		FMenuToolCommands::Get().OpenWindow_PercentHuman,
		FExecuteAction::CreateRaw(this, &FMenuToolModule::Onclicked_PercentCreate),
		FCanExecuteAction());
	PluginCommands->MapAction(
		FMenuToolCommands::Get().OpenWindow_CustomHuman,
		FExecuteAction::CreateRaw(this, &FMenuToolModule::OnClicked_CustomCreate),
		FCanExecuteAction());
	PluginCommands->MapAction(
		FMenuToolCommands::Get().OpenWindow_PoseLib,
		FExecuteAction::CreateRaw(this, &FMenuToolModule::Onclicked_PoseLib),
		FCanExecuteAction());
	//动捕
	PluginCommands->MapAction(
		FMenuToolCommands::Get().OpenWindow_Recording,
		FExecuteAction::CreateRaw(this, &FMenuToolModule::Onclicked_Recording),
		FCanExecuteAction());
	PluginCommands->MapAction(
		FMenuToolCommands::Get().OpenWindow_DongBuSetting,
		FExecuteAction::CreateRaw(this, &FMenuToolModule::Onclicked_DongbuSetting),
		FCanExecuteAction());



	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMenuToolModule::RegisterMenus));
	
	//FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MenuToolTabName, FOnSpawnTab::CreateRaw(this, &FMenuToolModule::OnSpawnPluginTab))
	//	.SetDisplayName(LOCTEXT("FMenuToolTabTitle", "MenuTool"))
	//	.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMenuToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMenuToolStyle::Shutdown();

	FMenuToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MenuToolTabName);
}

void  FMenuToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	UE_LOG(LogTemp, Warning, TEXT("dfdfdfdf"));
	/*if (CustomHuman_Widget.IsValid())
	{
		return ;
	}*/
	//FText WidgetText = FText::Format(
	//	LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
	//	FText::FromString(TEXT("FMenuToolModule::OnSpawnPluginTab")),
	//	FText::FromString(TEXT("MenuTool.cpp"))
	//	);
	////SAssignNew(SCustomHuman_Widget, CustomHuman_Widget).OwnerHUD(this);
	//CustomHuman_Widget = MakeShared<SCustomHuman>();
	////TAttribute<int32> WidgetSwitcherIndexAttribute = TAttribute<int32>::CreateSP(this, &SNiagaraSystemEffectTypeBar::GetActiveDetailsWidgetIndex);
	//TSharedRef<SWindow>MenuWindow = SNew(SWindow)
	//	[
	//		SNew(SCustomHuman)
	//	];
	//SNew(SDockTab)
	//	.TabRole(ETabRole::NomadTab)
	//	[
	//		SNew(SCustomHuman)
	//	];
	 //SNew(SWindow).Content(CustomHuman_Widget.ToSharedRef());
	//TSharedRef<SWindow> windowss = SNew(SWindow).Title(FText::FromString(TEXT("Cookbook Window")));
	//windowss->SetContent(CustomHuman_Widget.ToSharedRef());
	//ColorBox->SetContent(CustomHuman_Widget.ToSharedRef());
	//MenuWindow->SetContent(CustomHuman_Widget.ToSharedRef());
	return ;
	//return SNew(SDockTab)
	//	.TabRole(ETabRole::NomadTab)
	//	[
	//		// Put your tab content here!
	//		SNew(SBox)
	//		.HAlign(HAlign_Left)
	//		.VAlign(VAlign_Top)
	//		
	//		//+SCanvas::Slot().AutoWidth()
	//		[
	//			SNew(SHorizontalBox)
	//			+SHorizontalBox::Slot()
	//			[
	//				SNew(SVerticalBox)
	//				+ SVerticalBox::Slot()
	//				.AutoHeight()
	//				.HAlign(HAlign_Left)
	//				.VAlign(VAlign_Top)
	//				[
	//				
	//					SNew(STextBlock)
	//					.Text(FText::FromString(TEXT("文本")))
	//					//SNew(STextBlock)
	//				]
	//				+ SVerticalBox::Slot()
	//				.AutoHeight()
	//				.HAlign(HAlign_Left)
	//				.VAlign(VAlign_Top)
	//				[

	//					SNew(SButton)
	//					.Text(FText::FromString(TEXT("Click Me点击")))
	//					.OnClicked_Lambda([&]() {UE_LOG(LogTemp, Warning, TEXT("CLicking")); return FReply::Handled(); })
	//				//SNew(STextBlock)
	//				]

	//				/*SNew(STextBlock)
	//				.Text(WidgetText)*/
	//			]
	//			+ SHorizontalBox::Slot().HAlign(HAlign_Left)
	//				[
	//					/*SNew(SButton)
	//					.Text(WidgetText)*/
	//					/*SNew(SWidgetSwitcher)
	//					+ SWidgetSwitcher::Slot().FSlotArguments()
	//					.AutoWidth()
	//					[
	//					]*/
	//				]
	//		]
	//	];
}

void FMenuToolModule::OnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("clicked"));
	/*if (CustomHuman_Widget!=nullptr)
	{
		
		return;
	}
	CustomHuman_Widget = SNew(SCustomHuman);
	TSharedRef<SWindow>MenuWindow = SNew(SWindow)
		.Title(FText::FromString(TEXT("创建自定义人体1")))
		[
			CustomHuman_Widget.ToSharedRef()
		];
	
	FSlateApplication::Get().AddWindow(MenuWindow);*/
}

//FReply FMenuToolModule::OnButtonClick_Test()
//{
//	UE_LOG(LogTemp, Warning, TEXT("dfdfdfd"));
//	return FReply::Handled();
//}

void FMenuToolModule::PluginButtonClicked()
{
	/*FGlobalTabmanager::Get()->TryInvokeTab(MenuToolTabName);
	OnClicked();*/
	OnClicked_CustomCreate();
	
}

void FMenuToolModule::OnClicked_CustomCreate()
{
	UE_LOG(LogTemp, Warning, TEXT("OnClicked_Custom"));
	//-------------------------------------------------
	if (CustomHuman_Widget!=nullptr && CustomHuman_Window.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("已存在窗口"));		
		FSlateApplication::Get().FindWidgetWindow(CustomHuman_Window.ToSharedRef())->BringToFront();
		//TSharedPtr<SWidget> ParentWidget = CustomHuman_Widget->GetParent();
		//TSharedPtr<SWindow> ParentWindow = StaticCastSharedPtr<SWindow>(ParentWidget);
		//if (ParentWindow.IsValid())
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("存在窗口"));
		//	FSlateApplication::Get().FindWidgetWindow(ParentWindow.ToSharedRef())->BringToFront();
		//	//ParentWindow->BringToFront(); 
		//}

		return;
	}


	if (CustomHuman_Widget==nullptr)
	{
		
			FString assetPath = TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/MenuTool/UI/UI_Custom_HUman.UI_Custom_HUman'");
			auto asset = UEditorAssetLibrary::LoadAsset(assetPath);
			CustomHuman_Widget = Cast<UEditorUtilityWidgetBlueprint>(asset);
		
	}
		
	if (CustomHuman_Widget == nullptr) return;

	CustomHuman_Window = SNew(SWindow)
	.Title(FText::FromString(TEXT("创建自定义人体-窗口")))
	 .SupportsMinimize(false)
	 .SupportsMaximize(false)
	.ClientSize(FVector2D(966, 847))
	 .IsTopmostWindow(true)
	.SizingRule(ESizingRule::FixedSize)
	.IsInitiallyMaximized(false)  
	.IsInitiallyMinimized(false)
		[
			CustomHuman_Widget->CreateUtilityWidget()
		];
	CustomHuman_Window->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
	{
		CustomHuman_Widget=nullptr;
		CustomHuman_Window = nullptr; // 重置引用，窗口已关闭
	}));
	FSlateApplication& SlateApp = FSlateApplication::Get();
	SlateApp.AddWindow(CustomHuman_Window.ToSharedRef(), true);	
	
}

void FMenuToolModule::Onclicked_PercentCreate()
{

	UE_LOG(LogTemp, Warning, TEXT("OnClicked_Custom"));
	//-------------------------------------------------
	if (PercentHuman_Widget!=nullptr && PercentHuman_Window.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("已存在窗口"));		
		FSlateApplication::Get().FindWidgetWindow(PercentHuman_Window.ToSharedRef())->BringToFront();		
		return;
	}


	if (PercentHuman_Widget==nullptr)
	{		
			FString assetPath = TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/MenuTool/UI/UI_PercentHuman.UI_PercentHuman'");
			auto asset = UEditorAssetLibrary::LoadAsset(assetPath);
			PercentHuman_Widget = Cast<UEditorUtilityWidgetBlueprint>(asset);
			//UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(asset);
	}
		
	if (PercentHuman_Widget == nullptr) return;

	PercentHuman_Window = SNew(SWindow)
	.Title(FText::FromString(TEXT("创建百分位人体-窗口")))
	 .SupportsMinimize(false)
	 .SupportsMaximize(false)
	.ClientSize(FVector2D(966, 847))
	 //.IsTopmostWindow(true)
	.SizingRule(ESizingRule::FixedSize)
	.IsInitiallyMaximized(false)  
	.IsInitiallyMinimized(false)
		[
			PercentHuman_Widget->CreateUtilityWidget()
		];
	PercentHuman_Window->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
	{
		PercentHuman_Widget=nullptr;
		PercentHuman_Window = nullptr; // 重置引用，窗口已关闭
	}));
	/*FSlateApplication& SlateApp = FSlateApplication::Get();
	SlateApp.AddWindow(PercentHuman_Window.ToSharedRef(), true);*/
	const TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (RootWindow.IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(PercentHuman_Window.ToSharedRef(), RootWindow.ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(PercentHuman_Window.ToSharedRef());
	}

	
}

void FMenuToolModule::Onclicked_PoseLib()
{
	if (PoseLib_Widget!=nullptr && PoseLib_Window.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("已存在窗口"));		
		FSlateApplication::Get().FindWidgetWindow(PoseLib_Window.ToSharedRef())->BringToFront();
		
		return;
	}


	if (PoseLib_Widget==nullptr)
	{		
		FString assetPath = TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/Game/LWX/Test-UI.Test-UI'");
		auto asset = UEditorAssetLibrary::LoadAsset(assetPath);
		PoseLib_Widget = Cast<UEditorUtilityWidgetBlueprint>(asset);
		//UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(asset);
	}
		
	if (PoseLib_Widget == nullptr) return;

	PoseLib_Window = SNew(SWindow)
	.Title(FText::FromString(TEXT("姿态库-窗口")))
	 .SupportsMinimize(false)
	 .SupportsMaximize(false)
	.ClientSize(FVector2D(700, 720))
	 //.IsTopmostWindow(true)
	.SizingRule(ESizingRule::FixedSize)
	.IsInitiallyMaximized(false)  
	.IsInitiallyMinimized(false)
		[
			PoseLib_Widget->CreateUtilityWidget()
		];
	PoseLib_Window->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
	{
		PoseLib_Widget=nullptr;
		PoseLib_Window = nullptr; // 重置引用，窗口已关闭
	}));
	//FSlateApplication& SlateApp = FSlateApplication::Get();
	//SlateApp.AddWindow(PoseLib_Window.ToSharedRef(), true);
	const TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (RootWindow.IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(PoseLib_Window.ToSharedRef(), RootWindow.ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(PoseLib_Window.ToSharedRef());
	}
}

void FMenuToolModule::Onclicked_DongbuSetting()
{
	UE_LOG(LogTemp, Warning, TEXT("动捕设置"));
	if (DongBuSetting_Widget != nullptr && DongBuSetting_Window.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("已存在窗口"));
		FSlateApplication::Get().FindWidgetWindow(DongBuSetting_Window.ToSharedRef())->BringToFront();
		return;
	}


	if (DongBuSetting_Widget == nullptr)
	{
		FString assetPath = TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/MenuTool/UI/UI_DongBuSetting.UI_DongBuSetting'");
		auto asset = UEditorAssetLibrary::LoadAsset(assetPath);
		DongBuSetting_Widget = Cast<UEditorUtilityWidgetBlueprint>(asset);
		//UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(asset);
	}

	if (DongBuSetting_Widget == nullptr) return;

	DongBuSetting_Window = SNew(SWindow)
		.Title(FText::FromString(TEXT("创建百分位人体-窗口")))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.ClientSize(FVector2D(966, 847))
		//.IsTopmostWindow(true)
		.SizingRule(ESizingRule::FixedSize)
		.IsInitiallyMaximized(false)
		.IsInitiallyMinimized(false)
		[
			DongBuSetting_Widget->CreateUtilityWidget()
		];
	DongBuSetting_Window->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
		{
			DongBuSetting_Widget = nullptr;
	DongBuSetting_Window = nullptr; // 重置引用，窗口已关闭
		}));
	const TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (RootWindow.IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(DongBuSetting_Window.ToSharedRef(), RootWindow.ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(DongBuSetting_Window.ToSharedRef());
	}


}



void FMenuToolModule::Onclicked_Recording()
{
	UE_LOG(LogTemp, Warning, TEXT("录制设置"));
	if (Recording_Widget != nullptr && Recording_Window.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("已存在窗口"));
		FSlateApplication::Get().FindWidgetWindow(Recording_Window.ToSharedRef())->BringToFront();
		return;
	}


	if (Recording_Widget == nullptr)
	{
		FString assetPath = TEXT("/Script/Blutility.EditorUtilityWidgetBlueprint'/MenuTool/UI/UI_Recording.UI_Recording'");
		auto asset = UEditorAssetLibrary::LoadAsset(assetPath);
		Recording_Widget = Cast<UEditorUtilityWidgetBlueprint>(asset);
		//UEditorUtilityWidgetBlueprint* EditorWidget = Cast<UEditorUtilityWidgetBlueprint>(asset);
	}

	if (Recording_Widget == nullptr) return;

	Recording_Window = SNew(SWindow)
		.Title(FText::FromString(TEXT("创建百分位人体-窗口")))
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.ClientSize(FVector2D(966, 847))
		//.IsTopmostWindow(true)
		.SizingRule(ESizingRule::FixedSize)
		.IsInitiallyMaximized(false)
		.IsInitiallyMinimized(false)
		[
			Recording_Widget->CreateUtilityWidget()
		];
	Recording_Window->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
		{
			Recording_Widget = nullptr;
	Recording_Window = nullptr; // 重置引用，窗口已关闭
		}));
	const TSharedPtr<SWindow> RootWindow = FGlobalTabmanager::Get()->GetRootWindow();
	if (RootWindow.IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(Recording_Window.ToSharedRef(), RootWindow.ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(Recording_Window.ToSharedRef());
	}



}

void FMenuToolModule::OnCommittedPoseValue(const FString& NamePose, float ValuePoe)
{
	UE_LOG(LogTemp, Warning, TEXT("%s--%f"),*NamePose, ValuePoe);


}

//菜单 创建
void FMenuToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		//测试菜单
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("Text");
			Section.AddSubMenu("NewMenu",
				LOCTEXT("New Menu", "人体模型"),
				LOCTEXT("", "创建人体模型"),
				FNewToolMenuChoice());

			static const FName NewMenuName = "LevelEditor.MainMenu.NewMenu";
			UToolMenu* NewMenu = UToolMenus::Get()->RegisterMenu(NewMenuName);
			FToolMenuSection& NewSection = NewMenu->AddSection("CreateHuman", FText::FromString(TEXT("人体模型")));
			NewSection.AddMenuEntryWithCommandList(FMenuToolCommands::Get().OpenWindow_CustomHuman, PluginCommands);
			NewSection.AddMenuEntryWithCommandList(FMenuToolCommands::Get().OpenWindow_PercentHuman, PluginCommands);
			NewSection.AddMenuEntryWithCommandList(FMenuToolCommands::Get().OpenWindow_PoseLib, PluginCommands);


		}

		
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("DongBu");
			Section.AddSubMenu("DongBu",
				LOCTEXT("DongBu", "动捕"),
				LOCTEXT("", "动捕"),
				FNewToolMenuChoice());

			static const FName NewMenuName = "LevelEditor.MainMenu.DongBu";
			UToolMenu* NewMenu = UToolMenus::Get()->RegisterMenu(NewMenuName);
			FToolMenuSection& DongBuSection = NewMenu->AddSection("DongBu", FText::FromString(TEXT("动捕")));
			DongBuSection.AddMenuEntryWithCommandList(FMenuToolCommands::Get().OpenWindow_Recording, PluginCommands);
			DongBuSection.AddMenuEntryWithCommandList(FMenuToolCommands::Get().OpenWindow_DongBuSetting, PluginCommands);
			

		}

	}

	{
		/*UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMenuToolCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}*/
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMenuToolModule, MenuTool)