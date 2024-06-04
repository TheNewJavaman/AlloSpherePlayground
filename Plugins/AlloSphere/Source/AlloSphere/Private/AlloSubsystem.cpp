// Copyright Gabriel Pizarro. All Rights Reserved.

#include "AlloSubsystem.h"

#define _BITS_TYPES___LOCALE_T_H 1
#include <fstream>

#include "AlloLog.h"
#include "AlloSceneViewExtension.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DefaultValueHelper.h"

TStatId UAlloSubsystem::GetStatId() const
{
	return GetStatID();
}

void UAlloSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SceneViewExtension = FSceneViewExtensions::NewExtension<FAlloSceneViewExtension>(MakeWeakObjectPtr(this));
}

void UAlloSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	UGameplayStatics::CreatePlayer(&InWorld, -1);
}

void UAlloSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Calibrations.IsSet())
	{
		LoadCalibrations();
	}

	int32 i = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It && i < 2; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC->IsLocalController()) continue;

		FAlloCalibration& Calibration = (*Calibrations)[i];

		UGameViewportClient* VP = GetWorld()->GetGameViewport();
		check(VP);

		FIntPoint WindowResolution = Calibration.Resolution;
		WindowResolution.X *= 2;
		VP->SetDisplayConfiguration(&WindowResolution, EWindowMode::Windowed);
		PC->FOV(FMath::RadiansToDegrees(Calibration.FovInRad));
		PC->SetControlRotation(Calibration.Rotation);
		Calibration.PlayerIndex = PC->PlayerState->GetPlayerId();

		i++;
	}
}

void UAlloSubsystem::LoadCalibrations()
{
	Calibrations = TArray<FAlloCalibration, TInlineAllocator<2>>();
	Calibrations->SetNum(2);

	const auto Filename = TEXT("/home/gpizarro/calibration-current/gr02.txt");
	int32 i = 0;
	auto Visitor = [&](const FStringView Line)
	{
		if (Line == TEXT(""))
		{
			i++;
			return;
		}

		const int32 SpaceIdx = Line.Find(TEXT(" "));
		FString Key = FString(Line.SubStr(0, SpaceIdx));
		FString Val = FString(Line.SubStr(SpaceIdx + 1, Line.Len() - SpaceIdx - 1));

		FAlloCalibration& Calibration = (*Calibrations)[i];
		if (Key == TEXT("id"))
		{
			Calibration.Id = Val;
		}
		else if (Key == TEXT("filepath"))
		{
			Calibration.Filepath = Val;
		}
		else if (Key == TEXT("width"))
		{
			FDefaultValueHelper::ParseInt(Val, Calibration.Resolution.X);
		}
		else if (Key == TEXT("height"))
		{
			FDefaultValueHelper::ParseInt(Val, Calibration.Resolution.Y);
		}
		else if (Key == TEXT("b"))
		{
			FDefaultValueHelper::ParseDouble(Val, Calibration.WindowPosition.Y);
		}
		else if (Key == TEXT("h"))
		{
			FDefaultValueHelper::ParseDouble(Val, Calibration.WindowExtent.Y);
		}
		else if (Key == TEXT("l"))
		{
			FDefaultValueHelper::ParseDouble(Val, Calibration.WindowPosition.X);
		}
		else if (Key == TEXT("w"))
		{
			FDefaultValueHelper::ParseDouble(Val, Calibration.WindowExtent.Y);
		}
		else if (Key == TEXT("active"))
		{
			int32 Active;
			FDefaultValueHelper::ParseInt(Val, Active);
			Calibration.Active = static_cast<bool>(Active);
		}
		else
		{
			UE_LOG(LogAlloSphere, Warning, TEXT("Unknown calibration key: %s"), *Key);
			return;
		}
		UE_LOG(LogAlloSphere, Display, TEXT("Calibration[%d]: %s = %s"), i, *Key, *Val);
	};
	FFileHelper::LoadFileToStringWithLineVisitor(Filename, Visitor);

	for (FAlloCalibration& Calibration : *Calibrations)
	{
		Calibration.FovInRad = 2.0 / 3.0 * PI;

		TArray64<FVector4f> WarpAndBlendData;
		WarpAndBlendData.SetNum(Calibration.Resolution.X * Calibration.Resolution.Y);
		std::ifstream File(*Calibration.Filepath, std::ios::in | std::ios::binary);
		if (!File)
		{
			UE_LOG(LogAlloSphere, Error, TEXT("Failed to open file: %s"), *Calibration.Filepath);
			return;
		}
		File.read(reinterpret_cast<char*>(WarpAndBlendData.GetData()), sizeof(FVector4f) * WarpAndBlendData.Num());
		File.close();

		TArrayView64<uint8> WarpAndBlendCast(
			reinterpret_cast<uint8*>(WarpAndBlendData.GetData()),
			sizeof(FVector4f) * WarpAndBlendData.Num());
		Calibration.WarpAndBlendTexture = UTexture2D::CreateTransient(
			Calibration.Resolution.X,
			Calibration.Resolution.Y,
			PF_A32B32G32R32F,
			NAME_None,
			WarpAndBlendCast);
		Calibration.WarpAndBlendTexture->UpdateResource();

		FVector3f Direction(0, 0, 0);
		for (const FVector4f& Entry : WarpAndBlendData)
		{
			Direction.X += Entry.X;
			Direction.Y += Entry.Y;
			Direction.Z += Entry.Z;
		}
		Direction.Normalize();
		Direction = FVector3f(-Direction.Z, Direction.X, Direction.Y); // Convert AlloSphere/OpenGL -> Unreal
		Calibration.Rotation = FVector(Direction).ToOrientationRotator();

		const float Angle = FMath::Acos(FVector3f::ForwardVector.Dot(Direction));
		const FVector3f Axis = FVector3f::ForwardVector.Cross(Direction).GetUnsafeNormal();
		const FQuat4f Quat(Axis, -Angle);
		Calibration.RotationMatrix = FQuatRotationMatrix44f(Quat);

		// Calibration.RotationMatrix = FMatrix44f{
		// 	{
		// 		CosT + X * X * (1 - CosT),
		// 		X * Y * (1 - CosT) - Z * SinT,
		// 		X * Z * (1 - CosT) + Y * SinT,
		// 		0
		// 	},
		// 	{
		// 		Y * X * (1 - CosT) + Z * SinT,
		// 		CosT + Y * Y * (1 - CosT),
		// 		Y * Z * (1 - CosT) - X * SinT,
		// 		0
		// 	},
		// 	{
		// 		Z * X * (1 - CosT) - Y * SinT,
		// 		Z * Y * (1 - CosT) + X * SinT,
		// 		CosT + Z * Z * (1 - CosT),
		// 		0
		// 	},
		// 	{0, 0, 0, 1}
		// };
	}
}
