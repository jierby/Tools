// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimControlComponent.h"

#include "ControlRigComponent.h"
#include "ControlRigComponent.h"
#include "Editor.h"
#include "AnimGraphRuntime/Public/KismetAnimationLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UAnimControlComponent::UAnimControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAnimControlComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UAnimControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAnimControlComponent::Initialize(USkeletalMeshComponent* body, USkeletalMeshComponent* face)
{
	if (body)
	{
		Body = body;
		BodyAnimInstance = Cast<UCustomAnimInstance>(Body->GetAnimInstance());
		// // #if WITH_EDITOR
		// 		if(GIsEditor)
		// 		{
		// 			BodyAnimInstance = Body->GetAnimClass()->GetDefaultObject<UCustomAnimInstance>();
		// 		}
		// #endif
		if (BodyAnimInstance == nullptr)
		{
			return;
		}
	}
	if (face)
	{
		Face = face;
		FaceAnimInstance = Cast<UFaceAnimInstance>(Face->GetAnimInstance());
		// #if WITH_EDITOR
				// if(GIsEditor)
				// {
				// 	FaceAnimInstance = Face->GetAnimClass()->GetDefaultObject<UFaceAnimInstance>();
				// }
		// #endif
		if (FaceAnimInstance == nullptr)
		{
			return;
		}
	}

	BaseHeight = GetHeight();
	ShoulderHeight = GetShoulderHeight();
	PerinealHeight = GetPerinealHeight();
	TibialPointHeight = GetTibiaPointHeight();
	EyeHeight = GetEyeHeight();
}

#if WITH_EDITOR
void UAnimControlComponent::InitializeInEditorWithControlRig(USkeletalMeshComponent* body, USkeletalMeshComponent* face, UControlRigComponent* ControlRig)
{
	if (body)
	{
		Body = body;
		BodyAnimInstance = Cast<UCustomAnimInstance>(Body->GetAnimClass()->GetDefaultObject());
		UE_LOG(LogTemp, Error, TEXT("%s"), *BodyAnimInstance->GetName());
		if (BodyAnimInstance == nullptr)
		{
			return;
		}
		ResetData(ControlRig);
	}
	if (face)
	{
		Face = face;
		FaceAnimInstance = Cast<UFaceAnimInstance>(Face->GetAnimClass()->GetDefaultObject());
		if (FaceAnimInstance == nullptr)
		{
			return;
		}
	}

	BaseHeight = GetHeight();
	ShoulderHeight = GetShoulderHeight();
	PerinealHeight = GetPerinealHeight();
	TibialPointHeight = GetTibiaPointHeight();
	EyeHeight = GetEyeHeight();
}

void UAnimControlComponent::InitializeInEditor(USkeletalMeshComponent* body, USkeletalMeshComponent* face)
{
	if (body && body->GetAnimClass())
	{	
		Body = body;
		BodyAnimInstance = Body->GetAnimClass()->GetDefaultObject<UCustomAnimInstance>();
		UE_LOG(LogTemp, Error, TEXT("%s"), *BodyAnimInstance->GetName());
		if (BodyAnimInstance == nullptr)
		{
			return;
		}
	}
	else
	{
		return;
	}
	if (face)
	{
		Face = face;
		FaceAnimInstance = Face->GetAnimClass()->GetDefaultObject<UFaceAnimInstance>();
		if (FaceAnimInstance == nullptr)
		{
			return;
		}
	}

	BaseHeight = GetHeight();
	ShoulderHeight = GetShoulderHeight();
	PerinealHeight = GetPerinealHeight();
	TibialPointHeight = GetTibiaPointHeight();
	EyeHeight = GetEyeHeight();
}

void UAnimControlComponent::SetActivateChildIndex(int32 Index)
{
	if (BodyAnimInstance)
	{
		BodyAnimInstance->ActiveChildIndex = Index;
	}

	// if(BodyAnimInstance)
	// {
	// 	BodyAnimInstance->SetActiveIndex(Index);
	// }
}

bool UAnimControlComponent::IsValidAnimInstance()
{
	return Body->GetAnimInstance() != nullptr ? true : false;
}
#endif

void UAnimControlComponent::ApplyBoneInput(FString Str, double Data)
{
	const EBoneSecion Section = GetEnumValueFromDisplayName<EBoneSecion>("EBoneSecion", Str);

	if (BodyAnimInstance)
	{
		switch (Section) {
		case EBoneSecion::Height:
			break;
		case EBoneSecion::EyeHigh:
			SetEyeHeight(Data);
			break;
		case EBoneSecion::ShoulderHigh:
			SetShoulderHeight(Data);
			break;
		case EBoneSecion::PerinealHeight:
			SetPerinealHeight(Data);
			break;
		case EBoneSecion::HighTibialPoint:
			SetTibiaPointHeight(Data);
			break;
		case EBoneSecion::UpperArmLength:
			SetUpperArmLength(Data);
			break;
		case EBoneSecion::ForearmLength:
			SetForearmLength(Data);
			break;
		case EBoneSecion::ThighLength:
			SetThighLength(Data);
			break;
		case EBoneSecion::CalfLength:
			SetCalfLength(Data);
			break;
		case EBoneSecion::SittingHeight:
			ApplyBoneInputSittingHeight(Data);
			break;
		case EBoneSecion::SittingWithEyesHigh:
			SetEyeHeightForSittingPosition(Data);
			break;
		case EBoneSecion::SittingShoulderHeight:
			SetShoulderHeightForSittingPosition(Data);
			break;
		case EBoneSecion::SittingElbowHeight:
			SetElbowHeightForSittingPosition(Data);
			break;
		case EBoneSecion::SittingKneeHigh:
			SetKneeHeightForSittingPosition(Data);
			break;
		case EBoneSecion::SittingHipWidth:
			SetSeatedHipWidth(Data);
			break;
		case EBoneSecion::SittingKneeDistance:
			SetHipKneeDistanceForSittingPosition(Data);
			break;
		case EBoneSecion::WidthBetweenCubits:
			SetSittingPositionTwoElbows(Data);
			break;
		case EBoneSecion::HeadWidth:
			SetHeadWidth(Data);
			break;
		case EBoneSecion::HeadLength:
			SetHeadLength(Data);
			break;
		case EBoneSecion::HeadHeight:
			SetHeadHeight(Data);
			break;
		case EBoneSecion::HandLength:
			SetHandLength(Data);
			break;
		case EBoneSecion::PalmLength:
			SetPalmLength(Data);
			break;
		case EBoneSecion::HandWidth:
			SetHandWidth(Data);
			break;
		case EBoneSecion::ThumbLength:
			SetThumbLength(Data);
			break;
		case EBoneSecion::IndexFingerLength:
			SetIndexFingerLength(Data);
			break;
		case EBoneSecion::FootLength:
			SetFootLength(Data);
			break;
		case EBoneSecion::FootWidth:
			SetFootWidth(Data);
			break;
		case EBoneSecion::ChestWidth:
			SetChestWidth(Data);
			break;
		case EBoneSecion::ShoulderWidth:
			SetShoulderWidth(Data);
			break;
		case EBoneSecion::ShoulderMaxWidth:
			SetMaximumShoulderWidth(Data);
			break;
		case EBoneSecion::HipWidth:
			SetHipWidth(Data);
			break;
		case EBoneSecion::ChestThick:
			SetChestThickness(Data);
			break;
		case EBoneSecion::Weight:
			SetWeight(Data);
			break;
		default:;
		}
	}
}

void UAnimControlComponent::ConstructByWeightAndHeight(double Height, double Weight)
{
	SetTibiaPointHeight(GetCurrentDistance(Height, TibialPointHeight));
	SetPerinealHeight(GetCurrentDistance(Height, PerinealHeight));
	SetShoulderHeight(GetCurrentDistance(Height, ShoulderHeight));
	SetEyeHeight(Height - GetHeight() + GetEyeHeight());
	const float ClampWeight = UKismetMathLibrary::MapRangeClamped(Weight, 60, 100, 1, 1.6);
	SetWeight(ClampWeight);
}

void UAnimControlComponent::ApplyBoneInputbyHeightandWeight(double Height, double Weight)
{
	ConstructByWeightAndHeight(Height, Weight);
}

void UAnimControlComponent::ApplyBoneInputSittingHeight(double Height)
{
	//
	float Difference = Height - GetSittingHeight();
	const float Length = GetBodyBoneLocationZ("spine_05") - GetBodyBoneLocationZ("pelvis");

	float RV1 = 0, RV2 = 0, RV3 = 0, RV4 = 0, RV5 = 0, Res = 0;
	const float Distance = Difference;
	SetDifferenceOfTheLumbarVertebrae(Length, Distance, RV1, RV2, RV3, RV4, RV5, Res);
	const FVector Nor = FVector(0, 0, 1);

	BodyAnimInstance->spine_1 += RV1 * Nor;
	BodyAnimInstance->spine_2 += RV2 * Nor;
	BodyAnimInstance->spine_3 += RV3 * Nor;
	BodyAnimInstance->spine_4 += RV4 * Nor;
	BodyAnimInstance->spine_5 += RV5 * Nor;
}

void UAnimControlComponent::ExcuteWebHeightData(TMap<FString, float> InputData)
{
	if (InputData.Contains((TEXT("头长"))))
	{
		const float* Tmp = InputData.Find((TEXT("头长")));
		SetHeadLength(*Tmp);
	}

	if (InputData.Contains((TEXT("头高"))))
	{
		const float* Tmp = InputData.Find(TEXT("头高"));
		SetHeadHeight(*Tmp);
	}

	if (InputData.Contains(TEXT("头宽")))
	{
		const float* Tmp = InputData.Find(TEXT("头宽"));
		SetHeadWidth(*Tmp);
	}

	if (InputData.Contains(TEXT("小腿长")))
	{
		const float* Tmp = InputData.Find(TEXT("小腿长"));
		SetCalfLength(*Tmp);
	}
	else if (InputData.Contains(TEXT("胫骨点高")))
	{
		const float* Tmp1 = InputData.Find(TEXT("胫骨点高"));
		SetTibiaPointHeight(*Tmp1);
	}

	if (InputData.Contains(TEXT("大腿长")))
	{
		const float* Tmp = InputData.Find(TEXT("大腿长"));
		SetThighLength(*Tmp);
	}
	else
	{
		const float* Tmp1 = InputData.Find(TEXT("会阴高"));
		SetPerinealHeight(*Tmp1);
	}

	if (InputData.Contains(TEXT("肩高")))
	{
		const float* Tmp = InputData.Find(TEXT("肩高"));
		SetShoulderHeight(*Tmp);
	}

	if (InputData.Contains(TEXT("眼高")))
	{
		const float* Tmp = InputData.Find(TEXT("眼高"));
		SetEyeHeight(*Tmp);
	}
	else
	{
		if (const float* Tmp1 = InputData.Find(TEXT("身高")))
		{
			SetEyeHeight(*Tmp1 - GetHeight() + GetEyeHeight());
		}
		else
		{
			return;
		}
	}

	if (!InputData.Contains(TEXT("肩高")))
	{
		float Tmp = ShoulderHeight / BaseHeight * GetHeight() - GetShoulderHeight();
		SetShoulderHeight(ShoulderHeight / BaseHeight * GetHeight());
		SetEyeHeight(Tmp * -1 + GetEyeHeight());
	}

	if (!(InputData.Contains(TEXT("大腿长")) || InputData.Contains(TEXT("会阴高"))))
	{
		float Tmp = PerinealHeight / BaseHeight * GetHeight() - GetPerinealHeight();
		SetPerinealHeight(PerinealHeight / BaseHeight * GetHeight());
		SetShoulderHeight(GetShoulderHeight() + Tmp * -1);
	}

	if (!(InputData.Contains(TEXT("小腿长")) || InputData.Contains(TEXT("胫骨点高"))))
	{
		float Tmp = TibialPointHeight / BaseHeight * GetHeight() - GetTibiaPointHeight();
		SetTibiaPointHeight(TibialPointHeight / BaseHeight * GetHeight());
		SetPerinealHeight(Tmp * -1 + GetPerinealHeight());
	}
}

void UAnimControlComponent::ApplyBoneInputbyWebDatat(TMap<FString, float> InputData)
{
	/*for (auto data :InputData)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s--%f"),*data.Key,data.Value);
	}*/
	if (InputData.Contains(TEXT("掌长")))
	{
		SetPalmLength(InputData[TEXT("掌长")]);
	}
	if (InputData.Contains(TEXT("手长")))
	{
		SetHandLength(InputData[TEXT("手长")]);
	}
	if (InputData.Contains(TEXT("食指长")))
	{
		SetMiddleFingerLength(InputData[TEXT("食指长")]);
	}
	if (InputData.Contains(TEXT("拇指长")))
	{
		SetThumbLength(InputData[TEXT("拇指长")]);
	}
	if (InputData.Contains(TEXT("足长")))
	{
		SetFootLength(InputData[TEXT("足长")]);
	}
	if (InputData.Contains(TEXT("足宽")))
	{
		SetFootWidth(InputData[TEXT("足宽")]);
	}
	if (InputData.Contains(TEXT("上臂长")))
	{
		SetUpperArmLength(InputData[TEXT("上臂长")]);
	}
	if (InputData.Contains(TEXT("前臂长")))
	{
		SetUpperArmLength(InputData[TEXT("前臂长")]);
	}
	if (InputData.Contains(TEXT("胸厚")))
	{
		SetChestThickness(InputData[TEXT("胸厚")]);
	}
	if (InputData.Contains(TEXT("臀宽")))
	{
		SetHipWidth(InputData[TEXT("臀宽")]);
	}
	if (InputData.Contains(TEXT("肩宽")))
	{
		SetShoulderWidth(InputData[TEXT("肩宽")]);
	}
	if (InputData.Contains(TEXT("肩最大宽")))
	{
		SetMaximumShoulderWidth(InputData[TEXT("肩最大宽")]);
	}
	if (InputData.Contains(TEXT("胸宽")))
	{
		SetChestWidth(InputData[TEXT("胸宽")]);
	}

	ExcuteWebHeightData(InputData);
}

void UAnimControlComponent::ResetHumanData()
{
	if (BodyAnimInstance)
	{
		BodyAnimInstance->ResetHumanData();
	}
	if (FaceAnimInstance)
	{
		FaceAnimInstance->ResetHumanData();
	}
}

void UAnimControlComponent::SetMainBoneTransform(EImportantBone InBone, FTransform InTransform)
{
	if (BodyAnimInstance)
	{
		switch (InBone)
		{
		case EImportantBone::Head:
			BodyAnimInstance->HeadCtrl = InTransform;
			break;
		case EImportantBone::Pelvis:
			BodyAnimInstance->Pelvis_C = InTransform;
			break;
		case EImportantBone::LeftHand:
			BodyAnimInstance->Hand_L_C = InTransform;
			break;
		case EImportantBone::RightHand:
			BodyAnimInstance->Hand_R_C = InTransform;
			break;
		case EImportantBone::LeftFoot:
			BodyAnimInstance->Ball_L = InTransform;
			break;
		case EImportantBone::RightFoot:
			BodyAnimInstance->Ball_R = InTransform;
			break;
		}
	}
}

FTransform UAnimControlComponent::GetMainBoneTransform(EImportantBone InBone) const
{
	FTransform Trans;
	switch (InBone)
	{
	case EImportantBone::Head:
		Trans = BodyAnimInstance->HeadCtrl;
		break;
	case EImportantBone::Pelvis:
		Trans = BodyAnimInstance->Pelvis_C;
		break;
	case EImportantBone::LeftHand:
		Trans = BodyAnimInstance->Hand_L_C;
		break;
	case EImportantBone::RightHand:
		Trans = BodyAnimInstance->Hand_R_C;
		break;
	case EImportantBone::LeftFoot:
		Trans = BodyAnimInstance->Ball_L;
		break;
	case EImportantBone::RightFoot:
		Trans = BodyAnimInstance->Ball_R;
		break;
	}
	return Trans;
}

FRotator UAnimControlComponent::GetMainBoneRotator(EImportantBone InBone) const
{
	FRotator Rotator;
	switch (InBone) {
	case EImportantBone::Head:
		Rotator = BodyAnimInstance->HeadRotator;
		break;
	case EImportantBone::Pelvis:
		Rotator = BodyAnimInstance->PelvisRotator;
		break;
	case EImportantBone::LeftHand:
		Rotator = BodyAnimInstance->LHandRotator;
		break;
	case EImportantBone::RightHand:
		Rotator = BodyAnimInstance->RHandRotator;
		break;
	case EImportantBone::LeftFoot:
		Rotator = BodyAnimInstance->LFootRotator;
		break;
	case EImportantBone::RightFoot:
		Rotator = BodyAnimInstance->RFootRotator;
		break;
	}
	return Rotator;
}

void UAnimControlComponent::SetMainBoneRotator(EImportantBone InBone, FRotator InRotator)
{
	switch (InBone) {
	case EImportantBone::Head:
		BodyAnimInstance->HeadRotator = InRotator;
		break;
	case EImportantBone::Pelvis:
		BodyAnimInstance->PelvisRotator = InRotator;
		break;
	case EImportantBone::LeftHand:
		BodyAnimInstance->LHandRotator = InRotator;
		break;
	case EImportantBone::RightHand:
		BodyAnimInstance->RHandRotator = InRotator;
		break;
	case EImportantBone::LeftFoot:
		BodyAnimInstance->LFootRotator = InRotator;
		break;
	case EImportantBone::RightFoot:
		BodyAnimInstance->RFootRotator = InRotator;
		break;
	}
}

void UAnimControlComponent::ResetData(UControlRigComponent* ControlRig) const
{
	if (ControlRig)
	{
		BodyAnimInstance->HeadCtrl = ControlRig->GetControlTransform("head_ctrl");
		BodyAnimInstance->HeadRotator = FRotator::ZeroRotator;
		BodyAnimInstance->Hand_L_C = ControlRig->GetControlTransform("hand_l_ctrl");
		BodyAnimInstance->LHandRotator = FRotator::ZeroRotator;
		BodyAnimInstance->Hand_R_C = ControlRig->GetControlTransform("hand_r_ctrl");
		BodyAnimInstance->RHandRotator = FRotator::ZeroRotator;
		BodyAnimInstance->Ball_L = ControlRig->GetControlTransform("ball_l_ctrl");
		BodyAnimInstance->LFootRotator = FRotator::ZeroRotator;
		BodyAnimInstance->Ball_R = ControlRig->GetControlTransform("ball_r_ctrl");
		BodyAnimInstance->RFootRotator = FRotator::ZeroRotator;
		BodyAnimInstance->Pelvis_C = ControlRig->GetControlTransform("pelvis_ctrl");
		BodyAnimInstance->PelvisRotator = FRotator::ZeroRotator;
	}
}

UCustomAnimInstance* UAnimControlComponent::GetAnimInstanceDefault()
{
	return Cast<UCustomAnimInstance>(Body->GetAnimClass()->GetDefaultObject());
}

TMap<FName, float> UAnimControlComponent::GetStanceData()
{
	TMap<FName, float> InfoValueMap;
	InfoValueMap.Empty();

	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::Height), GetHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::ShoulderHigh), GetShoulderHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::EyeHigh), GetEyeHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::PerinealHeight), GetPerinealHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::HighTibialPoint), GetTibiaPointHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::UpperArmLength), GetUpperArmLength());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::ForearmLength), GetForearmLength());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::ThighLength), GetThighLength());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::CalfLength), GetCalfLength());

	return InfoValueMap;
}

TMap<FName, float> UAnimControlComponent::GetSittingPostureData()
{
	TMap<FName, float> InfoValueMap;
	InfoValueMap.Empty();

	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::SittingHeight), GetSittingHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::SittingWithEyesHigh), GetSittingEyeHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::SittingShoulderHeight), GetSittingShoulderHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::SittingElbowHeight), GetSeatedElbowHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::SittingKneeHigh), GetSeatedKneeHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::SittingHipWidth), GetSeatedHipWidth());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::SittingKneeDistance), GetKneeDistanceInSittingPosition());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::WidthBetweenCubits), GetTheWidthBetweenTwoCubits());

	return InfoValueMap;
}

TMap<FName, float> UAnimControlComponent::GetPublicData()
{
	TMap<FName, float> InfoValueMap;
	InfoValueMap.Empty();

	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::HandLength), GetHandLength());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::PalmLength), GetPalmLeader());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::HandWidth), GetHandWidth());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::IndexFingerLength), GetIndexFingerLength());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::ThumbLength), GetThumbLength());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::FootLength), GetFootLength());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::FootWidth), GetFootWidth());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::ChestWidth), GetChestWidth());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::ShoulderWidth), GetShoulderWidth());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::ShoulderMaxWidth), GetTheMaximumShoulderWidth());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::HipWidth), GetHipWidth());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::ChestThick), GetBreastThick());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::HeadLength), GetHeadLength());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::HeadWidth), GetHeadWidth());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::HeadHeight), GetHeadHeight());
	InfoValueMap.Add(GetBoneSectionName(EBoneSecion::FootWidth), GetFootWidth());

	return InfoValueMap;
}

float UAnimControlComponent::GetHeadBoneLocationZ(FName InSocketName) const
{
	if (Face)
	{
		const FTransform Trans = Face->GetSocketTransform(Face->GetSocketBoneName(InSocketName), RTS_World);
		return Trans.GetLocation().Z;
	}
	return -1;
}

float UAnimControlComponent::GetBodyBoneLocationZ(FName InSocketName) const
{
	if (Body)
	{
		FName name = Body->GetSocketBoneName(InSocketName);
		FTransform trans = Body->GetSocketTransform(name);
		return trans.GetLocation().Z;
	}
	return -1;
}

FVector UAnimControlComponent::GetBodyBoneLocation(FName InSocketName) const
{
	if (Body && InSocketName != NAME_None)
	{
		const FTransform Trans = GetBodyBoneTransform(InSocketName);
		return Trans.GetLocation();
	}
	return FVector::Zero();
}

FVector UAnimControlComponent::GetBodyBoneLocationBySpace(FName InSocketName, ERelativeTransformSpace SpaceType) const
{
	if (Body)
	{
		const FTransform Trans = Body->GetSocketTransform(Body->GetSocketBoneName(InSocketName), SpaceType);
		return Trans.GetLocation();
	}
	return FVector::Zero();
}

FTransform UAnimControlComponent::GetBodyBoneTransform(FName InSocketName, ERelativeTransformSpace SpaceType) const
{
	if (Body)
	{
		const FTransform Trans = Body->GetSocketTransform(Body->GetSocketBoneName(InSocketName), SpaceType);
		return Trans;
	}
	return FTransform(FVector::Zero());
}

float UAnimControlComponent::GetLengthByTwoSpacePoint(FName InChildName, FName InParentName) const
{
	const FVector Vec = GetBodyBoneLocation(InChildName) - GetBodyBoneLocation(InParentName);
	return Vec.Length();
}

FVector UAnimControlComponent::GetUnitVectorByTwoSpacePoint(FName InChildName, FName InParentName) const
{
	const FVector Vec = GetBodyBoneLocation(InChildName) - GetBodyBoneLocation(InParentName);
	return Vec.GetSafeNormal();
}

auto UAnimControlComponent::GetTwoSpacePointOffsetByDistance(FName InChildName, FName InParentName,
	float Distance) const -> FVector
{
	const FVector Vec = GetUnitVectorByTwoSpacePoint(InChildName, InParentName);
	const float Length = GetLengthByTwoSpacePoint(InChildName, InParentName);
	return Vec * (Length - Distance);
}

void UAnimControlComponent::SetDifferenceOfTheLumbarVertebrae(float length, float distance, float& ReturnV1,
	float& ReturnV2, float& ReturnV3, float& ReturnV4, float& ReturnV5, float& Res)
{
	float L1_L2 = GetBodyBoneLocationZ(TEXT("spine_01")) - GetBodyBoneLocationZ(TEXT("pelvis"));
	float L2_L3 = GetBodyBoneLocationZ(TEXT("spine_02")) - GetBodyBoneLocationZ(TEXT("spine_01"));
	float L3_L4 = GetBodyBoneLocationZ(TEXT("spine_03")) - GetBodyBoneLocationZ(TEXT("spine_02"));
	float L4_L5 = GetBodyBoneLocationZ(TEXT("spine_04")) - GetBodyBoneLocationZ(TEXT("spine_03"));
	float L5_L6 = GetBodyBoneLocationZ(TEXT("spine_05")) - GetBodyBoneLocationZ(TEXT("spine_04"));

	ReturnV1 = L1_L2 / length * distance;
	ReturnV2 = L2_L3 / length * distance;
	ReturnV3 = L3_L4 / length * distance;
	ReturnV4 = L4_L5 / length * distance;
	ReturnV5 = L5_L6 / length * distance;
	Res = L1_L2 + L2_L3 + L3_L4 + L4_L5 + L5_L6;
}

void UAnimControlComponent::SetDifferenceOfTheCervicalVertebrae(float length, float distance, float& returnV1,
	float& returnV2, float& ReturnV3)
{
	float L1_L2 = GetBodyBoneLocationZ(TEXT("neck_01")) - GetBodyBoneLocationZ(TEXT("spine_05"));
	float L2_L3 = GetBodyBoneLocationZ(TEXT("neck_02")) - GetBodyBoneLocationZ(TEXT("neck_01"));
	float L3_L4 = GetBodyBoneLocationZ(TEXT("head")) - GetBodyBoneLocationZ(TEXT("neck_02"));

	returnV1 = L1_L2 / length * distance;
	returnV2 = L2_L3 / length * distance;
	ReturnV3 = L3_L4 / length * distance;
}

void UAnimControlComponent::SetOffsetToParent(float A, FName InSocketNameR, FName InSocketNameL, FVector& ReturnValueR,
	FVector& ReturnValueL)
{
	if (BodyAnimInstance)
	{
		FVector VecR = GetBodyBoneLocationBySpace(InSocketNameR, RTS_ParentBoneSpace);
		ReturnValueR = (A - VecR.Length()) * (VecR.GetSafeNormal()) + BodyAnimInstance->lowerarm_r;
		FVector VecL = GetBodyBoneLocationBySpace(InSocketNameL, RTS_ParentBoneSpace);
		ReturnValueL = (A - VecL.Length()) * (VecL.GetSafeNormal()) + BodyAnimInstance->lowerarm_l;
	}
}

void UAnimControlComponent::SetOffsetToParent_(float A, FName InSocketNameR, FName InSocketNameL, FVector& InVectiorR,
	FVector& InVectiorL, FVector& ReturnValueR, FVector& ReturnValueL)
{
	if (BodyAnimInstance)
	{
		//float distance = A-InVectiorR.Length();
		FVector VecR = GetBodyBoneLocationBySpace(InSocketNameR, RTS_ParentBoneSpace);
		//ReturnValueR = (A-InVectiorR.Length())*(VecR.GetSafeNormal()) + InVectiorR;
		ReturnValueR = A * (VecR.GetSafeNormal()) + InVectiorR;
		FVector VecL = GetBodyBoneLocationBySpace(InSocketNameL, RTS_ParentBoneSpace);
		//ReturnValueL = (A-InVectiorL.Length()) * (VecL.GetSafeNormal()) + InVectiorL;
		ReturnValueL = A * (VecL.GetSafeNormal()) + InVectiorL;
	}
}

float UAnimControlComponent::GetLengthToParentSpace(FName InSocketName)
{
	if (Body)
	{
		return GetBodyBoneTransform(InSocketName, RTS_ParentBoneSpace).GetLocation().Length();
	}
	return -1;
}

FVector UAnimControlComponent::SetTheValueOfASingleBone(FName child, FName parent, float distance)
{
	return GetTwoSpacePointOffsetByDistance(child, parent, distance);
}

FVector UAnimControlComponent::SetTheValueOfASingleBoneX(FName child, FName parent, float distance)
{
	const FVector Vec = GetTwoSpacePointOffsetByDistance(child, parent, distance);
	return FVector(Vec.X, 0, 0);
}

float UAnimControlComponent::GetTestHeight()
{
	return GetBodyBoneLocationZ("spine_05");
}

float UAnimControlComponent::GetDifference(FName InSocket1, FName InSocket2) const
{
	return FMath::Abs(GetHeadBoneLocationZ(InSocket1) - GetBodyBoneLocationZ(InSocket2));
}

float UAnimControlComponent::GetHeight()
{
	return GetDifference("FACIAL_C_Hair5", "root");
}

float UAnimControlComponent::GetEyeHeight()
{
	return GetDifference("FACIAL_L_Eye", "root");
}

float UAnimControlComponent::GetSittingHeight()
{
	return GetDifference("FACIAL_C_Hair5", "thigh_bck_lwr_l");
}

float UAnimControlComponent::GetSittingEyeHeight()
{
	return GetDifference("FACIAL_L_Eye", "thigh_bck_lwr_l");
}

float UAnimControlComponent::GetDifferenceInBody(FName InSocket1, FName InSocket2) const
{
	return FMath::Abs(GetBodyBoneLocationZ(InSocket1) - GetBodyBoneLocationZ(InSocket2));
}

float UAnimControlComponent::GetShoulderHeight()
{
	return GetDifferenceInBody("clavicle_out_r", "root");
}

float UAnimControlComponent::GetPerinealHeight()
{
	return GetDifferenceInBody("thigh_in_r", "root");
}

float UAnimControlComponent::GetTibiaPointHeight()
{
	return GetDifferenceInBody("calf_r", "root");
}

float UAnimControlComponent::GetThighLength()
{
	return GetDifferenceInBody("thigh_r", "calf_r");
}

float UAnimControlComponent::GetSittingShoulderHeight()
{
	return GetDifferenceInBody("clavicle_out_r", "thigh_bck_lwr_l");
}

float UAnimControlComponent::GetSeatedElbowHeight()
{
	return GetDifferenceInBody("lowerarm_bck_l", "thigh_bck_lwr_l");
}

float UAnimControlComponent::GetSeatedKneeHeight()
{
	return GetDifferenceInBody("calf_knee_l", "root");
}

float UAnimControlComponent::GetHipWidth()
{
	const FVector thigh_out_l = GetBodyBoneLocation("thigh_out_l");
	const FVector thigh_correctiveRoot_l = GetBodyBoneLocation("thigh_correctiveRoot_l");
	const FVector thigh_correctiveRoot_r = GetBodyBoneLocation("thigh_correctiveRoot_r");
	const FVector thigh_out_r = GetBodyBoneLocation("thigh_out_r");
	const FVector Length1Vec = FVector((thigh_out_l - thigh_correctiveRoot_l).X, (thigh_out_l - thigh_correctiveRoot_l).Y, 0);
	const FVector Length2Vec = FVector((thigh_correctiveRoot_l - thigh_correctiveRoot_r).X, (thigh_correctiveRoot_l - thigh_correctiveRoot_r).Y, 0);
	const FVector Length3Vec = FVector((thigh_correctiveRoot_r - thigh_out_r).X, (thigh_correctiveRoot_r - thigh_out_r).Y, 0);
	return Length1Vec.Length() + Length2Vec.Length() + Length3Vec.Length();
}

float UAnimControlComponent::GetSeatedHipWidth()
{
	return GetHipWidth();
}

float UAnimControlComponent::GetDifferenceInSpace(FName InSocket1, FName InSocket2) const
{
	return GetLengthByTwoSpacePoint(InSocket1, InSocket2);
}

float UAnimControlComponent::GetDifferenceInSpaceXY(FName InSocket1, FName InSocket2) const
{
	const FVector Vec = GetBodyBoneLocation(InSocket1) - GetBodyBoneLocation(InSocket2);
	return FVector(Vec.X, Vec.Y, 0).Length();
}

float UAnimControlComponent::GetUpperArmLength()
{
	return GetDifferenceInSpace("lowerarm_r", "upperarm_r");
}

float UAnimControlComponent::GetForearmLength()
{
	return GetDifferenceInSpace("hand_l", "lowerarm_l");
}

float UAnimControlComponent::GetCalfLength()
{
	return GetDifferenceInSpace("calf_r", "foot_r");
}

float UAnimControlComponent::GetKneeDistanceInSittingPosition()
{
	return GetLengthToParentSpace("calf_l");
}

float UAnimControlComponent::GetTheWidthBetweenTwoCubits()
{
	return GetDifferenceInSpace("lowerarm_out_r", "lowerarm_out_l");
}

float UAnimControlComponent::GetPalmLeader()
{
	return GetDifferenceInSpace("hand_r", "middle_metacarpal_r")
		+ GetDifferenceInSpace("middle_01_r", "middle_metacarpal_r")
		+ GetDifferenceInSpace("middle_01_r", "middle_01_palmMid_r");
}

float UAnimControlComponent::GetHandLength()
{
	const float PalmLeader = GetPalmLeader();
	const float middle_01_02 = GetDifferenceInSpace("middle_01_r", "middle_02_r");
	const float middle_02_03 = GetDifferenceInSpace("middle_02_r", "middle_03_r");
	if (Body == nullptr)
		return -1;
	const float middle_03_Socket = (GetBodyBoneLocation("middle_03_r") - (Body->GetSocketLocation("middle_03_rSocket"))).Length();
	const float middle_01_palm = GetDifferenceInSpace("middle_01_r", "middle_01_palmMid_r");
	return middle_01_02 + middle_02_03 + middle_03_Socket - middle_01_palm + PalmLeader;
}

float UAnimControlComponent::GetHandWidth()
{
	return GetDifferenceInSpace("index_01_palm_r", "pinky_01_half_r");
}

float UAnimControlComponent::GetIndexFingerLength()
{
	const float index_01_02 = GetDifferenceInSpace("index_01_r", "index_02_r");
	const float index_02_03 = GetDifferenceInSpace("index_02_r", "index_03_r");
	if (Body == nullptr)
		return -1;
	const float index_03_rSocket = (GetBodyBoneLocation("index_03_r") - (Body->GetSocketLocation("index_03_rSocket"))).Length();
	const float index_01_palm = GetDifferenceInSpace("index_01_r", "index_01_palmMid_r");
	return index_01_02 + index_02_03 + index_03_rSocket - index_01_palm;
}

float UAnimControlComponent::GetFingerLength(FName r_01, FName r_02, FName r_03, FName r_01_palm, FName r_03_socket)
{
	const float middle_01_02 = GetDifferenceInSpace(r_01, r_02);
	const float middle_02_03 = GetDifferenceInSpace(r_02, r_03);
	if (Body == nullptr)
		return -1;
	const float middle_03_rSocket = (GetBodyBoneLocation(r_03) - (Body->GetSocketLocation(r_03_socket))).Length();
	const float middle_01_palm = GetDifferenceInSpace(r_01, r_01_palm);
	return  middle_01_02 + middle_02_03 + middle_03_rSocket - middle_01_palm;
}

float UAnimControlComponent::GetMiddleFingerLength()
{
	const float middle_01_02 = GetDifferenceInSpace("middle_01_r", "middle_02_r");
	const float middle_02_03 = GetDifferenceInSpace("middle_02_r", "middle_03_r");
	if (Body == nullptr)
		return -1;
	const float middle_03_rSocket = (GetBodyBoneLocation("middle_03_r") - (Body->GetSocketLocation("middle_03_rSocket"))).Length();
	const float middle_01_palm = GetDifferenceInSpace("middle_01_r", "middle_01_palmMid_r");
	return  middle_01_02 + middle_02_03 + middle_03_rSocket - middle_01_palm;
}

float UAnimControlComponent::GetThumbLength()
{
	const float index_01_02 = GetDifferenceInSpace("thumb_02_r", "thumb_03_r");
	if (Body == nullptr)
		return -1;
	const float thumb_03_rSocket = (GetBodyBoneLocation("thumb_03_r") - Body->GetSocketLocation("thumb_03_rSocket")).Length();
	return index_01_02 + thumb_03_rSocket;
}

float UAnimControlComponent::GetFootLength()
{
	if (Body == nullptr)
		return -1;
	const float foot_r_ankle = UKismetAnimationLibrary::K2_DistanceBetweenTwoSocketsAndMapRange(Body,
		"foot_r",
		RTS_World,
		"ankle_bck_r",
		RTS_World,
		false, 0, 0, 0, 0);
	const float foot_r_ball = UKismetAnimationLibrary::K2_DistanceBetweenTwoSocketsAndMapRange(Body,
		"foot_r",
		RTS_World,
		"ball_r",
		RTS_World,
		false, 0, 0, 0, 0);
	const float ball_r_rSocket = (GetBodyBoneLocation("ball_r") - Body->GetSocketLocation("ball_rSocket")).Length();

	return foot_r_ankle + foot_r_ball + ball_r_rSocket;
}

float UAnimControlComponent::GetFootWidth()
{
	if (Body == nullptr)
		return -1;
	return (Body->GetSocketLocation("ball_rSocket_0") - Body->GetSocketLocation("ball_rSocket_1")).Length();
}

float UAnimControlComponent::GetChestWidth()
{
	return GetDifferenceInSpaceXY("clavicle_out_r", "clavicle_out_l");
}

float UAnimControlComponent::GetShoulderWidth()
{
	return GetDifferenceInSpaceXY("upperarm_in_l", "upperarm_in_r");
}

float UAnimControlComponent::GetTheMaximumShoulderWidth()
{
	return GetDifferenceInSpaceXY("upperarm_out_r", "upperarm_out_l");
}

float UAnimControlComponent::GetBreastThick()
{
	return GetDifferenceInSpaceXY("spine_04_latissimus_l", "clavicle_pec_l");
}

float UAnimControlComponent::GetWeight()
{
	if (BodyAnimInstance)
	{
		return BodyAnimInstance->Weight;
	}
	return -1;
}

FVector UAnimControlComponent::GetHeadOffset()
{
	if (BodyAnimInstance)
	{
		return BodyAnimInstance->HeadCtrl.GetTranslation();
	}
	return FVector::Zero();
}

void UAnimControlComponent::SetHeadOffset(float x)
{
	if (BodyAnimInstance)
	{
		FVector vec = FVector(BodyAnimInstance->HeadCtrl.GetTranslation().X + x, BodyAnimInstance->HeadCtrl.GetTranslation().Y, BodyAnimInstance->HeadCtrl.GetTranslation().Z);
		BodyAnimInstance->HeadCtrl.SetTranslation(vec);
	}
}

float UAnimControlComponent::GetHeadLength()
{
	if (FaceAnimInstance)
	{
		return FaceAnimInstance->Nan_Chang;
	}
	return -1;
}

float UAnimControlComponent::GetHeadWidth()
{
	if (FaceAnimInstance)
	{
		return FaceAnimInstance->Nan_Kuan;
	}
	return -1;
}

float UAnimControlComponent::GetHeadHeight()
{
	if (FaceAnimInstance)
	{
		return FaceAnimInstance->Nan_Gao;
	}
	return -1;
}

void UAnimControlComponent::SetTibiaPointHeight(float InputValue)
{
	if (BodyAnimInstance)
	{
		const float Difference = GetBodyBoneLocationZ("calf_r") - GetBodyBoneLocationZ("root") - InputValue;
		//------------------------------------
		/*
		 * 首先获取到当前的单位向量（foor-calf）
		 * 获取当前的位置（measou）
		 *
		 */
		 /*//当前足部的向量
		 const FVector foorlocation = GetBodyBoneLocation("foot_r");
		 //当前足部到膝盖新位置的向量
		 const FVector foorlocation2 = foorlocation-(GetBodyBoneLocation("calf_r") + FVector(Difference*(-1),0,0));
		 //新的向量
		 const FVector normalized_ = foorlocation2.GetSafeNormal();
		 //获取长度
		 float length = GetLengthByTwoSpacePoint("foot_r","calf_twist_01_r");
		 //获取新的位置
		 const FVector newlocation = foorlocation + normalized_*length;*/
		 //获取新的偏移值
		/*(newlocation - GetBodyBoneLocation("calf_twist_01_r"))*/;

		/*GetUnitVectorByTwoSpacePoint("foot_r","calf_r");
		GetBodyBoneLocation("calf_twist_01_r");
		GetBodyBoneLocation("calf_twist_02_r");*/

		/*const float DifferenceFoot = InputValue - GetBodyBoneLocationZ("foot_r") ;
		FVector VecR = GetBodyBoneLocationBySpace(TEXT("calf_twist_01_r"),RTS_ParentBoneSpace);
		FVector VecR2 = GetBodyBoneLocationBySpace(TEXT("calf_twist_02_r"),RTS_ParentBoneSpace);*/
		//-------------------------------------
		BodyAnimInstance->calf += Difference * (-1);
		BodyAnimInstance->foot += Difference;
		BodyAnimInstance->calf_twist_01_r += FVector(-Difference, 0, 0);
		/*BodyAnimInstance->calf_twist_02_r += FVector(Difference,0,0);
		if(BodyAnimInstance->calf_twist_02_r.X > 2){ BodyAnimInstance->calf_twist_02_r.X = 2;};*/
		BodyAnimInstance->calf_twist_01_l += FVector(Difference, 0, 0);

		BodyAnimInstance->calf_twist_02_r += FVector(Difference, 0, 0);
		BodyAnimInstance->calf_twist_02_l += FVector(-Difference, 0, 0);

		//BodyAnimInstance->calf_twist_01_l = (BodyAnimInstance->calf_twist_01_l.X < -2) ? BodyAnimInstance->calf_twist_01_l.X = -2 : BodyAnimInstance->calf_twist_01_l;

		//BodyAnimInstance->calf_twist_02_l += FVector(-Difference,0,0);
		//3.19fxl---------------
		/*BodyAnimInstance->calf_twist_01_r += FVector(DifferenceFoot*0.6666 - VecR.X,0,0);
		BodyAnimInstance->calf_twist_02_r += FVector(DifferenceFoot*0.3333 - VecR2.X,0,0);*/
		//BodyAnimInstance->calf_twist_01_l = -test;

		//FVector RV_R=FVector::Zero(),RV_L=FVector::Zero();
		/*SetOffsetToParent_(Difference/-2,TEXT("calf_twist_01_r"), TEXT("calf_twist_01_l"),BodyAnimInstance->calf_twist_01_r,BodyAnimInstance->calf_twist_01_l, RV_R, RV_L );
		BodyAnimInstance->calf_twist_01_r = RV_R;
		BodyAnimInstance->calf_twist_01_l = RV_L;*/

		/*SetOffsetToParent_(Difference/-2,TEXT("calf_twist_02_r"), TEXT("calf_twist_02_l"),BodyAnimInstance->calf_twist_02_r,BodyAnimInstance->calf_twist_02_l, RV_R, RV_L );
		BodyAnimInstance->calf_twist_02_r = RV_R;
		BodyAnimInstance->calf_twist_02_l = RV_L;*/
	}
}

void UAnimControlComponent::SetCalfLength(float InputValue)
{
	if (BodyAnimInstance)
	{
		const float Difference = GetBodyBoneLocationZ("calf_r") - GetBodyBoneLocationZ("foot_r") - InputValue;
		BodyAnimInstance->calf += Difference * (-1);
		BodyAnimInstance->foot += Difference;

		//**********************
		BodyAnimInstance->calf_twist_01_r += FVector(-Difference, 0, 0);
		BodyAnimInstance->calf_twist_01_l += FVector(Difference, 0, 0);
		BodyAnimInstance->calf_twist_02_r += FVector(Difference, 0, 0);
		BodyAnimInstance->calf_twist_02_l += FVector(-Difference, 0, 0);
		//**********************
	}
}

void UAnimControlComponent::SetThighLength(float InputValue)
{
	if (BodyAnimInstance)
	{
		const float Difference = InputValue - (GetBodyBoneLocationZ("thigh_r") - GetBodyBoneLocationZ("calf_r"));
		BodyAnimInstance->calf += Difference * (-1);
		BodyAnimInstance->pelvis += Difference;

		//**********************
		BodyAnimInstance->calf_twist_02_r += FVector(Difference, 0, 0);
		BodyAnimInstance->calf_twist_02_l += FVector(-Difference, 0, 0);
		//**********************

		float RV1 = 0, RV2 = 0, RV3 = 0, RV4 = 0, RV5 = 0, Res = 0;
		const float length = GetBodyBoneLocationZ("spine_05") - GetBodyBoneLocationZ("pelvis");
		const float Distance = Difference * -1;
		SetDifferenceOfTheLumbarVertebrae(length, Distance, RV1, RV2, RV3, RV4, RV5, Res);
		const FVector Nor = GetUnitVectorByTwoSpacePoint("spine_05", "root");

		BodyAnimInstance->spine_1 += RV1 * Nor;
		BodyAnimInstance->spine_2 += RV2 * Nor;
		BodyAnimInstance->spine_3 += RV3 * Nor;
		BodyAnimInstance->spine_4 += RV4 * Nor;
		BodyAnimInstance->spine_5 += RV5 * Nor;
	}
}

void UAnimControlComponent::SetPerinealHeight(float InputValue)
{
	if (BodyAnimInstance)
	{
		const float Difference = InputValue - (GetBodyBoneLocationZ("thigh_in_r") - GetBodyBoneLocationZ("root"));
		BodyAnimInstance->calf += Difference * (-1);
		BodyAnimInstance->pelvis += Difference;

		float RV1 = 0, RV2 = 0, RV3 = 0, RV4 = 0, RV5 = 0, Res = 0;
		const float length = GetBodyBoneLocationZ("spine_05") - GetBodyBoneLocationZ("pelvis");
		const float Distance = Difference * -1;
		SetDifferenceOfTheLumbarVertebrae(length, Distance, RV1, RV2, RV3, RV4, RV5, Res);
		const FVector Nor = GetUnitVectorByTwoSpacePoint("spine_05", "root");

		BodyAnimInstance->spine_1 += RV1 * Nor;
		BodyAnimInstance->spine_2 += RV2 * Nor;
		BodyAnimInstance->spine_3 += RV3 * Nor;
		BodyAnimInstance->spine_4 += RV4 * Nor;
		BodyAnimInstance->spine_5 += RV5 * Nor;
	}
}

void UAnimControlComponent::SetShoulderHeight(float InputValue)
{
	if (BodyAnimInstance)
	{
		const float Difference = InputValue - (GetBodyBoneLocationZ("clavicle_out_r") - GetBodyBoneLocationZ("root"));

		float RV1 = 0, RV2 = 0, RV3 = 0, RV4 = 0, RV5 = 0, Res = 0;
		const float length = GetBodyBoneLocationZ("spine_05") - GetBodyBoneLocationZ("pelvis");
		const float Distance = Difference;
		SetDifferenceOfTheLumbarVertebrae(length, Distance, RV1, RV2, RV3, RV4, RV5, Res);
		const FVector Nor = GetUnitVectorByTwoSpacePoint("spine_05", "root");

		BodyAnimInstance->spine_1 += RV1 * Nor;
		BodyAnimInstance->spine_2 += RV2 * Nor;
		BodyAnimInstance->spine_3 += RV3 * Nor;
		BodyAnimInstance->spine_4 += RV4 * Nor;
		BodyAnimInstance->spine_5 += RV5 * Nor;

		const float length_head = GetHeadBoneLocationZ("head") - GetBodyBoneLocationZ("spine_05");
		const float distance_head = Difference * -1;
		float RV1_head = 0, RV2_head = 0, RV3_head = 0;
		SetDifferenceOfTheCervicalVertebrae(length_head, distance_head, RV1_head, RV2_head, RV3_head);

		BodyAnimInstance->neck_01 += RV1_head;
		BodyAnimInstance->neck_02 += RV2_head;
		BodyAnimInstance->head += RV3_head;
	}
}

void UAnimControlComponent::SetEyeHeight(float InputValue)
{
	if (BodyAnimInstance)
	{
		const float Difference = InputValue - (GetHeadBoneLocationZ("FACIAL_L_Eye") - GetBodyBoneLocationZ("root"));
		const float length_head = GetHeadBoneLocationZ("head") - GetBodyBoneLocationZ("spine_05");
		const float distance_head = Difference;
		float RV1_head = 0, RV2_head = 0, RV3_head = 0;
		SetDifferenceOfTheCervicalVertebrae(length_head, distance_head, RV1_head, RV2_head, RV3_head);

		BodyAnimInstance->neck_01 += RV1_head;
		BodyAnimInstance->neck_02 += RV2_head;
		BodyAnimInstance->head += RV3_head;
	}
}

void UAnimControlComponent::SetUpperArmLength(float InputValue)
{
	if (BodyAnimInstance)
	{
		FVector RV_R = FVector::Zero(), RV_L = FVector::Zero();
		SetOffsetToParent(InputValue, TEXT("lowerarm_r"), TEXT("lowerarm_l"), RV_R, RV_L);
		BodyAnimInstance->lowerarm_r = RV_R;
		BodyAnimInstance->lowerarm_l = RV_L;
	}
}

void UAnimControlComponent::SetForearmLength(float InputValue)
{
	if (BodyAnimInstance)
	{
		FVector RV_R = FVector::Zero(), RV_L = FVector::Zero();
		SetOffsetToParent(InputValue, TEXT("hand_r"), TEXT("hand_l"), RV_R, RV_L);
		BodyAnimInstance->hand_r = RV_R;
		BodyAnimInstance->hand_l = RV_L;
	}
}

void UAnimControlComponent::SetFootLength(float InputValue)
{
	const float FootLength = GetFootLength();

	const float Difference = InputValue - FootLength;
	const FVector Nor = GetBodyBoneLocationBySpace("ball_r", RTS_ParentBoneSpace).GetSafeNormal();
	if (BodyAnimInstance)
	{
		BodyAnimInstance->ball_location += Difference * Nor;
	}
}

void UAnimControlComponent::SetFootWidth(float InputValue)
{
	const float FootWidth = GetFootWidth();
	const float Difference = InputValue / FootWidth;
	if (BodyAnimInstance)
	{
		BodyAnimInstance->ball_scale = FVector(1, 1, Difference * BodyAnimInstance->ball_scale.Z);
	}
}

void UAnimControlComponent::SetIndexFingerLength(float InputValue)
{
	const float IndexFingerLength = GetIndexFingerLength();
	const float Difference = InputValue - IndexFingerLength;
	const FVector index_02 = GetBodyBoneLocationBySpace("index_02_r", RTS_ParentBoneSpace).GetSafeNormal();
	const FVector index_03 = GetBodyBoneLocationBySpace("index_03_r", RTS_ParentBoneSpace).GetSafeNormal();

	const FVector index_04 = GetBodyBoneLocationBySpace("index_02_l", RTS_ParentBoneSpace).GetSafeNormal();
	const FVector index_05 = GetBodyBoneLocationBySpace("index_03_l", RTS_ParentBoneSpace).GetSafeNormal();
	if (BodyAnimInstance)
	{
		BodyAnimInstance->index_02_r += index_02 * Difference * 0.6;
		BodyAnimInstance->index_03_r += index_03 * Difference * 0.4;
		BodyAnimInstance->index_02_l += index_04 * Difference * 0.6;
		BodyAnimInstance->index_03_l += index_05 * Difference * 0.4;
	}
}

void UAnimControlComponent::SetMiddleFingerLength(float InputValue)
{
	const float MiddleFingerLength = GetMiddleFingerLength();
	const float Difference = InputValue - MiddleFingerLength;

	const FVector middle_02 = GetBodyBoneLocationBySpace("middle_02_r", RTS_ParentBoneSpace).GetSafeNormal();
	const FVector middle_03 = GetBodyBoneLocationBySpace("middle_03_r", RTS_ParentBoneSpace).GetSafeNormal();
	//----------------------------------------
	const FVector middle_04 = GetBodyBoneLocationBySpace("middle_02_l", RTS_ParentBoneSpace).GetSafeNormal();
	const FVector middle_05 = GetBodyBoneLocationBySpace("middle_03_l", RTS_ParentBoneSpace).GetSafeNormal();

	const float pincklength = GetFingerLength("pinky_01_r", "pinky_02_r", "pinky_03_r", "pinky_01_palmMid_r", "pinky_03_rSocket");
	const float ringlength = GetFingerLength("ring_01_r", "ring_02_r", "ring_03_r", "ring_01_palmMid_r", "ring_03_rSocket");

	//----------------------------------------
	if (BodyAnimInstance)
	{
		BodyAnimInstance->middle_02_r += middle_02 * Difference * 0.6;
		BodyAnimInstance->middle_03_r += middle_03 * Difference * 0.4;
		//--------------------------------------------
		BodyAnimInstance->middle_02_l += middle_04 * Difference * 0.6;
		BodyAnimInstance->middle_03_l += middle_05 * Difference * 0.4;

		SetPinkyFingerLength(pincklength / MiddleFingerLength * InputValue, pincklength);
		SetRingFingerLength(ringlength / MiddleFingerLength * InputValue, ringlength);
		//--------------------------------------------
	}
}

void UAnimControlComponent::SetPinkyFingerLength(float InputValue, float length)
{
	const float Difference = InputValue - length;

	const FVector middle_02 = GetBodyBoneLocationBySpace("pinky_02_r", RTS_ParentBoneSpace).GetSafeNormal();
	const FVector middle_03 = GetBodyBoneLocationBySpace("pinky_03_r", RTS_ParentBoneSpace).GetSafeNormal();

	const FVector middle_04 = GetBodyBoneLocationBySpace("pinky_02_l", RTS_ParentBoneSpace).GetSafeNormal();
	const FVector middle_05 = GetBodyBoneLocationBySpace("pinky_03_l", RTS_ParentBoneSpace).GetSafeNormal();
	if (BodyAnimInstance)
	{
		BodyAnimInstance->pinky_02_r += middle_02 * Difference * 0.6;
		BodyAnimInstance->pinky_03_r += middle_03 * Difference * 0.4;

		BodyAnimInstance->pinky_02_l += middle_04 * Difference * 0.6;
		BodyAnimInstance->pinky_03_l += middle_05 * Difference * 0.4;
	}
}
void UAnimControlComponent::SetRingFingerLength(float InputValue, float length)
{
	const float Difference = InputValue - length;

	const FVector middle_02 = GetBodyBoneLocationBySpace("ring_02_r", RTS_ParentBoneSpace).GetSafeNormal();
	const FVector middle_03 = GetBodyBoneLocationBySpace("ring_03_r", RTS_ParentBoneSpace).GetSafeNormal();

	const FVector middle_04 = GetBodyBoneLocationBySpace("ring_02_l", RTS_ParentBoneSpace).GetSafeNormal();
	const FVector middle_05 = GetBodyBoneLocationBySpace("ring_03_l", RTS_ParentBoneSpace).GetSafeNormal();
	if (BodyAnimInstance)
	{
		BodyAnimInstance->ring_02_r += middle_02 * Difference * 0.6;
		BodyAnimInstance->ring_02_r += middle_03 * Difference * 0.4;

		BodyAnimInstance->ring_02_l += middle_04 * Difference * 0.6;
		BodyAnimInstance->ring_02_l += middle_05 * Difference * 0.4;
	}
}

void UAnimControlComponent::SetThumbLength(float InputValue)
{
	const float ThumbLength = GetThumbLength();
	const float Difference = InputValue - ThumbLength;
	const FVector Nor = GetBodyBoneLocationBySpace("thumb_03_r", RTS_ParentBoneSpace).GetSafeNormal();

	const FVector Nor1 = GetBodyBoneLocationBySpace("thumb_03_l", RTS_ParentBoneSpace).GetSafeNormal();
	if (BodyAnimInstance)
	{
		BodyAnimInstance->thumb_03_r += Nor * Difference;

		BodyAnimInstance->thumb_03_l += Nor1 * Difference;
	}
}

void UAnimControlComponent::SetPalmLength(float InputValue)
{
	const float PalmLength = GetPalmLeader();
	const float Difference = InputValue - PalmLength;

	/*
	const FVector Nor_middle_meta = GetBodyBoneLocationBySpace("middle_metacarpal_r",RTS_ParentBoneSpace).GetSafeNormal();
	const FVector Nor_middle_01 = GetBodyBoneLocationBySpace("middle_01_r",RTS_ParentBoneSpace).GetSafeNormal();
	if(BodyAnimInstance)
	{
		BodyAnimInstance->middle_01_r += Nor_middle_meta * 0.2 * Difference;
		BodyAnimInstance->middle_meta_r += Nor_middle_01 * 0.8 * Difference;
	}*/

	//3.21fxl----------------------------------------------
	if (BodyAnimInstance)
	{
		BodyAnimInstance->middle_01_r += (GetBodyBoneLocationBySpace("middle_01_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
		BodyAnimInstance->middle_meta_r += (GetBodyBoneLocationBySpace("middle_metacarpal_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);
		BodyAnimInstance->middle_01_l += (GetBodyBoneLocationBySpace("middle_01_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
		BodyAnimInstance->middle_meta_l += (GetBodyBoneLocationBySpace("middle_metacarpal_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);

		BodyAnimInstance->pinky_01_r += (GetBodyBoneLocationBySpace("pinky_01_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
		BodyAnimInstance->pinky_meta_r += (GetBodyBoneLocationBySpace("pinky_metacarpal_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);
		BodyAnimInstance->pinky_01_l += (GetBodyBoneLocationBySpace("pinky_01_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
		BodyAnimInstance->pinky_meta_l += (GetBodyBoneLocationBySpace("pinky_metacarpal_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);

		BodyAnimInstance->ring_01_r += (GetBodyBoneLocationBySpace("ring_01_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
		BodyAnimInstance->ring_meta_r += (GetBodyBoneLocationBySpace("ring_metacarpal_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);
		BodyAnimInstance->ring_01_l += (GetBodyBoneLocationBySpace("ring_01_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
		BodyAnimInstance->ring_meta_l += (GetBodyBoneLocationBySpace("ring_metacarpal_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);

		BodyAnimInstance->index_01_r += (GetBodyBoneLocationBySpace("index_01_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
		BodyAnimInstance->index_meta_r += (GetBodyBoneLocationBySpace("index_metacarpal_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);
		BodyAnimInstance->index_01_l += (GetBodyBoneLocationBySpace("index_01_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
		BodyAnimInstance->index_meta_l += (GetBodyBoneLocationBySpace("index_metacarpal_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);

		BodyAnimInstance->thumb_01_r += (GetBodyBoneLocationBySpace("thumb_01_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);
		BodyAnimInstance->thumb_02_r += (GetBodyBoneLocationBySpace("thumb_02_r", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
		BodyAnimInstance->thumb_01_l += (GetBodyBoneLocationBySpace("thumb_01_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.2 * Difference);
		BodyAnimInstance->thumb_02_l += (GetBodyBoneLocationBySpace("thumb_02_l", RTS_ParentBoneSpace).GetSafeNormal() * 0.8 * Difference);
	}
	//-----------------------------------------------------
}

void UAnimControlComponent::SetHandWidth(float InputValue)
{
	const float Difference = InputValue - GetHandWidth();
	const FVector Nor = GetUnitVectorByTwoSpacePoint("index_01_palm_r", "pinky_01_half_r");
	if (BodyAnimInstance)
	{
		BodyAnimInstance->Translation += Nor * Difference;
	}
}

void UAnimControlComponent::SetHandLength(float InputValue)
{
	const float Difference = InputValue - GetPalmLeader();
	SetMiddleFingerLength(Difference);
}

void UAnimControlComponent::SetShoulderHeightForSittingPosition(float InputValue)
{
	const float Difference = InputValue - GetSittingShoulderHeight();
	const float Length = GetBodyBoneLocationZ("spine_05") - GetBodyBoneLocationZ("pelvis");

	float RV1 = 0, RV2 = 0, RV3 = 0, RV4 = 0, RV5 = 0, Res = 0;
	const float Distance = Difference;
	SetDifferenceOfTheLumbarVertebrae(Length, Distance, RV1, RV2, RV3, RV4, RV5, Res);
	const FVector Nor = FVector(0, 0, 1);

	BodyAnimInstance->spine_1 += RV1 * Nor;
	BodyAnimInstance->spine_2 += RV2 * Nor;
	BodyAnimInstance->spine_3 += RV3 * Nor;
	BodyAnimInstance->spine_4 += RV4 * Nor;
	BodyAnimInstance->spine_5 += RV5 * Nor;
}

void UAnimControlComponent::SetEyeHeightForSittingPosition(float InputValue)
{
	const float Difference = InputValue - GetSittingEyeHeight();
	if (BodyAnimInstance)
	{
		const float Distance = Difference - BodyAnimInstance->pelvis;
		const float Length = GetHeadBoneLocationZ("head") - GetBodyBoneLocationZ("spine_05");
		float RV1_head = 0, RV2_head = 0, RV3_head = 0;
		SetDifferenceOfTheCervicalVertebrae(Length, Distance, RV1_head, RV2_head, RV3_head);

		BodyAnimInstance->neck_01 += RV1_head;
		BodyAnimInstance->neck_02 += RV2_head;
		BodyAnimInstance->head += RV3_head;
	}

}

void UAnimControlComponent::SetElbowHeightForSittingPosition(float InputValue)
{
	const float Difference = InputValue - GetSeatedElbowHeight();
	if (BodyAnimInstance)
	{
		BodyAnimInstance->lowerarm_l += FVector(Difference * -1, 0, 0);
		BodyAnimInstance->lowerarm_r += FVector(Difference, 0, 0);
	}
}

void UAnimControlComponent::SetKneeHeightForSittingPosition(float InputValue)
{
	const float Difference = InputValue - GetSeatedKneeHeight();
	if (BodyAnimInstance)
	{
		BodyAnimInstance->pelvis += Difference;
		BodyAnimInstance->foot += Difference * -1;
	}
}

void UAnimControlComponent::SetChestThickness(float InputValue)
{
	const float ChestThick = GetBreastThick();
	const float Difference = InputValue / ChestThick;
	if (BodyAnimInstance)
	{
		BodyAnimInstance->ChestDepth *= Difference;
	}
}

void UAnimControlComponent::SetHipWidth(float InputValue)
{
	const float HipWidth = GetHipWidth();
	const float Difference = (InputValue - HipWidth) / 2.0;
	const FVector Vec = GetBodyBoneLocation("thigh_correctiveRoot_r") - GetBodyBoneLocation("thigh_correctiveRoot_l");
	const FVector Nor = Vec.GetSafeNormal2D();
	const FVector NegateNor = UKismetMathLibrary::NegateVector(Nor);
	if (BodyAnimInstance)
	{
		BodyAnimInstance->HipWidthL += Difference * NegateNor;
		BodyAnimInstance->HipWidthR += Difference * Nor;
	}
}

void UAnimControlComponent::SetShoulderWidth(float InputValue)
{
	const float ShoulderWidth = GetShoulderWidth();
	const float Difference = (InputValue - ShoulderWidth) / 2.0;
	if (BodyAnimInstance)
	{
		BodyAnimInstance->ShoulderWidth += Difference;
	}
}

void UAnimControlComponent::SetMaximumShoulderWidth(float InputValue)
{
	const float Difference = (InputValue - GetTheMaximumShoulderWidth()) / 2.0;
	if (BodyAnimInstance)
	{
		BodyAnimInstance->MaxShoulderWidth += Difference;
	}
}

void UAnimControlComponent::SetChestWidth(float InputValue)
{
	const float Difference = (InputValue - GetChestWidth()) / 2.0;
	if (BodyAnimInstance)
	{
		BodyAnimInstance->ChestWidth += Difference;
	}
}

void UAnimControlComponent::SetSeatedHipWidth(float InputValue)
{
	SetHipWidth(InputValue);
}

void UAnimControlComponent::SetHipKneeDistanceForSittingPosition(float InputValue)
{
	const float Difference = InputValue - GetKneeDistanceInSittingPosition();
	if (BodyAnimInstance)
	{
		BodyAnimInstance->SittingHipKneeDistance += Difference;
	}
}

void UAnimControlComponent::SetSittingPositionTwoElbows(float InputValue)
{
	const float Difference = (InputValue - GetTheWidthBetweenTwoCubits()) / 4.0;
	if (BodyAnimInstance)
	{
		BodyAnimInstance->ChestWidth += Difference;
		BodyAnimInstance->ShoulderWidth += Difference;
	}
}

void UAnimControlComponent::SetHeadHeight(float InputValue)
{
	if (FaceAnimInstance)
	{
		const float ClampValue = FMath::Clamp(InputValue, 22.33f, 27.77f);
		FaceAnimInstance->Nan_Gao = ClampValue;
	}
}

void UAnimControlComponent::SetHeadWidth(float InputValue)
{
	if (FaceAnimInstance)
	{
		const float ClampValue = FMath::Clamp(InputValue, 18.4f, 22.58f);
		FaceAnimInstance->Nan_Kuan = ClampValue;
	}
}

void UAnimControlComponent::SetHeadLength(float InputValue)
{
	if (FaceAnimInstance)
	{
		const float ClampValue = FMath::Clamp(InputValue, 18.4f, 24.58f);
		FaceAnimInstance->Nan_Chang = ClampValue;
	}
}

void UAnimControlComponent::SetWeight(float InputValue)
{
	if (BodyAnimInstance)
	{
		BodyAnimInstance->Weight = InputValue;
	}
}

float UAnimControlComponent::GetCurrentDistance(double B, double A) const
{
	return A / BaseHeight * B;
}
