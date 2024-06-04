// Copyright Gabriel Pizarro. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AlloSubsystem.generated.h"

class FAlloSceneViewExtension;

struct FAlloCalibration
{
	int32 PlayerIndex;
	FString Id;
	FString Filepath;
	FIntPoint Resolution;
	FVector2D WindowPosition;
	FVector2D WindowExtent;
	bool Active;
	float FovInRad;
	FRotator Rotation;
	UTexture2D* WarpAndBlendTexture;
	FMatrix44f RotationMatrix;
};

UCLASS()
class UAlloSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual TStatId GetStatId() const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;

	void LoadCalibrations();

	TOptional<TArray<FAlloCalibration, TInlineAllocator<2>>> Calibrations;
	
private:
	TSharedPtr<FAlloSceneViewExtension> SceneViewExtension;
};
