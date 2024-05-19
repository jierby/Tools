// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControlRigComponent.h"
#include "CustomAnimInstance.h"
#include "FaceAnimInstance.h"
#include "Components/ActorComponent.h"
#include "AnimControlComponent.generated.h"

UENUM(BlueprintType)
enum class EImportantBone : uint8
{
	Head UMETA(DisplayName = "头部"),
	Pelvis UMETA(DisplayName = "腰部"),
	LeftHand UMETA(DisplayName = "左手"),
	RightHand UMETA(DisplayName = "右手"),
	LeftFoot UMETA(DisplayName = "左脚"),
	RightFoot UMETA(DisplayName =  "右脚"),
};

UENUM(BlueprintType)
enum class EBoneSecion : uint8
{
	Default UMETA(DisplayName = "默认"),
	Height UMETA(DisplayName = "身高"),
	EyeHigh UMETA(DisplayName = "眼高"),
	ShoulderHigh UMETA(DisplayName = "肩高"),
	PerinealHeight UMETA(DisplayName = "会阴高"),
	HighTibialPoint UMETA(DisplayName = "胫骨点高"),
	UpperArmLength UMETA(DisplayName =  "上臂长"),
	ForearmLength UMETA(DisplayName = "前臂长"),
	ThighLength UMETA(DisplayName = "大腿长"),
	CalfLength UMETA(DisplayName = "小腿长"),
	SittingHeight UMETA(DisplayName = "坐高"),
	SittingWithEyesHigh UMETA(DisplayName = "坐姿眼高"),
	SittingShoulderHeight UMETA(DisplayName = "坐姿肩高"),
	SittingElbowHeight UMETA(DisplayName = "坐姿肘高"),
	SittingKneeHigh UMETA(DisplayName = "坐姿膝高"),
	SittingHipWidth UMETA(DisplayName = "坐姿臀宽"),
	SittingKneeDistance UMETA(DisplayName = "坐姿膝距"),
	WidthBetweenCubits UMETA(DisplayName = "两肘间宽"),
	HeadWidth UMETA(DisplayName = "头宽"),
	HeadLength UMETA(DisplayName = "头长"),
	HeadHeight UMETA(DisplayName = "头高"),
	HandLength UMETA(DisplayName = "手长"),
	PalmLength UMETA(DisplayName = "掌长"),
	HandWidth UMETA(DisplayName = "手宽"),
	ThumbLength UMETA(DisplayName = "拇指长"),
	IndexFingerLength UMETA(DisplayName = "食指长"),
	FootLength UMETA(DisplayName = "足长"),
	FootWidth UMETA(DisplayName = "足宽"),
	ChestWidth UMETA(DisplayName = "胸宽"),
	ShoulderWidth UMETA(DisplayName = "肩宽"),
	ShoulderMaxWidth UMETA(DisplayName = "肩最大宽"),
	HipWidth UMETA(DisplayName = "臀宽"),
	ChestThick UMETA(DisplayName = "胸厚"),
	Weight UMETA(DisplayName = "体重"),
};

inline FName GetBoneSectionName(EBoneSecion Section)
{
	const UEnum* EnumPtr = StaticEnum<EBoneSecion>();
	if(!EnumPtr) return FName("");
	return FName(*EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Section)).ToString());
}

template<typename TEnum>
TEnum GetEnumValueFromDisplayName(const FString& EnumName, const FString& DisplayName)
{
	const UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (!Enum)
	{
		return TEnum(0); // 或其他适当的默认值
	}

	for (int32 i = 0; i < Enum->NumEnums() - 1; i++) // -1 排除_MAX最后一个条目
	{
		FString CurrentDisplayName = Enum->GetDisplayNameTextByIndex(i).ToString();
		if (CurrentDisplayName.Equals(DisplayName, ESearchCase::IgnoreCase)) // 忽略大小写比较
		{
			return static_cast<TEnum>(Enum->GetValueByIndex(i));
		}
	}
	
	return TEnum(0); // 如果没有找到匹配项，则返回默认值
}


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENDATAMANAGERCORE_API UAnimControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAnimControlComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base")
	void Initialize(USkeletalMeshComponent* body, USkeletalMeshComponent* face);
	
#if WITH_EDITOR
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / ControlRig")
	void InitializeInEditorWithControlRig(USkeletalMeshComponent* body, USkeletalMeshComponent* face,UControlRigComponent* ControlRig);
	UFUNCTION(BlueprintCallable, Category = "Anim Control Component / Base")
	void InitializeInEditor(USkeletalMeshComponent* body, USkeletalMeshComponent* face);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set")
	void SetActivateChildIndex(int32 Index);
	UFUNCTION(BlueprintCallable,BlueprintPure, Category="Anim Control Component / Get")
	bool IsValidAnimInstance();
#endif
	void ResetData(UControlRigComponent* ControlRig) const;
	UCustomAnimInstance* GetAnimInstanceDefault();
	
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base")
	void ApplyBoneInput(FString Section, double Data);
	
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base")
	void ConstructByWeightAndHeight(double Height, double Weight);
	
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base")
	void ApplyBoneInputbyHeightandWeight(double Height, double Weight);

	//4.11 fxl
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base")
	void ApplyBoneInputSittingHeight(double Height);
    //-----------------------------------
	
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base")
	void ExcuteWebHeightData(TMap<FString,float> InputData);

	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base")
	void ApplyBoneInputbyWebDatat(TMap<FString,float> InputData);

	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base")
	void ResetHumanData();

	void SetMainBoneTransform(EImportantBone InBone,FTransform InTransform);
	FTransform GetMainBoneTransform(EImportantBone InBone) const;
	FRotator GetMainBoneRotator(EImportantBone InBone) const;
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Control")
	void SetMainBoneRotator(EImportantBone InBone,FRotator InRotator);
	
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base",meta=(DisplayName="获取站姿的数据"))
	TMap<FName,float> GetStanceData();
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base",meta=(DisplayName="获取坐姿的数据"))
	TMap<FName,float> GetSittingPostureData();
	UFUNCTION(BlueprintCallable,Category="Anim Control Component / Base",meta=(DisplayName="获取公共的数据"))
	TMap<FName,float> GetPublicData();

	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="获取头部骨骼点的Z轴数值"))
	float		GetHeadBoneLocationZ(FName InSocketName) const;
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="获取身体骨骼点的Z轴数值"))
	float		GetBodyBoneLocationZ(FName InSocketName) const;
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="获取身体骨骼点的位置信息"))
	FVector		GetBodyBoneLocation(FName InSocketName)const;
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="依据类型获取身体骨骼点的位置信息"))
	FVector		GetBodyBoneLocationBySpace(FName InSocketName, ERelativeTransformSpace SpaceType) const;
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="获取指定骨骼点的Transform"))
	FTransform	GetBodyBoneTransform(FName InSocketName,ERelativeTransformSpace SpaceType = RTS_World) const;
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="获取两个骨骼点的空间距离"))
	float		GetLengthByTwoSpacePoint(FName InChildName,FName InParentName) const;
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="获取两个骨骼点的单位向量"))
	FVector		GetUnitVectorByTwoSpacePoint(FName InChildName, FName InParentName) const;
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="通过空间向量的位置获取距离添加移动的偏移量"))
	FVector		GetTwoSpacePointOffsetByDistance(FName InChildName, FName InParentName,float Distance) const;
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="设置腰椎的比例差值"))
	void		SetDifferenceOfTheLumbarVertebrae(float length,
					float distance,
					float& returnV1,
					float& returnV2,
					float& ReturnV3,
					float& ReturnV4,
					float& ReturnV5,
					float& res);
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="设置颈椎的比例差值"))
	void		SetDifferenceOfTheCervicalVertebrae(float length,
					float distance,
					float& returnV1,
					float& returnV2,
					float& ReturnV3);

	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="设置相对父节点的偏移值"))
	void		SetOffsetToParent(float A,FName InSocketNameR,FName InSocketNameL,FVector& ReturnValueR,FVector& ReturnValueL);
	//3.19fxl---------------------------------
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="设置相对父节点的偏移值加变量"))
	void		SetOffsetToParent_(float A,FName InSocketNameR,FName InSocketNameL,FVector& InVectiorR,
	FVector& InVectiorL,FVector& ReturnValueR,FVector& ReturnValueL);
	//----------------------------------------
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="获取相对父节点的空间相对长度"))
	float		GetLengthToParentSpace(FName InSocketName);
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="设置单节骨骼的值"))
	FVector		SetTheValueOfASingleBone(FName child,FName parent,float distance);
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="设置单节骨骼的值X"))
	FVector		SetTheValueOfASingleBoneX(FName child,FName parent,float distance);
	UFUNCTION(BlueprintCallable,Category="Anim Control Component",meta=(DisplayName="获取测试高度"))
	float		GetTestHeight();

	////
	/// 主要为头身高度差
	///
	float GetDifference(FName InSocket1, FName InSocket2) const;
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取身高"))
	float GetHeight();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取眼高"))
	float GetEyeHeight();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取坐高"))
	float GetSittingHeight();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取坐姿眼高"))
	float GetSittingEyeHeight();
	
	
	

	////
	/// 主要为身体部位的高度差
	///
	float GetDifferenceInBody(FName InSocket1, FName InSocket2) const;
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取肩高"))
	float GetShoulderHeight();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取会阴高"))
	float GetPerinealHeight();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取胫骨点高"))
	float GetTibiaPointHeight();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取大腿长"))
	float GetThighLength();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取坐姿肩高"))
	float GetSittingShoulderHeight();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取坐姿肘高"))
	float GetSeatedElbowHeight();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取坐姿膝高"))
	float GetSeatedKneeHeight();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取臀宽"))
	float GetHipWidth();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取坐姿臀宽"))
	float GetSeatedHipWidth();


	/// 
	/// 主要是空间两点的距离
	///
	float GetDifferenceInSpace(FName InSocket1, FName InSocket2) const;
	float GetDifferenceInSpaceXY(FName InSocket1, FName InSocket2) const;
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取上臂长"))
	float GetUpperArmLength();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取前臂长"))
	float GetForearmLength();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取小腿长"))
	float GetCalfLength();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取坐姿膝距"))
	float GetKneeDistanceInSittingPosition();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取两肘间宽"))
	float GetTheWidthBetweenTwoCubits();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取掌长"))
	float GetPalmLeader();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取手长"))
	float GetHandLength();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取手宽"))
	float GetHandWidth();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取食指长"))
	float GetIndexFingerLength();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取中指长"))
	float GetMiddleFingerLength();
	//3.20fxl-------------------
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取手指长"))
	float GetFingerLength(FName r_01,FName r_02,FName r_03,FName r_01_palm,FName r_03_socket);
	//-------------------
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取拇指长"))
	float GetThumbLength();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取足长"))
	float GetFootLength();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取足宽"))
	float GetFootWidth();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取胸宽"))
	float GetChestWidth();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取肩宽"))
	float GetShoulderWidth();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取肩最大宽"))
	float GetTheMaximumShoulderWidth();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取胸厚"))
	float GetBreastThick();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取体重"))
	float GetWeight();
	
	FVector GetHeadOffset();
     
	void SetHeadOffset(float x);
	
	/// 
	/// 主要为头部
	///
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取头长"))
	float GetHeadLength();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取头宽"))
	float GetHeadWidth();
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Get", meta=(DisplayName="获取头高"))
	float GetHeadHeight();

	/// 
	/// 主要是设置相关数值
	/// 
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置胫骨点高"))
	void SetTibiaPointHeight(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置小腿长"))
	void SetCalfLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置大腿长"))
	void SetThighLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置会阴高"))
	void SetPerinealHeight(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置肩高"))
	void SetShoulderHeight(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置眼高"))
	void SetEyeHeight(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置上臂长"))
	void SetUpperArmLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置前臂长"))
	void SetForearmLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置足长"))
	void SetFootLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置足宽"))
	void SetFootWidth(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置食指长"))
	void SetIndexFingerLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置中指长"))
	void SetMiddleFingerLength(float InputValue);
	//3.20fxl------------------------------------
	void SetPinkyFingerLength(float InputValue,float length);
	void SetRingFingerLength(float InputValue,float length);
	//------------------------------------
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置拇指长"))
	void SetThumbLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置掌长"))
	void SetPalmLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置手宽"))
	void SetHandWidth(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置手长"))
	void SetHandLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置坐姿肩高"))
	void SetShoulderHeightForSittingPosition(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置坐姿眼高"))
	void SetEyeHeightForSittingPosition(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置坐姿肘高"))
	void SetElbowHeightForSittingPosition(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置坐姿膝高"))
	void SetKneeHeightForSittingPosition(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置胸厚"))
	void SetChestThickness(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置臀宽"))
	void SetHipWidth(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置肩宽"))
	void SetShoulderWidth(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置肩最大宽"))
	void SetMaximumShoulderWidth(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置胸宽"))
	void SetChestWidth(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置坐姿臀宽"))
	void SetSeatedHipWidth(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置坐姿臀膝距"))
	void SetHipKneeDistanceForSittingPosition(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置坐姿两肘肩宽"))
	void SetSittingPositionTwoElbows(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置头高"))
	void SetHeadHeight(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置头宽"))
	void SetHeadWidth(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置头长"))
	void SetHeadLength(float InputValue);
	UFUNCTION(BlueprintCallable, Category="Anim Control Component / Set", meta=(DisplayName="设置体重"))
	void SetWeight(float InputValue);

	float GetCurrentDistance(double B, double A) const;

	UPROPERTY()
	USkeletalMeshComponent* Body;
	UPROPERTY()
	USkeletalMeshComponent* Face;
	UPROPERTY()
	UCustomAnimInstance* BodyAnimInstance;
	UPROPERTY()
	UFaceAnimInstance* FaceAnimInstance;

	UPROPERTY()
	double BaseHeight;
	UPROPERTY()
	double ShoulderHeight;
	UPROPERTY()
	double PerinealHeight;
	UPROPERTY()        
	double EyeHeight;
	UPROPERTY()
	double TibialPointHeight;
	// UPROPERTY()
	// TMap<EBoneSecion,float> InfoValueMap;
	
};
