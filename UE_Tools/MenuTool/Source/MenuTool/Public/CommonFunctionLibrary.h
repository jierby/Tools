// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimControlComponent.h"
#include "AssetToolsModule.h"
#include "CustomAnimInstance.h"
#include "IAssetTools.h"
#include "PersonaUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "BoneControllers/AnimNode_AnimDynamics.h"
#include "Factories/AnimBlueprintFactory.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/FileHelper.h"
#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "CommonFunctionLibrary.generated.h"

DECLARE_DELEGATE_TwoParams(FOnPoseValueCommitted, const FString&, float);//姿势名称，自定义值改变
DECLARE_DELEGATE_TwoParams(FPoseValueInput, FString, float);//姿势数据输入
DECLARE_DELEGATE_TwoParams(FOnSexValueChanged, ETypeSlate, ESex);//控件索引，性别索引
DECLARE_DELEGATE_TwoParams(FOnSkinColorChanged, ETypeSlate, ESkin_Color);//控件索引，肤色索引
DECLARE_DELEGATE_TwoParams(FHumanCreate_HWeight, float, float);//身高体重创建：H,W
DECLARE_DELEGATE_TwoParams(FHumanCreate_Percent, ETypeSlate, float);//百分位索引，
DECLARE_DELEGATE_OneParam(FHumanCreate_Custom, ETypeSlate);//创建自定义人体，
DECLARE_DELEGATE_OneParam(FHumanCreate_Reset, ETypeSlate);//重置自定义人体，

#define LOCTEXT_NAMESPACE "CommonFunctionLibrary"

//De
//DECLARE_DELEGATE_TwoParams();//标准索引
//地区索引
//名称设置




UENUM(BlueprintType)
enum class ECustom : uint8
{
	_df,
	_fd,
};

UENUM(BlueprintType)
enum class ETypeSlate : uint8
{
	Custom,
	Percent
};

UENUM(BlueprintType)
enum class ESex : uint8
{
	Male ,
	Female 
};
UENUM(BlueprintType)
enum class ESkin_Color : uint8
{
	Yellow,
	White,
	Black
};



USTRUCT(BlueprintType)
struct FPoseValue
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuTool")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuTool")
		int32 Value;
};

FTimerHandle handle;


USTRUCT(BlueprintType)
struct FCSVStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "csv")
		TArray<FString> ExampleString1;
	


};

/**
 * 
 */
UCLASS()
class MENUTOOL_API UCommonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:


UFUNCTION(BlueprintCallable)
		static bool  ExportDataToCSV(const TArray<FString>& ColumnNames, const TArray<FCSVStruct>& Rows, const FString& FilePath)
	{

		FString OutFilePath = TEXT(""); // 初始化输出路径为空字符串

		// 获取桌面平台接口
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		if (DesktopPlatform != nullptr)
		{
			// 初始化保存文件对话框参数
			//auto* ParentWindowHandle = nullptr;
			// 获取游戏主窗口的句柄
			auto* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);

			// 设置文件对话框的初始目录
			FString InitialDirectory = FPaths::ProjectDir();

			// 弹出保存文件对话框
			TArray<FString> OutFilenames;
			bool bSave = DesktopPlatform->SaveFileDialog(
				ParentWindowHandle,
				TEXT("Save"),
				InitialDirectory,
				TEXT("1.csv"),
				TEXT("csv"),
				EFileDialogFlags::None,
				OutFilenames
			);

			if (bSave && OutFilenames.Num() > 0)
			{
				// 选择了文件路径并确认保存
				OutFilePath = OutFilenames[0];
			}
			else
			{
				return false;
			}
		}


		const ANSICHAR UTF8BOM[] = "\xEF\xBB\xBF";
		FString CSVContent;
		//CSVContent += UTF8BOM;
		// 添加列名
		CSVContent += FString::Join(ColumnNames, TEXT(",")) + TEXT("\n");

		// 添加行数据
		for (const FCSVStruct& Row : Rows)
		{
			CSVContent += FString::Join(Row.ExampleString1, TEXT(",")) + TEXT("\n");
		}

		// 保存到文件
		return FFileHelper::SaveStringToFile(CSVContent, *OutFilePath,FFileHelper::EEncodingOptions::ForceUTF8);
	}


	UFUNCTION(BlueprintCallable)
	static void UpdateAnimation(AActor*Actor,UClass* Body,UClass* Face)
	{
		if(Actor==nullptr)return;
		TInlineComponentArray<USkeletalMeshComponent*> Components;
		Actor->GetComponents(Components);
		USkeletalMeshComponent* BodySketal=nullptr;
		USkeletalMeshComponent* FaceSketal=nullptr;
		UClass*Body_1;
		UClass*Face_1;
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
		if (!BodySketal || !FaceSketal)return;
		
		if(Body == nullptr && Face == nullptr)
		{
			if(BodySketal->GetAnimClass() == nullptr)
				return;
			if(FaceSketal->GetAnimClass() == nullptr)
				return;
			Face_1 = FaceSketal->GetAnimClass()->GetClass();
			Body_1 = BodySketal->GetAnimClass()->GetClass();
		}
		else
		{
			Body_1 = Body;
			Face_1 = Face;
		}
		UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
		Actor->Modify();

		BodySketal->SetAnimClass(nullptr);
		FaceSketal->SetAnimClass(nullptr);

		auto callback = [BodySketal,FaceSketal,Body_1,Face_1]()
		{
			UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();

			BodySketal->SetAnimClass(Body_1);
			BodySketal->SetAnimInstanceClass(Body_1);
			BodySketal->SetUpdateAnimationInEditor(true);

			FaceSketal->SetAnimClass(Face_1);
			FaceSketal->SetAnimInstanceClass(Face_1);
			FaceSketal->SetUpdateAnimationInEditor(true);

			EditorWorld->GetTimerManager().ClearTimer(handle);
		};
		FTimerDelegate timedelegate;
		timedelegate.BindLambda(callback);

		handle = EditorWorld->GetTimerManager().SetTimerForNextTick(timedelegate);
	}

	UFUNCTION(BlueprintCallable)
	static void CopyToCDO(UAnimInstance* InAnimInstance)
	{
		if(InAnimInstance==nullptr)
			return;
	
		const int32 NumChangedProperties = PersonaUtils::CopyPropertiesToCDO(InAnimInstance,PersonaUtils::ECopyOptions::Default);
		UBlueprint* Blueprint = (InAnimInstance != nullptr) ? Cast<UBlueprint>(InAnimInstance->GetClass()->ClassGeneratedBy) : nullptr;
		if(Blueprint)
		{
			FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
			FKismetEditorUtilities::CompileBlueprint(Blueprint);
			
			if (NumChangedProperties > 0)
			{
				// Set up a notification record to indicate success/failure
				FNotificationInfo NotificationInfo(FText::GetEmpty());
				NotificationInfo.FadeInDuration = 1.0f;
				NotificationInfo.FadeOutDuration = 2.0f;
				NotificationInfo.bUseLargeFont = false;
				SNotificationItem::ECompletionState CompletionState;
				
				if (NumChangedProperties > 1)
				{
					FFormatNamedArguments Args;
					// Args.Add(TEXT("BlueprintName"), FText::FromName(Blueprint->GetFName()));
					Args.Add(TEXT("NumChangedProperties"), NumChangedProperties);
					Args.Add(TEXT("ActorName"), FText::FromString(Blueprint->GetName()));
					NotificationInfo.Text = FText::Format(LOCTEXT("CommonFunctionLibrary", "({NumChangedProperties} 个 {ActorName} 属性发生变更)."), Args);
				}
				else
				{
					FFormatNamedArguments Args;
					// Args.Add(TEXT("BlueprintName"), FText::FromName(Blueprint->GetFName()));
					Args.Add(TEXT("ActorName"), FText::FromString(Blueprint->GetName()));
					NotificationInfo.Text = FText::Format(LOCTEXT("CommonFunctionLibrary", "(1 个 {ActorName} 属性发生变更)."), Args);
				}
				CompletionState = SNotificationItem::CS_Success;
				// Add the notification to the queue
				const TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(NotificationInfo);
				Notification->SetCompletionState(CompletionState);
			}
		}
	}

	UFUNCTION(BlueprintCallable)
	static void ApplyChangesToBP(AActor* ActorContex)
	{
		int32 NumChangedProperties = 0;

		AActor* Actor = ActorContex;
		UBlueprint* Blueprint = (Actor != nullptr) ? Cast<UBlueprint>(Actor->GetClass()->ClassGeneratedBy) : nullptr;

		if (Actor != NULL && Blueprint != NULL && Actor->GetClass()->ClassGeneratedBy == Blueprint)
		{
			// Cache the actor label as by the time we need it, it may be invalid
			const FString ActorLabel = Actor->GetActorLabel();
			// FXGTARestoreSelectedInstanceComponent RestoreSelectedInstanceComponent;
			{
				Actor->Modify();

				// Mark components that are either native or from the SCS as modified so they will be restored
				for (UActorComponent* ActorComponent : Actor->GetComponents())
				{
					if (ActorComponent && (ActorComponent->CreationMethod == EComponentCreationMethod::SimpleConstructionScript || ActorComponent->CreationMethod == EComponentCreationMethod::Native))
					{
						ActorComponent->Modify();
					}
				}

				// Perform the actual copy
				{
					AActor* BlueprintCDO = Actor->GetClass()->GetDefaultObject<AActor>();
					if (BlueprintCDO != NULL)
					{
						const EditorUtilities::ECopyOptions::Type CopyOptions = (EditorUtilities::ECopyOptions::Type)(EditorUtilities::ECopyOptions::OnlyCopyEditOrInterpProperties | EditorUtilities::ECopyOptions::PropagateChangesToArchetypeInstances | EditorUtilities::ECopyOptions::SkipInstanceOnlyProperties);
						NumChangedProperties = EditorUtilities::CopyActorProperties(Actor, BlueprintCDO, CopyOptions);
						if (Actor->GetInstanceComponents().Num() > 0)
						{
							// RestoreSelectedInstanceComponent.Save(Actor);
							FKismetEditorUtilities::AddComponentsToBlueprint(Blueprint, Actor->GetInstanceComponents());
							NumChangedProperties += Actor->GetInstanceComponents().Num();
							Actor->ClearInstanceComponents(true);
						}
						if (NumChangedProperties > 0)
						{
							Actor = nullptr; // It is unsafe to use Actor after this point as it may have been reinstanced, so set it to null to make this obvious
						}
					}
				}
			}

			// Compile the BP outside of the transaction
			if (NumChangedProperties > 0)
			{
				FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
				FKismetEditorUtilities::CompileBlueprint(Blueprint);
				// RestoreSelectedInstanceComponent.Restore();
			}

			// Set up a notification record to indicate success/failure
			FNotificationInfo NotificationInfo(FText::GetEmpty());
			NotificationInfo.FadeInDuration = 1.0f;
			NotificationInfo.FadeOutDuration = 2.0f;
			NotificationInfo.bUseLargeFont = false;
			SNotificationItem::ECompletionState CompletionState;
			if (NumChangedProperties > 0)
			{
				if (NumChangedProperties > 1)
				{
					FFormatNamedArguments Args;
					Args.Add(TEXT("BlueprintName"), FText::FromName(Blueprint->GetFName()));
					Args.Add(TEXT("NumChangedProperties"), NumChangedProperties);
					Args.Add(TEXT("ActorName"), FText::FromString(ActorLabel));
					NotificationInfo.Text = FText::Format(LOCTEXT("CommonFunctionLibrary", "Updated Blueprint {BlueprintName} ({NumChangedProperties} property changes applied from actor {ActorName})."), Args);
				}
				else
				{
					FFormatNamedArguments Args;
					Args.Add(TEXT("BlueprintName"), FText::FromName(Blueprint->GetFName()));
					Args.Add(TEXT("ActorName"), FText::FromString(ActorLabel));
					NotificationInfo.Text = FText::Format(LOCTEXT("CommonFunctionLibrary", "Updated Blueprint {BlueprintName} (1 property change applied from actor {ActorName})."), Args);
				}
				CompletionState = SNotificationItem::CS_Success;
			}
			else
			{
				NotificationInfo.Text = LOCTEXT("CommonFunctionLibrary", "No properties were copied");
				CompletionState = SNotificationItem::CS_Fail;
			}

			// Add the notification to the queue
			const TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(NotificationInfo);
			Notification->SetCompletionState(CompletionState);
		}
	}
	
	UFUNCTION(BlueprintCallable)
	static void CreateAnimationClass(AActor*Actor,UClass* &Body,UClass*& Face/*,TSubclassOf<AActor> BlueprintActorClass,UClass*BlueprintClassObj_,FString Name,FString PackageName*/)
	{
		if(Actor==nullptr)return;
		// FActorSpawnParameters ActorParms;
		// // TSubclassOf<AActor> BlueprintActorClass;
		// FVector SpawnLocation = Actor==nullptr? FVector(0,0,0):Actor->GetActorLocation()+FVector(100,0,0);
		// FRotator SpawnRotation = Actor==nullptr? FRotator(0,0,0):Actor->GetActorRotation();
		//
		// AActor*ActorSpawn =  GEditor->GetEditorWorldContext().World()->SpawnActor<AActor>(BlueprintActorClass,SpawnLocation,SpawnRotation);
		//

		UClass* BlueprintClassObj = LoadClass<UCustomAnimInstance>(nullptr,TEXT("/MenuTool/Body.Body_C"));
		if(BlueprintClassObj == nullptr) return;
		// Body= BlueprintClassObj;
		UClass* FaceClassObj = LoadClass<UFaceAnimInstance>(nullptr,TEXT("/MenuTool/Face.Face_C"));
		if(FaceClassObj ==nullptr) return;;
		Face = FaceClassObj;
		
		TInlineComponentArray<USkeletalMeshComponent*> Components;
		Actor->GetComponents(Components);
		USkeletalMeshComponent* BodySketal=nullptr;
		USkeletalMeshComponent* FaceSketal=nullptr;
		UAnimControlComponent* AnimControl=nullptr;
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
		if (!BodySketal)return;
		FString Name="";
		FString PackageName="";
		IAssetTools::Get().CreateUniqueAssetName(BlueprintClassObj->GetOutermost()->GetName(), TEXT("_Child"), PackageName, Name);
		FString PackagePath = FPackageName::GetLongPackagePath(PackageName);
		UE_LOG(LogTemp, Log, TEXT("生成的全新资源：%s"), *PackagePath);
		// UClass* TargetParentClass = BlueprintClassObj;
		if (!FKismetEditorUtilities::CanCreateBlueprintOfClass(BlueprintClassObj))
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT( "无法创建动画蓝图")));
			return;
		}
		UAnimBlueprintFactory* AnimBlueprintFactory = NewObject<UAnimBlueprintFactory>();
		AnimBlueprintFactory->BlueprintType = BPTYPE_Normal;
        AnimBlueprintFactory->ParentClass = TSubclassOf<UCustomAnimInstance>(BlueprintClassObj);
		AnimBlueprintFactory->TargetSkeleton = BodySketal->GetAnimInstance()->CurrentSkeleton;
        AnimBlueprintFactory->bTemplate = false;
		AnimBlueprintFactory->SupportedClass = UAnimBlueprint::StaticClass();
		
		FAssetToolsModule& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools");
        	UAnimBlueprint* NewAsset = CastChecked<UAnimBlueprint>(AssetToolsModule.Get().CreateAsset(Name, PackagePath, UAnimBlueprint::StaticClass(), AnimBlueprintFactory));
        	NewAsset->SetPreviewAnimationBlueprint(NewAsset);
        	NewAsset->OnCompiled();
        	NewAsset->BroadcastCompiled();
        	if (NewAsset)
        	{
        		// AnimBlueprint = NewAsset;
        		Body = NewAsset->GeneratedClass;
        		UPackage* package = NewAsset->GetOutermost();
        		UClass* Myclass = NewAsset->GetBlueprintClass();
        
        		FString packageFileName = FPackageName::LongPackageNameToFilename(package->GetName(), FPackageName::GetAssetPackageExtension());
        		bool bSaved = UPackage::SavePackage(package, nullptr, EObjectFlags::RF_Standalone, *packageFileName, GError, nullptr, true, true, SAVE_NoError);
        		if (bSaved)
        		{
        			UE_LOG(LogTemp, Log, TEXT("新创建的动画蓝图保存成功：%s"), *package->GetName());
        			FKismetEditorUtilities::CompileBlueprint(NewAsset);

        		}
        
        		BodySketal->ClearAnimScriptInstance();
        		BodySketal->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        		BodySketal->SetAnimClass(NewAsset->GeneratedClass);
        		BodySketal->SetAnimInstanceClass(NewAsset->GeneratedClass);
        	}
        
        	// TInlineComponentArray<UAnimControlComponent*> AnimComponents;
        	// ActorSpawn->GetComponents(AnimComponents);
        	// if (AnimComponents.IsValidIndex(0))
        	// {
        	// 	AnimControl = AnimComponents[0];
        	// 	AnimControl->InitializeInEditor(BodySketal, FaceSketal);
        	// }
		
		
		
		
		
		
		
		
		
		
		
		
	}
	

	
};


