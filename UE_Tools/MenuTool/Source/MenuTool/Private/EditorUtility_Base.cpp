// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorUtility_Base.h"

#include "AnimationRecorder.h"
#include "AnimControlComponent.h"
#include "CommonFunctionLibrary.h"
#include "ContentBrowserDelegates.h"
#include "CustomAnimInstance.h"
#include "FaceAnimInstance.h"
#include "UnrealEdGlobals.h"
#include "AssetToolsModule.h"
#include "CanvasTypes.h"
#include "ContentStreaming.h"
#include "EditorStyleSet.h"
#include "IImageWrapperModule.h"
#include "ShaderCompiler.h"
#include "Animation/AnimBlueprint.h"
#include "Editor/UnrealEdEngine.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Factories/AnimBlueprintFactory.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Slate/SlateTextures.h"
#include "ThumbnailRendering/TextureThumbnailRenderer.h"
#include "AssetThumbnail.h"
#include "ImageUtils.h"

/** Renders a thumbnail for the specified object */
void UEditorUtility_Base::RenderThumbnail(UObject* InObject, const uint32 InImageWidth, const uint32 InImageHeight, ThumbnailTools::EThumbnailTextureFlushMode::Type InFlushMode, FTextureRenderTargetResource* InTextureRenderTargetResource, FObjectThumbnail* OutThumbnail)
{
	if (!FApp::CanEverRender())
	{
		return;
	}

	// Renderer must be initialized before generating thumbnails
	check(GIsRHIInitialized);

	// Store dimensions
	if (OutThumbnail)
	{
		OutThumbnail->SetImageSize(InImageWidth, InImageHeight);
	}

	// Grab the actual render target resource from the texture.  Note that we're absolutely NOT ALLOWED to
	// dereference this pointer.  We're just passing it along to other functions that will use it on the render
	// thread.  The only thing we're allowed to do is check to see if it's NULL or not.
	FTextureRenderTargetResource* RenderTargetResource = InTextureRenderTargetResource;
	if (RenderTargetResource == NULL)
	{
		// No render target was supplied, just use a scratch texture render target
		const uint32 MinRenderTargetSize = FMath::Max(InImageWidth, InImageHeight);
		UTextureRenderTarget2D* RenderTargetTexture = GEditor->GetScratchRenderTarget(MinRenderTargetSize);
		check(RenderTargetTexture != NULL);

		// Make sure the input dimensions are OK.  The requested dimensions must be less than or equal to
		// our scratch render target size.
		check(InImageWidth <= RenderTargetTexture->GetSurfaceWidth());
		check(InImageHeight <= RenderTargetTexture->GetSurfaceHeight());

		RenderTargetResource = RenderTargetTexture->GameThread_GetRenderTargetResource();
	}
	check(RenderTargetResource != NULL);

	if (GShaderCompilingManager)
	{
		GShaderCompilingManager->ProcessAsyncResults(false, true);
	}

	// Create a canvas for the render target and clear it to black
	FCanvas Canvas(RenderTargetResource, NULL, FApp::GetCurrentTime() - GStartTime, FApp::GetDeltaTime(), FApp::GetCurrentTime() - GStartTime, GMaxRHIFeatureLevel);
	Canvas.Clear(FLinearColor::Black);


	// Get the rendering info for this object
	FThumbnailRenderingInfo* RenderInfo = GUnrealEd ? GUnrealEd->GetThumbnailManager()->GetRenderingInfo(InObject) : nullptr;

	// Wait for all textures to be streamed in before we render the thumbnail
	// @todo CB: This helps but doesn't result in 100%-streamed-in resources every time! :(
	if (InFlushMode == ThumbnailTools::EThumbnailTextureFlushMode::AlwaysFlush)
	{
		FlushAsyncLoading();

		IStreamingManager::Get().StreamAllResources(100.0f);
	}

	// If this object's thumbnail will be rendered to a texture on the GPU.
	bool bUseGPUGeneratedThumbnail = true;

	if (RenderInfo != NULL && RenderInfo->Renderer != NULL)
	{
		const float ZoomFactor = 1.0f;

		uint32 DrawWidth = InImageWidth;
		uint32 DrawHeight = InImageHeight;
		if (OutThumbnail)
		{
			// Find how big the thumbnail WANTS to be RenderCommandsChannel
			uint32 DesiredWidth = 0;
			uint32 DesiredHeight = 0;
			{
				// Currently we only allow textures/icons (and derived classes) to override our desired size
				// @todo CB: Some thumbnail renderers (like particles and lens flares) hard code their own
				//	   arbitrary thumbnail size even though they derive from TextureThumbnailRenderer
				if (RenderInfo->Renderer->IsA(UTextureThumbnailRenderer::StaticClass()))
				{
					RenderInfo->Renderer->GetThumbnailSize(
						InObject,
						ZoomFactor,
						DesiredWidth,		// Out
						DesiredHeight);	// Out
				}
			}

			// Does this thumbnail have a size associated with it?  Materials and textures often do!
			if (DesiredWidth > 0 && DesiredHeight > 0)
			{
				// Scale the desired size down if it's too big, preserving aspect ratio
				if (DesiredWidth > InImageWidth)
				{
					DesiredHeight = (DesiredHeight * InImageWidth) / DesiredWidth;
					DesiredWidth = InImageWidth;
				}
				if (DesiredHeight > InImageHeight)
				{
					DesiredWidth = (DesiredWidth * InImageHeight) / DesiredHeight;
					DesiredHeight = InImageHeight;
				}

				// Update dimensions
				DrawWidth = FMath::Max<uint32>(1, DesiredWidth);
				DrawHeight = FMath::Max<uint32>(1, DesiredHeight);
				OutThumbnail->SetImageSize(DrawWidth, DrawHeight);
			}
		}

		// Draw the thumbnail
		const int32 XPos = 0;
		const int32 YPos = 0;
		const bool bAdditionalViewFamily = false;
		RenderInfo->Renderer->Draw(
			InObject,
			XPos,
			YPos,
			DrawWidth,
			DrawHeight,
			RenderTargetResource,
			&Canvas,
			bAdditionalViewFamily
		);
	}

	// GPU based thumbnail rendering only
	if (bUseGPUGeneratedThumbnail)
	{
		// Tell the rendering thread to draw any remaining batched elements
		Canvas.Flush_GameThread();

		{
			ENQUEUE_RENDER_COMMAND(UpdateThumbnailRTCommand)(
				[RenderTargetResource](FRHICommandListImmediate& RHICmdList)
				{
					// Copy (resolve) the rendered thumbnail from the render target to its texture
					RHICmdList.CopyToResolveTarget(
						RenderTargetResource->GetRenderTargetTexture(),		// Source texture
						RenderTargetResource->TextureRHI,					// Dest texture
						FResolveParams());									// Resolve parameters
				});

			if (OutThumbnail)
			{
				const FIntRect InSrcRect(0, 0, OutThumbnail->GetImageWidth(), OutThumbnail->GetImageHeight());

				TArray<uint8>& OutData = OutThumbnail->AccessImageData();
				
				OutData.Empty();
				OutData.AddUninitialized(OutThumbnail->GetImageWidth() * OutThumbnail->GetImageHeight() * sizeof(FColor));

				// Copy the contents of the remote texture to system memory
				// NOTE: OutRawImageData must be a preallocated buffer!
				RenderTargetResource->ReadPixelsPtr((FColor*)OutData.GetData(), FReadSurfaceDataFlags(), InSrcRect);
			}

			// 导出一张资源缩略图
			if (RenderTargetResource)
			{
				FString  ImageName;
				InObject->GetName(ImageName);

				TArray<FColor> Colors;
				if (RenderTargetResource->ReadPixels(Colors))
				{
					RenderImage(InImageWidth, InImageHeight, (uint8*)Colors.GetData(), ImageName);
				}
			}
		}
	}
}

void UEditorUtility_Base::RenderImage(int32 Width, int32 Height, const uint8* Data, const FString& ImageName, bool bSaveJpgOnly)
{
	if (Data)
	{
		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
		TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::JPEG);
		if (ImageWrapper.IsValid())
		{
			ImageWrapper->SetRaw(Data, Width * Height * sizeof(FColor), Width, Height, ERGBFormat::BGRA, 8);
			FString Filename = FString::Printf(TEXT("%sThumbnailImage/%s.jpg"), *FPaths::ProjectSavedDir(), *FPaths::GetBaseFilename(ImageName, true));
			FFileHelper::SaveArrayToFile(ImageWrapper->GetCompressed(100), *Filename);
		}
	}
}

void UEditorUtility_Base::GetThumbnailInEditor(FString AssetPath)
{
	UObject* Asset = LoadObject<UObject>(nullptr, *AssetPath);
	if(!Asset)
	{
		return;
	}
	RenderThumbnail(Asset, 512, 512, ThumbnailTools::EThumbnailTextureFlushMode::NeverFlush, nullptr, nullptr);

	FString path = FPaths::Combine(FPaths::ProjectSavedDir(),"ThumbnailImage/");
	FString filename = path + FPaths::GetBaseFilename(AssetPath)+".jpg";
	
	// 创建纹理对象
	UTexture2D* Texture = FImageUtils::ImportFileAsTexture2D(filename);
	if (Texture)
	{
		FSlateBrush* ImageBrush = new FSlateBrush();
		ImageBrush->SetResourceObject(Texture);
		ImageBrush->ImageSize.X = Texture->GetSizeX();
		ImageBrush->ImageSize.Y = Texture->GetSizeY();
		
		TSharedPtr<SWindow> Window =
				SNew(SWindow)
				.Title(FText::FromString(TEXT("Texture Window")))
				.ClientSize(FVector2D(Texture->GetSizeX(), Texture->GetSizeY()))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Fill)
					.HAlign(HAlign_Fill)
					[
						SNew(SImage)
						.Image(ImageBrush)
					]
				];
		FSlateApplication::Get().AddWindow(Window.ToSharedRef());
	}
}

void UEditorUtility_Base::InitEditorWidget_CPP()
{
	BlueprintClassObj = LoadClass<UCustomAnimInstance>(nullptr,TEXT("/MenuTool/Body/Body_MenuTool.Body_MenuTool_C"));
	if (BlueprintClassObj != nullptr)
	{
	}

	FaceAnimBlueprintClass = LoadClass<UFaceAnimInstance>(nullptr,TEXT("/MenuTool/Face/Face_MenuTool.Face_MenuTool_C"));
	if (FaceAnimBlueprintClass != nullptr)
	{
	}
}

void UEditorUtility_Base::UpdateAnimBlueprint_CPP()
{
	UCommonFunctionLibrary::CopyToCDO(BodySketal->GetAnimInstance());
	UCommonFunctionLibrary::CopyToCDO(FaceSketal->GetAnimInstance());
}

void UEditorUtility_Base::CreateActor_CPP(AActor* ParentSelected)
{
	if (ParentSelected == nullptr)return;
	/*if (ParentSelected != nullptr)*/
	{
		SelectedActor_CPP = ParentSelected;
		TInlineComponentArray<USkeletalMeshComponent*> Components;
		SelectedActor_CPP->GetComponents(Components);
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

	CreateBodyAnimationBlueprint();
	CreateFaceAnimationBlueprint();

	TInlineComponentArray<UAnimControlComponent*> AnimComponents;
	SelectedActor_CPP->GetComponents(AnimComponents);
	if (AnimComponents.IsValidIndex(0))
	{
		AnimControl = AnimComponents[0];
		AnimControl->InitializeInEditor(BodySketal, FaceSketal);
	}
}


void UEditorUtility_Base::SetSelectedActor_CPP(AActor* Selected)
{
	if (Selected == nullptr /*|| Selected == SelectedActor_CPP*/)
	{
		return;
	}

	SelectedActor_CPP = Selected;

	RefreshComponents();
}


void UEditorUtility_Base::OnValueChanged_CPP(FString Section, double Value)
{
	// RefreshComponents();
	if (AnimControl)
	{
		AnimControl->ApplyBoneInput(Section, Value);
	}
}

void UEditorUtility_Base::ChangePose_CPP(int Index)
{
	// RefreshComponents();
	if (AnimControl)
	{
		AnimControl->SetActivateChildIndex(Index);
	}
}

void UEditorUtility_Base::SelectActorInEditor(AActor* ActorToSelect)
{
	if (GEditor != nullptr && ActorToSelect != nullptr)
	{
		// 清除当前的选中集合
		GEditor->SelectNone(true, true, false);

		// 将目标Actor设置为选中状态
		GEditor->SelectActor(ActorToSelect, true, true, true);
	}
}

// void UEditorUtility_Base::LoadAssetByDir()
// {
// 	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
// 	TArray<FAssetData> AssetDatas;
// 	FName DirectoryPath = TEXT("/MenuTool/Body");
// 	
// 	AssetRegistryModule.Get().GetAssetsByPath(DirectoryPath, AssetDatas, true /*bRecursive*/);
// 	for (const FAssetData& AssetData : AssetDatas)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Found Asset: %s"), *AssetData.AssetName.ToString());
// 	}
// }

void UEditorUtility_Base::SaveChange()
{
	SaveAnimBlueprintAsset(BodySketal->GetAnimInstance());
	SaveAnimBlueprintAsset(FaceSketal->GetAnimInstance());
}

void UEditorUtility_Base::CreateAssetSelector(USkeletalMeshComponent* Component)
{
#pragma region CreateAssetSelector
	/*
	// 创建一个资源类型过滤器
	FAssetData InitialData;
	FAssetData CurrentData;
	TSharedPtr<SWidget> AssetSelector;

	FAssetData Value;
	UObject* ObjectValue = nullptr;

	FAssetPickerConfig AssetPickerConfig;
	// 使用 AddUnique 以确保类名不重复
	FName AnimBPClassName = UAnimBlueprint::StaticClass()->GetFName();
	AssetPickerConfig.Filter.ClassNames.AddUnique(AnimBPClassName);

	AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateLambda([&](const FAssetData& AssetData)
	{
		ObjectValue = AssetData.GetAsset();
	});
	
	// 准备类指针数组用于 GetNewAssetFactoriesForClasses
	TArray<const UClass*> ClassesToFilter;
	ClassesToFilter.Add(UAnimBlueprint::StaticClass());

	// AssetSelector = PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
	// 	InitialData,
	// 	true,
	// 	ClassesToFilter,
	// 	PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(ClassesToFilter),
	// 	FOnShouldFilterAsset(),
	// 	AssetPickerConfig.OnAssetSelected,
	// 	FSimpleDelegate()
	// 	);

	FOnGetAllowedClasses OnGetAllowedClasses = FOnGetAllowedClasses::CreateLambda([&](TArray<const UClass*>& Classes)
	{
		Classes = ClassesToFilter;
	});
	
	FOnAssetSelected OnAssetSelected = FOnAssetSelected::CreateLambda([&](const FAssetData& AssetData)
	{
		ObjectValue = AssetData.GetAsset();
	});
	
	AssetSelector = PropertyCustomizationHelpers::MakeAssetPickerAnchorButton(OnGetAllowedClasses,OnAssetSelected);
	
	// 创建Slate界面
	TSharedRef<SVerticalBox> VerticalBox = SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Select an Animation Blueprint:")))
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5)
		[
			AssetSelector.ToSharedRef()
		];

	TSharedRef<SWindow> Window = SNew(SWindow)
		.Title(FText::FromString(TEXT("My Asset Selector")))
		.ClientSize(FVector2D(400, 200))
		[
			VerticalBox
		];

	FSlateApplication::Get().AddWindow(Window);
	*/
#pragma endregion

	FAnimationRecorder Recorder;
	Recorder.TriggerRecordAnimation(Component);
}

void UEditorUtility_Base::SaveAnimBlueprintAsset(UAnimInstance* InAnimInstance)
{
	UBlueprint* Blueprint = (InAnimInstance != nullptr) ? Cast<UBlueprint>(InAnimInstance->GetClass()->ClassGeneratedBy) : nullptr;
	if(Blueprint)
	{
		FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
		FKismetEditorUtilities::CompileBlueprint(Blueprint);
		
		UPackage* package = Blueprint->GetOutermost();
		UClass* Myclass = Blueprint->GetBlueprintClass();
		FString packageFileName = FPackageName::LongPackageNameToFilename(
			package->GetName(), FPackageName::GetAssetPackageExtension());
		bool bSaved = UPackage::SavePackage(package, nullptr, EObjectFlags::RF_Standalone, *packageFileName, GError,
											nullptr, true, true, SAVE_NoError);

		// Set up a notification record to indicate success/failure
		FNotificationInfo NotificationInfo(FText::GetEmpty());
		NotificationInfo.FadeInDuration = 1.0f;
		NotificationInfo.FadeOutDuration = 2.0f;
		NotificationInfo.bUseLargeFont = false;
		SNotificationItem::ECompletionState CompletionState;

		FFormatNamedArguments Args;
		Args.Add(TEXT("AssetName"), FText::FromString(Blueprint->GetName()));
		
		if(bSaved)
		{

			NotificationInfo.Text = FText::Format(LOCTEXT("UEditorUtility_Base", "{AssetName} 保存成功!"), Args);
			CompletionState = SNotificationItem::CS_Success;
		}
		else
		{
			NotificationInfo.Text = LOCTEXT("UEditorUtility_Base", "{AssetName} 保存失败!");
			CompletionState = SNotificationItem::CS_Fail;
		}
		

		// Add the notification to the queue
		const TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(NotificationInfo);
		Notification->SetCompletionState(CompletionState);
	}
}

void UEditorUtility_Base::RefreshComponents()
{
	if (SelectedActor_CPP)
	{
		TInlineComponentArray<USkeletalMeshComponent*> Components;
		SelectedActor_CPP->GetComponents(Components);
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

		TInlineComponentArray<UAnimControlComponent*> AnimComponents;
		SelectedActor_CPP->GetComponents(AnimComponents);
		if (AnimComponents.IsValidIndex(0))
		{
			AnimControl = AnimComponents[0];
			AnimControl->Initialize(BodySketal, FaceSketal);
		}
	}
	else
	{
		ResetComponents();
	}
}

void UEditorUtility_Base::ResetComponents()
{
	BodySketal = nullptr;
	FaceSketal = nullptr;
	AnimControl = nullptr;
	// ChangeClass = nullptr;
}

void UEditorUtility_Base::CreateAnimationBlueprint(USkeletalMeshComponent* SkeletalComponent,
					UClass* Class)
{
	if (!SkeletalComponent)return;
	FString Name;
	FString PackageName;
	IAssetTools::Get().CreateUniqueAssetName(Class->GetOutermost()->GetName(),TEXT("_Child"), PackageName,
											 Name);
	FString PackagePath = FPackageName::GetLongPackagePath(PackageName);
	//PackagePath = FPaths::Combine(PackagePath,RelativeDir);
	UE_LOG(LogTemp, Log, TEXT("生成的全新资源：%s"), *PackagePath);
	UClass* TargetParentClass = Class;
	
	if (!FKismetEditorUtilities::CanCreateBlueprintOfClass(TargetParentClass))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("类无效")));
		return;
	}
	
	UAnimBlueprintFactory* AnimBlueprintFactory = NewObject<UAnimBlueprintFactory>();
	AnimBlueprintFactory->BlueprintType = BPTYPE_Normal;
	AnimBlueprintFactory->ParentClass = TSubclassOf<UAnimInstance>(Class);
	AnimBlueprintFactory->TargetSkeleton = SkeletalComponent->GetAnimInstance()->CurrentSkeleton;
	AnimBlueprintFactory->bTemplate = false;
	AnimBlueprintFactory->SupportedClass = UAnimBlueprint::StaticClass();

	FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
	UAnimBlueprint* NewAsset = CastChecked<UAnimBlueprint>(
		AssetToolsModule.Get().CreateAsset(Name, PackagePath, UAnimBlueprint::StaticClass(), AnimBlueprintFactory));
	NewAsset->SetPreviewAnimationBlueprint(NewAsset);
	NewAsset->OnCompiled();
	NewAsset->BroadcastCompiled();
	
	if (NewAsset)
	{
		UPackage* package = NewAsset->GetOutermost();
		UClass* Myclass = NewAsset->GetBlueprintClass();
		FString packageFileName = FPackageName::LongPackageNameToFilename(
			package->GetName(), FPackageName::GetAssetPackageExtension());
		bool bSaved = UPackage::SavePackage(package, nullptr, EObjectFlags::RF_Standalone, *packageFileName, GError,
											nullptr, true, true, SAVE_NoError);
		if (bSaved)
		{
			UE_LOG(LogTemp, Log, TEXT("新创建的动画蓝图保存成功：%s"), *package->GetName());
		}

		SkeletalComponent->ClearAnimScriptInstance();
		SkeletalComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		SkeletalComponent->SetAnimClass(NewAsset->GeneratedClass);
		SkeletalComponent->SetAnimInstanceClass(NewAsset->GeneratedClass);
	}
}

void UEditorUtility_Base::CreateBodyAnimationBlueprint()
{
	CreateAnimationBlueprint(BodySketal,BlueprintClassObj);
}

void UEditorUtility_Base::CreateFaceAnimationBlueprint()
{
	CreateAnimationBlueprint(FaceSketal,FaceAnimBlueprintClass);
}

void UEditorUtility_Base::Reba_ScoreA_Parts(int &NeckScore,int& LegsScore,int& TrunkScore,int& LoadScore,int neckPitch, bool neckTwisted, bool neckSideBend, int trunkPitch, bool trunkTwisted, bool trunkSideBend, bool bothLegsDown, int kneeBend, float load_Kg, bool bShockForce)
{
	int idxNeck = 0;
	int idxLegs = 0;
	int idxTrunk = 0;

	// Neck Index
	if (neckPitch > 0 && neckPitch < 20)
	{
		idxNeck += 1;
	}
	else
	{
		idxNeck += 2;
	}
	if (neckTwisted) idxNeck += 1;
	if (neckSideBend) idxNeck += 1;

	// Trunk Index
	if (trunkPitch == 0) idxTrunk += 1;
	else if (trunkPitch > -20 && trunkPitch < 20) idxTrunk += 2;
	else if (trunkPitch >= 20 && trunkPitch < 60) idxTrunk += 3;
	else if (trunkPitch < -20) idxTrunk += 3;
	else if (trunkPitch >= 60) idxTrunk += 4;

	if (trunkTwisted) idxTrunk += 1;
	if (trunkSideBend) idxTrunk += 1;

	// Leg Index
	if (bothLegsDown) idxLegs += 1;
	else idxLegs += 2;
	if (kneeBend > 30 && kneeBend < 60) idxLegs += 1;
	else if (kneeBend >= 60) idxLegs += 2;

	int ForceLoadScore = 0;
	if (load_Kg >5 && load_Kg < 10)
	{
		ForceLoadScore += 1;
	}
	else if (load_Kg >= 10)
	{
		ForceLoadScore += 2;
	}

	if (bShockForce) ForceLoadScore += 1;

	LegsScore = idxLegs;
	NeckScore = idxNeck;  LegsScore = idxLegs; TrunkScore = idxTrunk; LoadScore = ForceLoadScore;

	return;
}

void UEditorUtility_Base::Reba_ScoreB_Parts(int &idxUpperArmScore ,int &idxLowerArmScore,int& idxWristScore, int upperArmPosition, bool bShouldRaised, bool bUpperArmAbducted, bool bArmSupported, int lowerArmPosition, int wristPosition, bool bWristBendTwisted, int couplingScore)
{
	int idxUpperArm = 0;
	int idxLowerArm = 0;
	int idxWrist = 0;

	if (upperArmPosition > -20 && upperArmPosition < 20)
	{
		idxUpperArm += 1;
	}
	else if (upperArmPosition <= -20)
	{
		idxUpperArm += 2;
	}
	else if (upperArmPosition >= 20 && upperArmPosition < 45)
	{
		idxUpperArm += 2;
	}
	else if (upperArmPosition >= 45 && upperArmPosition < 90)
	{
		idxUpperArm += 3;
	}
	else if (upperArmPosition >= 90)
	{
		idxUpperArm += 4;
	}

	if (bShouldRaised)
	{
		idxUpperArm += 1;
	}

	if (bUpperArmAbducted)
	{
		idxUpperArm += 1;
	}

	if (bArmSupported)
	{
		idxUpperArm -= 1;
	}

	// Lower Arm
	if (lowerArmPosition >= 60 && lowerArmPosition < 100)
	{
		idxLowerArm += 1;
	}
	else if (lowerArmPosition >= 0 && lowerArmPosition < 60)
	{
		idxLowerArm += 2;
	}
	else if (lowerArmPosition >= 100)
	{
		idxLowerArm += 2;
	}

	// Wrist

	if (wristPosition >= -15 && wristPosition < 15)
	{
		idxWrist += 1;
	}
	else
	{
		idxWrist += 2;
	}

	if (bWristBendTwisted)
	{
		idxWrist += 1;
	}

	idxLowerArmScore = idxLowerArm;
	idxUpperArmScore = idxUpperArm;
	idxWristScore = idxWrist;
}

void UEditorUtility_Base::Rula_WristArmScore(int &idxUpperArmScore,
int &idxLowerArmScore,
int &idxWristScore,
int &idxWristTwistScore,
int &loadScoreScore, int upperArmPosition, bool bShoulderRaised, bool bUpperArmAbducted, bool bArmSupported, int lowerArmPosition, bool bArmOffset, int wristPosition, bool bWristBent, bool bWristTwisted, bool bWristEndofRange, int muscleScore, float load, bool bLoadIsStaticOrRepeated)
{
	int idxUpperArm = 0;
	int idxLowerArm = 0;
	int idxWrist = 0;
	int idxWristTwist = 0;
	int loadScore = 0;
	if (upperArmPosition >= -20 && upperArmPosition < 20)
	{
		idxUpperArm += 1;
	}
	else if (upperArmPosition < -20 || (upperArmPosition >= 20 && upperArmPosition < 45))
	{
		idxUpperArm += 2;
	}
	else if (upperArmPosition >= 45 && upperArmPosition < 90)
	{
		idxUpperArm += 3;
	}
	else if (upperArmPosition >= 90 && upperArmPosition <= 180)
	{
		idxUpperArm += 4;
	}

	if (bShoulderRaised) idxUpperArm += 1;
	if (bUpperArmAbducted) idxUpperArm += 1;
	if (bArmSupported) idxUpperArm -= 1;

	// LowerArm
	
	if (lowerArmPosition >= 60 && lowerArmPosition < 100)
	{
		idxLowerArm += 1;
	}
	else if (lowerArmPosition >= 0 && lowerArmPosition < 60)
	{
		idxLowerArm += 2;
	}
	else if (lowerArmPosition >= 100)
	{
		idxLowerArm += 2;
	}
	if (bArmOffset)
	{
		idxLowerArm += 1;
	}

	// Wrist
	
	if (wristPosition == 0)
	{
		idxWrist += 1;
	}
	else if (wristPosition >= -15 && wristPosition < 15)
	{
		idxWrist += 2;
	}
	else if (wristPosition < -15 || wristPosition > 15)
	{
		idxWrist += 3;
	}
	if (bWristBent) idxWrist += 1;

	// WristTwist
	
	if (bWristTwisted) idxWristTwist += 1;
	if (bWristEndofRange) idxWristTwist += 1;

	//int result = TableA[idxUpperArm - 1][idxLowerArm - 1][idxWrist - 1][idxWristTwist - 1];

	//result += muscleScore;

	
	if (load >= 2 && load < 10)
	{
		if (bLoadIsStaticOrRepeated)
			loadScore += 2;
		else
			loadScore += 1;
	}
	else if (load >=10)
	{
		loadScore += 3;
	}
	idxUpperArmScore = idxUpperArm;
	idxLowerArmScore = idxLowerArm;
	idxWristScore = idxWrist;
	idxWristTwistScore = idxWristTwist;
	loadScoreScore = loadScore;
		

	//result += loadScore;

	//wristArmScore = result;
}

void UEditorUtility_Base::Rula_NeckTrunkLegScore(int& muscleScoreScore,
int &idxNeckPostureScore,
int& idxTrunkPostureScore,
int &idxLegsScore, int neckPosition, bool bNeckTwisted, bool bNeckSideBent, int trunkPosition, bool bTrunkTwisted, bool bTrunkSideBent, bool bLegsAreSupported, bool bPostureStaticOrRepeated, float load, bool bLoadIsStaticOrRepeated)
{
	int muscleScore = 0;
	int idxNeckPosture = 0;
	int idxTrunkPosture = 0;
	int idxLegs = 0;

	// Neck
	if (neckPosition >= 0 && neckPosition < 10)
	{
		idxNeckPosture += 1;
	}
	else if (neckPosition >= 10 && neckPosition < 20)
	{
		idxNeckPosture += 2;
	}
	else if (neckPosition >= 20)
	{
		idxNeckPosture += 3;
	}
	else if (neckPosition < 0)
	{
		idxNeckPosture += 4;
	}

	if (bNeckTwisted) idxNeckPosture += 1;
	if (bNeckSideBent) idxNeckPosture += 1;

	// Trunk
	if (trunkPosition == 0)
	{
		idxTrunkPosture += 1;
	}
	else if (trunkPosition > 0 && trunkPosition < 20)
	{
		idxTrunkPosture += 2;
	}
	else if (trunkPosition >= 20 && trunkPosition < 60)
	{
		idxTrunkPosture += 3;
	}
	else if (trunkPosition >= 60)
	{
		idxTrunkPosture += 4;
	}

	if (bTrunkTwisted) idxTrunkPosture += 1;
	if (bTrunkSideBent) idxTrunkPosture += 1;

	//Legs
	if (bLegsAreSupported)
	{
		idxLegs += 1;
	}
	else
	{
		idxLegs += 2;
	}

	//int tableBScore = TableB[idxNeckPosture - 1][idxTrunkPosture - 1][idxLegs - 1];

	
	if (bPostureStaticOrRepeated) muscleScore += 1;

	int forceScore = 0;
	if (load >= 2 && load < 10)
	{
		if (bLoadIsStaticOrRepeated)
		{
			forceScore += 2;
		}
		else
		{
			forceScore += 1;
		}
	}
	else if (load >= 10)
	{
		forceScore += 3;
	}

	muscleScoreScore = muscleScore;
	idxNeckPostureScore=idxNeckPosture ;				
	idxTrunkPostureScore = idxTrunkPosture;
	idxLegsScore = idxLegs;

}
