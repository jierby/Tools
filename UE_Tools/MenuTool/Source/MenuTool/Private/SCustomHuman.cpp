// Fill out your copyright notice in the Description page of Project Settings.


#include "SCustomHuman.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Input/SSegmentedControl.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "SItemPose.h"
#include "Interfaces/IPluginManager.h"
#include "../Plugins/Editor/PluginBrowser/Source/PluginBrowser/Private/PluginStyle.h"
#include "Selection.h"
#include "AssetTools/Public/AssetToolsModule.h"
#include "Factories/AnimBlueprintFactory.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "CustomAnimInstance.h"
#include "UObject/ConstructorHelpers.h"
//#include "Widgets/SPanel.h"
//#include "SWidgetSwitcher.h"
//***************************自定义人体控件*******************************
#define LOCTEXT_NAMESPACE "SCustomHuman"
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SCustomHuman::Construct(const FArguments& InArgs)
{
	SetBPClassToCode();
	IndexWidget = InArgs._IndexWidget;
	//OnPoseValueCommitted = InArgs._OnPoseValueCommitted;
	HandleData_Pose();
	initOptions();
	index_Switch = 0;
	
	//Switch H W
	int32 H_SwitchSlot = 20;
	int32 W_SwitchSlot = 10;
	//OnPoseValueChanged.BindUFunction(this, "OnPoseDataValueCommitted");
	//OnPoseValueChanged.BindUObject(this,&SCustomHuman::OnPoseDataValueCommitted);
	//自定义样式
	FSlateFontInfo FontCustom = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/DroidSansFallback.ttf"), 11);
	FMargin PaddingCustom = FMargin(5, 5, 5, 5);
	FSlateFontInfo Font_Option = FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/DroidSansFallback.ttf"), 11);
	FSlateColor Border_Color = FSlateColor(FLinearColor::Gray * 1.f);//FSlateColor(FColor(125,254,244,255)) //FSlateColor(FLinearColor::Black * 1.f)
	auto Border_Image = FAppStyle::Get().GetBrush("Brushes.Background");//Brushes.Background  Brushes.Panel Brushes.White
	//EHorizontalAlignment VA_Left = EHorizontalAlignment::HAlign_Left ;
	//EHorizontalAlignment VA_Right = EHorizontalAlignment::HAlign_Right;
	//性别
	TSharedPtr<SHorizontalBox> SexBox = SNew(SHorizontalBox);
	for (int i = 0; i <2; i++)
	{
		SexBox->AddSlot()
			.Padding(0.f,0.f,5.0f,0.f)
			[
				SNew(SCheckBox)/*.Style(FAppStyle::Get(), "RadioButton")*/
				
				.IsChecked_Lambda([this, i]() -> ECheckBoxState { return SexIndex == i ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
				.OnCheckStateChanged_Lambda([this, i](ECheckBoxState InState) { if (InState == ECheckBoxState::Checked) SexIndex = i; })
				[
					SNew(STextBlock)
					.Font(Font_Option)
					.Text_Lambda([this, i]() 
						{
							if (i == 0)return FText::FromString(TEXT("男")); 
							return FText::FromString(TEXT("女")); 
							
						})
				]
			];
	}
	//肤色
	TSharedPtr<SHorizontalBox> ColorBox = SNew(SHorizontalBox);
	for (int i = 0; i < 3; i++)
	{
		ColorBox->AddSlot()
			.Padding(0.f, 0.f, 5.0f, 0.f)
			[
				SNew(SCheckBox)/*.Style(FAppStyle::Get(), "RadioButton")*/
				
				.IsChecked_Lambda([this, i]() -> ECheckBoxState { return ColorIndex == i ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
				.OnCheckStateChanged_Lambda([this, i](ECheckBoxState InState) { if (InState == ECheckBoxState::Checked) ColorIndex = i; })
				[
					SNew(STextBlock)
					.Font(Font_Option)
					.Text_Lambda([this, i]()
						{
							if (i == 0)return FText::FromString(TEXT("黄"));
							if (i == 1) return FText::FromString(TEXT("白"));
							return FText::FromString(TEXT("黑")); 
						})
					]
				];
	}

	//左右对齐设置
	EHorizontalAlignment HA_Option;
	//公共
	TSharedPtr<SUniformGridPanel> UniPanel_GongGong 
		= SNew(SUniformGridPanel)
			.SlotPadding(FMargin(2,2,2,2))
			.MinDesiredSlotHeight(H_SwitchSlot)
			.MinDesiredSlotWidth(W_SwitchSlot);
	for (auto& elem : StrArray_GongGong)
	{
		HA_Option = StrArray_GongGong.Find(elem) % 2 == 0 ? EHorizontalAlignment::HAlign_Left : EHorizontalAlignment::HAlign_Right;
		TSharedPtr<SItemPose> itempose = SNew(SItemPose)
			.InputEnable(true)
			.NamePose(elem)
			.ValuePose(*Map_GongGong.Find(elem))
			.OnFloatValueCommitted(OnPoseValueCommitted)
			;
		Map_Item.Add(elem, itempose);
		UniPanel_GongGong->AddSlot(StrArray_GongGong.Find(elem) % 2, StrArray_GongGong.Find(elem) / 2)
			.HAlign(HA_Option)
			.VAlign(VAlign_Top)
			[
				itempose.ToSharedRef()
			];
	}


	//站姿
	TSharedPtr<SUniformGridPanel> UniPanel_ZhanZi = 
		SNew(SUniformGridPanel)
		.SlotPadding(FMargin(2, 2, 2, 2))
		.MinDesiredSlotHeight(H_SwitchSlot)
		.MinDesiredSlotWidth(W_SwitchSlot);;

	for (auto& elem : StrArray_ZhanZi)
	{
		HA_Option = StrArray_ZhanZi.Find(elem) % 2 == 0 ? EHorizontalAlignment::HAlign_Left : EHorizontalAlignment::HAlign_Right;
		TSharedPtr<SItemPose> itempose = SNew(SItemPose)
			.InputEnable(true)
			.NamePose(elem)
			.ValuePose(*Map_ZhanZi.Find(elem))
			.OnFloatValueCommitted(OnPoseValueCommitted)
			;
		Map_Item.Add(elem, itempose);

		UniPanel_ZhanZi->AddSlot(StrArray_ZhanZi.Find(elem) % 2, StrArray_ZhanZi.Find(elem) / 2)
			.HAlign(HA_Option)
			.VAlign(VAlign_Top)
			[
				itempose.ToSharedRef()
				//SNew(SCanvas)
				//+SCanvas::Slot()
				// //.FSlotArguments()
				//
				//[
				//	SNew(SBorder)
				//	
				//	[
				//		SNew(SHorizontalBox)
				//		+SHorizontalBox::Slot()
				//		[
				//			SNew(STextBlock)
				//			.Text(FText::FromString(elem))
				//		]
				//		//+ SHorizontalBox::Slot()
				//		//[
				//		//	SNew(SSpinBox<float>)							
				//		//	.Value(10.f)
				//		//	//.OnValueChanged(this, &SCustomHuman::OnPoseDataValueCommitted)
				//		//	//.OnValueCommitted(this, &SCustomHuman::OnPoseDataValueCommitted)
				//		//]
				//	]
				//]

			];
	}

	//坐姿
	TSharedPtr<SUniformGridPanel> UniPanel_ZuoZi = 
		SNew(SUniformGridPanel)
		
		.SlotPadding(FMargin(2, 2, 2, 2))
		.MinDesiredSlotHeight(H_SwitchSlot)
		.MinDesiredSlotWidth(W_SwitchSlot);;

	for (auto& elem : StrArray_ZuoZi)
	{
		HA_Option = StrArray_ZuoZi.Find(elem) % 2 == 0 ? EHorizontalAlignment::HAlign_Left : EHorizontalAlignment::HAlign_Right;
		TSharedPtr<SItemPose> itempose = SNew(SItemPose)
			.InputEnable(true)
			.NamePose(elem)
			.ValuePose(*Map_ZuoZi.Find(elem))
			.OnFloatValueCommitted(OnPoseValueCommitted)
			;
		Map_Item.Add(elem, itempose);
		UniPanel_ZuoZi->AddSlot(StrArray_ZuoZi.Find(elem) % 2, StrArray_ZuoZi.Find(elem) / 2)
			.HAlign(HA_Option)
			.VAlign(VAlign_Top)
			[
				itempose.ToSharedRef()
			];
	}

	//自定义人体界面
	
		UE_LOG(LogTemp, Display, TEXT("自定义：：0000000000000000"));
		ChildSlot
			[
				/*SNew(SCanvas)
				+SCanvas::Slot()*/
				/*[*/
				//0v
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()//标题
				.FillHeight(0.1f)
				[
					SNew(STextBlock)
					.Font(Font_Option)
					.Text(FText::FromString(TEXT("创建自定义人体")))
				]
				+ SVerticalBox::Slot()//标题
				.AutoHeight()
				[
					SNew(SHorizontalBox)//第一列功能：名称，百分位，标准
					+ SHorizontalBox::Slot()
					.Padding(PaddingCustom)
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.VAlign(VAlign_Center)
						.AutoWidth()
					[
						SNew(STextBlock)
						.Font(Font_Option)
						.Text(FText::FromString(TEXT("名称：    ")))
					]
						+ SHorizontalBox::Slot()//名称输入框
						.FillWidth(1.0)
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						[
							SNew(SEditableTextBox)
							.Font(Font_Option)
							.HintText(FText::FromString(TEXT("输入名称")))
							//.OnTextChanged_Lambda([this](const FText& text) {if (Text_Name)Text_Name->SetText(FText::FromString(text.ToString())); })
							.OnTextCommitted_Lambda([this](const FText& text, ETextCommit::Type CommitType){if(CommitType==ETextCommit::OnEnter)Name_Text = text;})
						]
				]
				+ SVerticalBox::Slot()//第一列第二行
			//[
			//	SNew(SHorizontalBox)
			//	+ SHorizontalBox::Slot()
			//	.VAlign(VAlign_Center)
			//	.AutoWidth()

			//[
			//	SNew(STextBlock)
			//	.Font(Font_Option)
			//	.Text(FText::FromString(TEXT("百分位：")))
			//]
			//	+ SHorizontalBox::Slot()//百分位选项
			// 
			//		.FillWidth(1.0)
			//		.HAlign(HAlign_Left)
			//		[
			//			SNew(SComboBox<TSharedPtr<FString>>)
			//			//.ComboBoxStyle(FAppStyle::Get(),"SimpleComboBox")
			//			.OptionsSource(&OptionsArray_Percent)
			//			.OnGenerateWidget(this, &SCustomHuman::GenerateComboBoxItem)
			//		/*.OnSelectionChanged_Lambda([this](TSharedPtr<FString> InSelection, ESelectInfo::Type InSelectInfo)
			//			{
			//				if (InSelection.IsValid())
			//				{
			//					ComboBox_PercentTitleBlock->SetText(FText::FromString(*InSelection));
			//				}
			//			})*/
			//		.OnSelectionChanged(this, &SCustomHuman::HandleSelectionChanged)
			//		[
			//			SAssignNew(TitleBlock_Percent, STextBlock)
			//			.Font(Font_Option)
			//			.Text(FText::FromString(TEXT("P1")))
			//		]
			//		]

			//]
				+ SVerticalBox::Slot()//第一列第三行
			//[
			//	SNew(SHorizontalBox)
			//	+ SHorizontalBox::Slot()
			//	.VAlign(VAlign_Center)
			//	.AutoWidth()
			//[
			//	SNew(STextBlock)
			//	.Font(Font_Option)
			//	.Text(FText::FromString(TEXT("标准：    ")))
			//]
			//	+ SHorizontalBox::Slot()//标准选项
			//	.HAlign(HAlign_Left)
			//	[
			//	SNew(SComboBox<TSharedPtr<FString>>)
			//	//.ComboBoxStyle(FAppStyle::Get(), "SimpleComboBox")
			//	.OptionsSource(&OptionsArray_standard)
			//	/*.OnGenerateWidget(this, &SCustomHuman::GenerateComboBoxItem)*/
			//	.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
			//		{
			//			return SNew(STextBlock).Text(FText::FromString(*Item));
			//		})
			//	.OnSelectionChanged_Lambda([this](TSharedPtr<FString> InSelection, ESelectInfo::Type InSelectInfo)
			//		{
			//			if (InSelection.IsValid())
			//			{
			//				TitleBlock_standard->SetText(FText::FromString(*InSelection));
			//			}
			//		})
			//			//.OnSelectionChanged(this, &SCustomHuman::HandleSelectionChanged)
			//			[
			//				SAssignNew(TitleBlock_standard, STextBlock)
			//				.Font(Font_Option)
			//			.Text(FText::FromString(TEXT("《中国成年人人体尺寸》")))
			//			]
			//	]
			//]
			]
		+ SHorizontalBox::Slot()//第二列功能：肤色，性别，地区
			.Padding(PaddingCustom)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()//第二列第一行
			[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Font(Font_Option)
						.Text(FText::FromString(TEXT("肤色：    ")))
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.0)
					.HAlign(HAlign_Left)
				[
					ColorBox.ToSharedRef()
				]
			]
				+ SVerticalBox::Slot()//第二列第二行
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Font(Font_Option)
						.Text(FText::FromString(TEXT("性别：    ")))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SexBox.ToSharedRef()
					]
			]
				+ SVerticalBox::Slot()//第二列第三行
		//	[
		//		SNew(SHorizontalBox)
		//		+ SHorizontalBox::Slot()
		//	.VAlign(VAlign_Center)
		//	.AutoWidth()
		//	[
		//		SNew(STextBlock)
		//		.Font(Font_Option)
		//	.Text(FText::FromString(TEXT("地区：    ")))
		//	]
		//+ SHorizontalBox::Slot()//地区选项
		//	.HAlign(HAlign_Left)
		//	[
		//		SNew(SComboBox<TSharedPtr<FString>>)
		//		//.ComboBoxStyle(FAppStyle::Get(), "SimpleComboBox")
		//	.OptionsSource(&OptionsArray_Conutry)
		//	/*.OnGenerateWidget(this, &SCustomHuman::GenerateComboBoxItem)*/
		//	.OnGenerateWidget_Lambda([](TSharedPtr<FString> Item)
		//		{
		//			return SNew(STextBlock).Text(FText::FromString(*Item));
		//		})
		//	.OnSelectionChanged_Lambda([this](TSharedPtr<FString> InSelection, ESelectInfo::Type InSelectInfo)
		//		{
		//			if (InSelection.IsValid() && TitleBlock_Conutry.IsValid())
		//			{
		//				TitleBlock_Conutry->SetText(FText::FromString(*InSelection));
		//			}
		//		})
		//			//.OnSelectionChanged(this, &SCustomHuman::HandleSelectionChanged)
		//			[
		//				SAssignNew(TitleBlock_Conutry, STextBlock)
		//				.Font(Font_Option)
		//			.Text(FText::FromString(TEXT("中国")))
		//			]
		//	]
		//	]
			]
			+ SHorizontalBox::Slot()//第三列功能：身高，体重，人物选择
			.Padding(PaddingCustom)
			[
				SNew(SVerticalBox)//第三列第一行
				+ SVerticalBox::Slot()
				.Padding(PaddingCustom)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
				[
						SNew(STextBlock)
						.Font(Font_Option)
						.Text(FText::FromString(TEXT("身高：    ")))
				]
					+ SHorizontalBox::Slot()//身高输入框
					.FillWidth(1.0)
					.HAlign(HAlign_Left)
					[
						SNew(SNumericEntryBox<float>)
						.Font(Font_Option)
					.MinValue(10.0f)
					.MaxValue(300.0f)
					.MinSliderValue(TAttribute< TOptional<float> >(10.0f))
					.MaxSliderValue(TAttribute< TOptional<float> >(300.0f))
					.Delta(0.5f)
					.Value(170.0f)

					//.OnValueChanged_Lambda([this](float InValue) { Height = InValue; })
					//.OnValueCommitted_Lambda([this](float InValue, ETextCommit::Type CommitInfo) { Height = InValue; })
					.Value_Lambda([this] { return TOptional<float>(*Map_ZhanZi.Find(Height_Actor)); })
					.OnValueCommitted(this,&SCustomHuman::OnHeightCommitted)
					]
			]
				+ SVerticalBox::Slot()//第三列第二行
				.Padding(PaddingCustom)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Font(Font_Option)
						.Text(FText::FromString(TEXT("体重：    ")))
					]
					+ SHorizontalBox::Slot()//体重输入框
					.HAlign(HAlign_Left)
					.FillWidth(1.0)
					[
						SNew(SNumericEntryBox<float>)
						.Font(Font_Option)
						.MinValue(10.0f)
						.MaxValue(600.0f)
						.MinSliderValue(TAttribute< TOptional<float> >(10.0f))
						.MaxSliderValue(TAttribute< TOptional<float> >(600.0f))
						.Delta(0.5f)
						.Value(120.0f)

						//.OnValueChanged_Lambda([this](float InValue) { Weight = InValue; })
						//.OnValueCommitted_Lambda([this](float InValue, ETextCommit::Type CommitInfo) { Weight = InValue; })
						.Value_Lambda([this] {
							                             if (AnimControl)
							                             {
							                             	return TOptional<float>(AnimControl->GetWeight());
							                             }
							                             return TOptional<float>(0); })
						.OnValueCommitted(this,&SCustomHuman::OnWeightCommitted)

					]
			]
				+ SVerticalBox::Slot()//第三列第三行
				.Padding(PaddingCustom)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					 .AutoWidth()
					[
						SNew(STextBlock)
						.Font(Font_Option)						
						.Text(FText::FromString(TEXT("人物选择：")))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						.Font(Font_Option).Text_Lambda([this]()->FText 
							{
								GetSelectedActor();
								if (SelectedActor!=nullptr)
								{									
									NameSelectedActor = SelectedActor->GetName(); 
									return FText::FromString(NameSelectedActor);
								}
								else
								{
									NameSelectedActor = TEXT("请点击角色");
									return FText::FromString(NameSelectedActor);
								}
							})

					]


				]
			]
			]
		/*+ SVerticalBox::Slot()
		.FillHeight(0.1f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("1111111111")))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("222222222")))
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("3333333333")))
			]
		]*/
		//描述：人体尺寸（单位：cm）
		+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10, 20, 10, 0)
			[
				SNew(STextBlock)
				.Font(Font_Option)
				.Text(FText::FromString(TEXT("人体尺寸（单位：cm）")))
				.ColorAndOpacity(FSlateColor(FLinearColor::Gray * 0.6f))
			]

		//人体尺寸
		+ SVerticalBox::Slot()
			.Padding(10, 10, 10, 10)
			[
				SNew(SHorizontalBox)
				//数据
			+ SHorizontalBox::Slot()
			[
				//姿势表格
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
			.Padding(0, 0, 50, 15)
			//.FillHeight(0.1f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Left)
			[
				SNew(STextBlock)
				.Font(Font_Option)
			.Text(FText::FromString(TEXT("姿势选择")))

			]
		+ SHorizontalBox::Slot()
			.AutoWidth()
			.FillWidth(1.0)
			//.Padding(100,0,50,0)
			.HAlign(HAlign_Right)
			[
				SNew(SSegmentedControl<int32>)
				.Value(0) // InitialValue
			.OnValueChanged_Lambda([this](int32 InValue) { SetCurrentIndex_Switch(InValue); })
			// .Value_Lambda( [this] { return SegmentedBoxChoice; } )  // Bound 

			+ SSegmentedControl<int32>::Slot(0)
			//.Icon(FAppStyle::Get().GetBrush("image1"))
			.Text(FText::FromString(TEXT("公共")))

			+ SSegmentedControl<int32>::Slot(1)
			//.Icon(FAppStyle::Get().GetBrush("Icons.cylinder"))
			.Text(FText::FromString(TEXT("站姿")))

			+ SSegmentedControl<int32>::Slot(2)
			//.Icon(FAppStyle::Get().GetBrush("Icons.pyramid"))
			.Text(FText::FromString(TEXT("坐姿")))

			//+ SSegmentedControl<int32>::Slot(3)
			//.Icon(FAppStyle::Get().GetBrush("Icons.sphere"))
			//.Text(FText::FromString(("Sphere", "Sphere")))
			]
			]
		+ SVerticalBox::Slot()
			.Padding(0, 0, 50, 0)
			.AutoHeight()
			[

				//姿势表格
				SNew(SWidgetSwitcher)
				.WidgetIndex(this, &SCustomHuman::GetCurrentIndex_Switch)
			+ SWidgetSwitcher::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			//公共
			[
				UniPanel_GongGong.ToSharedRef()
				/*SNew(SButton)
				.Text(FText::FromString(TEXT("1111")))*/

			]
		//站姿
		+ SWidgetSwitcher::Slot()

			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				UniPanel_ZhanZi.ToSharedRef()
				/*SNew(SButton)
				.Text(FText::FromString(TEXT("222")))*/

			]
		//坐姿
		+ SWidgetSwitcher::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			[
				UniPanel_ZuoZi.ToSharedRef()
			]
			]
			]
		+ SHorizontalBox::Slot()
			.FillWidth(0.65)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				//.ColorAndOpacity(FLinearColor::Blue*1)
				 .BorderImage(Border_Image)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.BorderBackgroundColor(Border_Color)
				
			[
				SNew(SImage)

				.Image(ImageBrush.Get())
			]
		

			]
		]
		
		
		//start-功能按钮
		+SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0,10,10,20)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("重置")))
					.OnClicked(this,&SCustomHuman::OnClicked_Reset)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("创建")))
					.OnClicked(this, &SCustomHuman::OnClicked_Create)
				]

			]

			
		//End-功能按钮
			
		]; 
	

	
	

	
}

FReply SCustomHuman::OnClicked_Reset()
{
	if (SelectedActor!=nullptr&&AnimControl!=nullptr)
	{
		AnimControl->ResetHumanData();
		UpdateAnimBlueprint();
		UpdateAllPoseData();
	}
	
	return FReply::Handled();
}

FReply SCustomHuman::OnClicked_Create()
{
	int32 SelectionCount = GEditor->GetSelectedActorCount();
	auto It = GEditor->GetSelectedActorIterator();

	for (; It; ++It)
	{
		if (AActor* Actor = Cast<AActor>(*It))
		{
			SelectedActor = Actor;
			TInlineComponentArray<USkeletalMeshComponent*> Components;
			SelectedActor->GetComponents(Components);
			for (auto component : Components)
			{
				if (component->ComponentHasTag(TEXT("Body")))
				{
					BodySketal = component;
					BodySketal->SetUpdateAnimationInEditor(true);
				}

				if (component->ComponentHasTag(TEXT("Face")))
				{
					FaceSketal = component;
					FaceSketal->SetUpdateAnimationInEditor(true);
				}
			}
		}
	}

	if (!BodySketal)
	{
		return FReply::Handled();
	}
		FActorSpawnParameters ActorParms;
		FVector SpawnLocation = SelectedActor==nullptr? FVector(0,0,0):SelectedActor->GetActorLocation()+FVector(100,0,0);
		FRotator SpawnRotation = SelectedActor==nullptr? FRotator(0,0,0):SelectedActor->GetActorRotation();

		AActor*ActorSpawn =  GEditor->GetEditorWorldContext().World()->SpawnActor<AActor>(BlueprintActorClass,SpawnLocation,SpawnRotation);
	
	FString Name;
	FString PackageName;
	IAssetTools::Get().CreateUniqueAssetName(BlueprintClassObj->GetOutermost()->GetName(), TEXT("_Child"), PackageName, Name);
	const FString PackagePath = FPackageName::GetLongPackagePath(PackageName);

	UE_LOG(LogTemp, Log, TEXT("生成的全新资源：%s"), *PackagePath);

	UClass* TargetParentClass = BlueprintClassObj;

	if (!FKismetEditorUtilities::CanCreateBlueprintOfClass(TargetParentClass))
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("InvalidClassToMakeBlueprintFrom", "Invalid class with which to make a Blueprint."));
		return FReply::Handled();
	}

	UAnimBlueprintFactory* AnimBlueprintFactory = NewObject<UAnimBlueprintFactory>();
	AnimBlueprintFactory->BlueprintType = BPTYPE_Normal;
	AnimBlueprintFactory->ParentClass = TSubclassOf<UCustomAnimInstance>(TargetParentClass);
	AnimBlueprintFactory->TargetSkeleton = BodySketal->GetAnimInstance()->CurrentSkeleton;
	AnimBlueprintFactory->bTemplate = false;
	AnimBlueprintFactory->SupportedClass = UAnimBlueprint::StaticClass();

	// FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	// ContentBrowserModule.Get().CreateNewAsset(Name,PackagePath,UAnimBlueprint::StaticClass(),AnimBlueprintFactory);

	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	UAnimBlueprint* NewAsset = CastChecked<UAnimBlueprint>(AssetToolsModule.Get().CreateAsset(Name, PackagePath, UAnimBlueprint::StaticClass(), AnimBlueprintFactory));
	NewAsset->SetPreviewAnimationBlueprint(NewAsset);
	NewAsset->OnCompiled();
	NewAsset->BroadcastCompiled();
	if (NewAsset)
	{
		// AnimBlueprint = NewAsset;
		ChangeClass = NewAsset->GeneratedClass;
		UPackage* package = NewAsset->GetOutermost();
		UClass* Myclass = NewAsset->GetBlueprintClass();

		FString packageFileName = FPackageName::LongPackageNameToFilename(package->GetName(), FPackageName::GetAssetPackageExtension());
		bool bSaved = UPackage::SavePackage(package, nullptr, EObjectFlags::RF_Standalone, *packageFileName, GError, nullptr, true, true, SAVE_NoError);
		if (bSaved)
		{
			UE_LOG(LogTemp, Log, TEXT("新创建的动画蓝图保存成功：%s"), *package->GetName());
		}

		BodySketal->ClearAnimScriptInstance();
		BodySketal->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		BodySketal->SetAnimClass(NewAsset->GeneratedClass);
		BodySketal->SetAnimInstanceClass(NewAsset->GeneratedClass);
	}

	TInlineComponentArray<UAnimControlComponent*> AnimComponents;
	ActorSpawn->GetComponents(AnimComponents);
	if (AnimComponents.IsValidIndex(0))
	{
		AnimControl = AnimComponents[0];
		AnimControl->InitializeInEditor(BodySketal, FaceSketal);
	}

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

//姿势数据
void SCustomHuman::HandleData_Pose()
{
	for (FString var : StrArray_ZhanZi)
	{
		Map_ZhanZi.Add(var, 0);
	}
	for (auto var : StrArray_ZuoZi)
	{
		Map_ZuoZi.Add(var, 0);
	}
	for (auto var : StrArray_GongGong)
	{
		Map_GongGong.Add(var, 0);
	}
}

void SCustomHuman::initOptions()
{
	OnPoseValueCommitted.BindRaw(this,&SCustomHuman::OnPoseDataValueCommitted);

	//图片-测试
	TSharedPtr<IPlugin> MyPlugin = IPluginManager::Get().FindPlugin("MenuTool");
	if (MyPlugin.IsValid())
	{
		// 获取插件的绝对路径
		FString PluginPath = MyPlugin->GetBaseDir();

		// 图片相对于插件目录的路径
		FString ImageRelativePath = "/Resources/Image1.png";

		// 构建完整的图片路径
		ImageFullPath = FPaths::Combine(*PluginPath, *ImageRelativePath);
		UE_LOG(LogTemp, Display, TEXT("sss： '%s' sss： %s"), *PluginPath, *ImageFullPath);
		// 创建 Slate Brush 并使用图片
		FSlateBrush MyCustomBrush;
		//MyCustomBrush.SetResourceObject(FSlateApplication::Get().GetRenderer()->GetResourceHandle(ImageFullPath));
		//ImageBrush.ImageSize = FVector2D(100.0f, 100.0f);
		//ImageBrush

		const FName BrushName(*ImageFullPath);
		const FIntPoint Size = FSlateApplication::Get().GetRenderer()->GenerateDynamicImageResource(BrushName);
		ImageBrush = MakeShareable(new FSlateDynamicImageBrush(BrushName, FVector2D(Size.X, Size.Y)));

		UE_LOG(LogTemp, Display, TEXT("x： '%d' y： %d"), Size.X, Size.Y);
		
	
	}



	if (OptionsArray_Percent.IsEmpty())
	{
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("P1")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("P2")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("P3")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("P5")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("P50")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("P95")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("P97")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("P98")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("P99")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("平均值")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("平均值标准误差")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("标准差")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("最大值")));
		OptionsArray_Percent.Add(MakeShared<FString>(TEXT("最小值")));
	}
	if (OptionsArray_Conutry.IsEmpty())
	{
		/*TitleBlock_Conutry->SetText(FText::FromString(TEXT("中国")));*/
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("中国")));
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("德国")));
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("意大利")));
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("日本")));
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("肯尼亚")));
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("韩国")));
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("荷兰")));
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("非洲")));
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("欧洲")));
		OptionsArray_Conutry.Add(MakeShared<FString>(TEXT("亚洲")));
	
	}

	if (OptionsArray_standard.IsEmpty())
	{
		OptionsArray_standard.Add(MakeShared<FString>(TEXT("《中国成年人人体尺寸》")));
		OptionsArray_standard.Add(MakeShared<FString>(TEXT("《中国男性飞行员人体尺寸》")));
		OptionsArray_standard.Add(MakeShared<FString>(TEXT("INTERNATIONALSTANDARD")));
		OptionsArray_standard.Add(MakeShared<FString>(TEXT("MIL-HDBK-759C")));
		OptionsArray_standard.Add(MakeShared<FString>(TEXT("其他")));
	}
	if (OptionsArray_HumanSelected.IsEmpty())
	{
		OptionsArray_HumanSelected.Add(MakeShared<FString>(TEXT("")));
	}

	
}

void SCustomHuman::OnPoseDataValueInput(FString PoseName, float NewValue)
{
	UE_LOG(LogTemp, Display, TEXT("输入数据：名称： '%s' 值： %f"), *PoseName, NewValue);
	if (Map_GongGong.Find(PoseName) != nullptr && Map_Item.Find(PoseName) != nullptr)
	{
		Map_GongGong.Add(PoseName, NewValue);
		Map_Item.Find(PoseName)->Get()->SetValue(NewValue);
		return;
	}
	else if (Map_ZhanZi.Find(PoseName) != nullptr)
	{
		Map_ZhanZi.Add(PoseName, NewValue);
		Map_Item.Find(PoseName)->Get()->SetValue(NewValue);
		return;
	}
	else if (Map_ZuoZi.Find(PoseName) != nullptr)
	{
		Map_ZuoZi.Add(PoseName, NewValue);
		Map_Item.Find(PoseName)->Get()->SetValue(NewValue);
		return;
	}
	;
}

void SCustomHuman::UpdateAnimBlueprint()
{
	ChangeClass->Modify();
	UPackage* package = ChangeClass->GetOutermost();
	if (package->IsDirty())
	{
		FString packageFileName;
		if(Name_Text.IsEmpty()){packageFileName = FPackageName::LongPackageNameToFilename(package->GetName(), FPackageName::GetAssetPackageExtension());}
		else {packageFileName = FPackageName::LongPackageNameToFilename(Name_Text.ToString(), FPackageName::GetAssetPackageExtension());}
		bool bSaved = UPackage::SavePackage(package, nullptr, EObjectFlags::RF_Standalone, *packageFileName, GError, nullptr, true, true, SAVE_NoError);
		if (bSaved && SelectedActor != nullptr)
		{
			UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
			SelectedActor->Modify();

			BodySketal->SetAnimClass(nullptr);
			FaceSketal->SetAnimClass(nullptr);

			auto callback = [this]()
			{
				UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();

				BodySketal->SetAnimClass(ChangeClass);
				BodySketal->SetAnimInstanceClass(ChangeClass);
				BodySketal->SetUpdateAnimationInEditor(true);

				FaceSketal->SetAnimClass(FaceAnimBlueprintClass);
				FaceSketal->SetAnimInstanceClass(FaceAnimBlueprintClass);
				FaceSketal->SetUpdateAnimationInEditor(true);

				EditorWorld->GetTimerManager().ClearTimer(handle);
			};
			FTimerDelegate timedelegate;
			timedelegate.BindLambda(callback);

			handle = EditorWorld->GetTimerManager().SetTimerForNextTick(timedelegate);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("资产保存失败"));
		}
	}
}

void SCustomHuman::UpdateAllPoseData()
{
	if (SelectedActor==nullptr||AnimControl==nullptr)
	{
		return;
	}
	Stand_Data_Anim= AnimControl->GetStanceData();
	Sit_Data_Anim = AnimControl->GetSittingPostureData();
	Public_Data_Anim = AnimControl->GetPublicData();
	for (auto data : Stand_Data_Anim)
	{
		// Map_ZhanZi.Add(data.Key.ToString(),data.Value);
		OnPoseDataValueInput(data.Key.ToString(),data.Value);
	}
	for (auto data : Sit_Data_Anim)
	{
		//Map_ZuoZi.Add(data.Key.ToString(),data.Value);
		OnPoseDataValueInput(data.Key.ToString(),data.Value);
	}
	for (auto data : Public_Data_Anim)
	{
		//Map_GongGong.Add(data.Key.ToString(),data.Value);
		OnPoseDataValueInput(data.Key.ToString(),data.Value);
	}
}

void SCustomHuman::SetBPClassToCode()
{
	BlueprintClassObj = LoadClass<UCustomAnimInstance>(nullptr,TEXT("/MenuTool/Body.Body_C"));
	// UE_LOG(LogTemp, Warning, TEXT("%s"),*FPaths::ProjectContentDir());FPaths::ProjectContentDir();
	//UE_LOG(LogTemp, Warning, TEXT(FPaths::ProjectContentDir()));
	// static ConstructorHelpers::FClassFinder<AActor> ActorBPClass(TEXT("/Game/MetaHumans/FXY_Huang/BP_FXY_Huang.BP_FXY_Huang_C"));
	// if (ActorBPClass.Class != nullptr)
	// {
	// 	BlueprintActorClass = ActorBPClass.Class;
	// }
	// else{BlueprintActorClass = LoadClass<AActor>(nullptr,TEXT("/Game/MetaHumans/FXY_Huang/BP_FXY_Huang.BP_FXY_Huang_C"));}
	FString ActorClassPath = FPaths::ProjectContentDir()+TEXT("MetaHumans/FXY_Huang/BP_FXY_Huang.BP_FXY_Huang_C");
	UE_LOG(LogTemp, Warning, TEXT("Actor路径：%s"),*ActorClassPath);
	
	BlueprintActorClass = LoadClass<AActor>(nullptr,TEXT("/Game/MetaHumans/FXY_Huang/BP_FXY_Huang.BP_FXY_Huang_C"));
	// UE_LOG(LogTemp, Warning, TEXT("Actor路径：%s"),*BlueprintActorClass->GetFName().ToString());/**/
	if(BlueprintActorClass != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor类导入成功：1"));
	}
	else
	{
		
		//static ConstructorHelpers::FClassFinder<AActor> BlueprintClass(TEXT(FPaths::ProjectContentDir()+TEXT("MetaHumans/FXY_Huang/BP_FXY_Huang.BP_FXY_Huang_C")));
		// if (BlueprintClass.Succeeded())
		// {
		// 	UE_LOG(LogTemp, Warning, TEXT("Actor类导入成功：2"));
		// 	BlueprintActorClass = BlueprintClass.Class;
		// }

	}
	// BlueprintClassCharacter = LoadClass<AActor>(nullptr,TEXT("/Game/MetaHumans/FXY_Huang/BP_FXY_Huang.BP_FXY_Huang_C"));
	// if(BlueprintClassCharacter!=nullptr)
	// {
	// 	
	// }

	FaceAnimBlueprintClass = LoadClass<UFaceAnimInstance>(nullptr,TEXT("/MenuTool/Face.Face_C"));
	if(FaceAnimBlueprintClass !=nullptr)
	{
		
	}
	
}

void SCustomHuman::GetSelectedActor()
{
	auto SelectedActors = GEditor->GetSelectedActors();
	
	
	if (SelectedActors && SelectedActors->Num() > 0)
	{
		// 获取第一个选中的Actor
		for (FSelectionIterator It(*SelectedActors); It; ++It)
		{
			if (SelectedActor!=nullptr && SelectedActor== *It)
			{
				return;
				break;
			}
			else
			{
				SelectedActor = Cast<AActor>(*It);
				break;
			}		
			break;
		}
		
		if (SelectedActor!=nullptr&&SelectedActor->GetComponentByClass(UAnimControlComponent::StaticClass())!=nullptr)
		{
			SetSkeletal();
		}
		else
		{
			SelectedActor = nullptr;
		}
		
	}
	else
	{
		SelectedActor = nullptr;
	}
	
}

void SCustomHuman::SetSkeletal()
{	
	//处理
	FaceSketal=nullptr;
	BodySketal=nullptr;
	TInlineComponentArray<USkeletalMeshComponent*> Components;
	SelectedActor->GetComponents(Components);	
	for (auto component : Components)
	{
		if (component->ComponentHasTag(TEXT("Body")))
		{
			BodySketal = component;
			if (BodySketal->GetAnimClass()==nullptr)
			{
				SelectedActor = nullptr;
				return;
			}
			
			BodySketal->SetUpdateAnimationInEditor(true);
		}

		if (component->ComponentHasTag(TEXT("Face")))
		{
			FaceSketal = component;
			if (FaceSketal->GetAnimClass()==nullptr)
			{
				SelectedActor = nullptr;
				return;
			}
			FaceSketal->SetUpdateAnimationInEditor(true);
		}
	}
	if (BodySketal==nullptr || FaceSketal==nullptr)
	{
		SelectedActor = nullptr;
		return;
	}

	TInlineComponentArray<UAnimControlComponent*> AnimComponents;
	SelectedActor->GetComponents(AnimComponents);
	if (AnimComponents.IsValidIndex(0))
	{
		AnimControl = AnimComponents[0];
		AnimControl->InitializeInEditor(BodySketal, FaceSketal);
	}

	if (!BodySketal||!FaceSketal)
	{
		SelectedActor=nullptr;
		return;
	}
	UpdateAllPoseData();
	ChangeClass = BodySketal->AnimClass;
}

void SCustomHuman::OnPoseDataValueCommitted(const FString& StrPose, float NewValue)
{
	// SetSkeletal();
	UE_LOG(LogTemp, Display, TEXT("自定义：名称： '%s' 值： %f"), *StrPose, NewValue);
	Str_Test = FString::Printf(TEXT("%s : %f"), *StrPose, NewValue);
	if (SelectedActor!=nullptr&&AnimControl!=nullptr)
	{
		OnPoseValueCommitted.ExecuteIfBound(StrPose, NewValue);
		AnimControl->ApplyBoneInput(StrPose, NewValue);

		UpdateAnimBlueprint();

	}
	

}


#undef LOCTEXT_NAMESPACE
