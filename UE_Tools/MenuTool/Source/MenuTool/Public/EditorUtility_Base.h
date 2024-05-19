// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ObjectTools.h"

class UAnimBlueprint;
// #include "Editor/UnrealEd/Public/EditorDelegates.h"/**/

#include "EditorUtility_Base.generated.h"

#define LOCTEXT_NAMESPACE "UEditorUtility_Base"

/**
 * 
 */
UCLASS()
class MENUTOOL_API UEditorUtility_Base : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category="User Interface")
	void GetThumbnailInEditor(FString path);

	void RenderImage(int32 Width, int32 Height, const uint8* Data, const FString& ImageName, bool bSaveJpgOnly = false);
	
	void RenderThumbnail(UObject* InObject, const uint32 InImageWidth, const uint32 InImageHeight, ThumbnailTools::EThumbnailTextureFlushMode::Type InFlushMode, FTextureRenderTargetResource* InTextureRenderTargetResource, FObjectThumbnail* OutThumbnail);
	
	UFUNCTION(BlueprintCallable, Category = "ActorDeletedBegin")
	void RegisterOnDeleteActorsBegin()
	{
		FEditorDelegates::OnDeleteActorsBegin.AddUObject(this, &UEditorUtility_Base::OnDeleteActorsBeginHandler);
		FEditorDelegates::OnDeleteActorsEnd.AddUObject(this, &UEditorUtility_Base::OnDeleteActorsEndHandler);
	};
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,Category="ActorDeletedBegin")
	void OnDeleteActorsBeginHandler();
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,Category="ActorDeletedBegin")
	void OnDeleteActorsEndHandler();
	UFUNCTION(BlueprintCallable, Category="User Interface")
	void InitEditorWidget_CPP(); 
	UFUNCTION(BlueprintCallable, Category="User Interface")
	void UpdateAnimBlueprint_CPP();

	UFUNCTION(BlueprintCallable, Category="User Interface" )
	void SetSelectedActor_CPP(AActor*Selected);
	UFUNCTION(BlueprintCallable, Category="User Interface" )
	void CreateActor_CPP(AActor*Selected);
	UFUNCTION(BlueprintCallable, Category="User Interface")
	void OnValueChanged_CPP(FString Section, double Value);
	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void ChangePose_CPP( int Index);

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void SelectActorInEditor(AActor* ActorToSelect);

	// UFUNCTION(BlueprintCallable, Category = "User Interface")
	// void LoadAssetByDir();

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void SaveChange();

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void CreateAssetSelector(USkeletalMeshComponent* Component);
	
	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void SaveAnimBlueprintAsset(UAnimInstance* Anim);
	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void RefreshComponents();
	void ResetComponents();
	void CreateAnimationBlueprint(USkeletalMeshComponent* SkeletalComponent,UClass* Class);
	void CreateBodyAnimationBlueprint();
	void CreateFaceAnimationBlueprint();
	
	UPROPERTY(BlueprintReadWrite)
	UClass* BlueprintClassObj;
	// UPROPERTY()
	// UClass* BlueprintClassCharacter;
	UPROPERTY(BlueprintReadWrite)
	UClass* FaceAnimBlueprintClass;
	// UPROPERTY(BlueprintReadWrite)
	// UAnimBlueprint* AnimBlueprint;

	UClass* ChangeClass;
	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* BodySketal;
	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* FaceSketal;
	UPROPERTY(BlueprintReadWrite)
	UAnimControlComponent* AnimControl;
	UPROPERTY(BlueprintReadWrite)
	AActor* SelectedActor_CPP;
	FTimerHandle handle;

	UFUNCTION(BlueprintCallable)
	void Reba_ScoreA_Parts(
		int& NeckScore, int& LegsScore, int& TrunkScore, int& LoadScore,
		int neckPitch, 
		bool neckTwisted, 
		bool neckSideBend, 
		int trunkPitch, 
		bool trunkTwisted,
		bool trunkSideBend, 
		bool bothLegsDown, 
		int kneeBend, 
		float load, 
		bool bShockForce);
	UFUNCTION(BlueprintCallable)
	void Reba_ScoreB_Parts(int& idxUpperArmScore, int& idxLowerArmScore, int& idxWristScore, int upperArmPosition, bool bShouldRaised, bool bUpperArmAbducted, bool bArmSupported,
		int lowerArmPosition, int wristPosition, bool bWristBendTwisted,
		int couplingScore);
	UFUNCTION(BlueprintCallable)
		void Rula_WristArmScore(int& idxUpperArmScore,
			int& idxLowerArmScore,
			int& idxWristScore,
			int& idxWristTwistScore,
			int& loadScoreScore, int upperArmPosition, bool bShoulderRaised, bool bUpperArmAbducted, bool bArmSupported,
			int lowerArmPosition, bool bArmOffset,
			int wristPosition, bool bWristBent, bool bWristTwisted, bool bWristEndofRange,
			int muscleScore, float load, bool bLoadIsStaticOrRepeated);
	UFUNCTION(BlueprintCallable)
		void Rula_NeckTrunkLegScore(int& muscleScoreScore,
			int& idxNeckPostureScore,
			int& idxTrunkPostureScore,
			int& idxLegsScore, int neckPosition, bool bNeckTwisted, bool bNeckSideBent,
		int trunkPosition, bool bTrunkTwisted, bool bTrunkSideBent,
		bool bLegsAreSupported, bool bPostureStaticOrRepeated, float load, bool bLoadIsStaticOrRepeated);




















	// TableA UpperArm - LowerArm - WristScore - WristTwist
	 const TArray<TArray<TArray<TArray<int32>>>> TableA = {
		// UpperArm 1
		{
			{
				{1, 2},
				{ 2,2 },
				{ 2,3 },
				{ 3,3 }
			},
				{
					{2,2},
					{2,2},
					{3,3},
					{3,3}
				},
				{
					{2,3},
					{3,3},
					{3,3},
					{4,4}
				}
		},
		// UpperArm 2
		{
			{
				{2,3},
				{3,3},
				{3,4},
				{4,4}
			},
			{
				{3,3},
				{3,3},
				{3,4},
				{4,4}
			},
			{
				{3,4},
				{4,4},
				{4,4},
				{5,5}
			}
		},
		// UpperArm 3
	{
		{
			{3,3},
			{4,4},
			{4,4},
			{5,5}
		},
		{
			{3,4},
			{4,4},
			{4,4},
			{5,5}
		},
		{
			{4,4},
			{4,4},
			{4,5},
			{5,5}
		}
	},
		// UpperArm 4
		{
			{
				{4,4},
				{4,4},
				{4,5},
				{5,5}
			},
			{
				{4,4},
				{4,4},
				{4,5},
				{5,5}
			},
			{
				{4,4},
				{4,5},
				{5,5},
				{6,6}
			}
		},
		// UpperArm 5
		{
			{
				{5,5},
				{5,5},
				{5,6},
				{6,7}
			},
			{
				{5,6},
				{6,6},
				{6,7},
				{7,7}
			},
			{
				{6,6},
				{6,7},
				{7,7},
				{7,8}
			}
		},
		// UpperArm 6
		{
			{
				{7,7},
				{7,7},
				{7,8},
				{8,9}
			},
			{
				{8,8},
				{8,8},
				{8,9},
				{9,9}
			},
			{
				{9,9},
				{9,9},
				{9,9},
				{9,9}
			}
		}
	};




	// TableB NeckPosture - TrunkPosture - Legs


	TArray<TArray<TArray<int32>>> TableB =
	{
		// Neck Posture 1
		{
			{1,3},
			{2,3},
			{3,4},
			{5,5},
			{6,6},
			{7,7}
		},
		// Neck Posture 2
		{
			{2,3},
			{2,3},
			{4,5},
			{5,5},
			{6,7},
			{7,7}
		},
		// Neck Posture 3
		{
			{3,3},
			{3,4},
			{4,5},
			{5,6},
			{6,7},
			{7,7}
		},
		// Neck Posture 4
		{
			{5,5},
			{5,6},
			{6,7},
			{7,7},
			{7,7},
			{8,8}
		},
		// Neck Posture 5
		{
			{7,7},
			{7,7},
			{7,8},
			{8,8},
			{8,8},
			{8,8}
		},
		// Neck Posture 6
		{
			{8,8},
			{8,8},
			{8,8},
			{8,9},
			{9,9},
			{9,9}
		}
	};


	// TableC Wrist/Arm - Neck,Trunk,Leg
	TArray<TArray<int32>> TableC = {
		{1,2,3,3,4,5,5},
		{2,2,3,4,4,5,5},
		{3,3,3,4,4,5,6},
		{3,3,3,4,5,6,6},
		{4,4,4,5,6,7,7},
		{4,4,5,6,6,7,7},
		{5,5,6,6,7,7,7},
		{5,5,6,7,7,7,7}
	};

	
	
#endif
};



