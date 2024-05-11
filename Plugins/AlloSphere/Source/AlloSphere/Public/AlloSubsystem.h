// Copyright Gabriel Pizarro. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AlloSubsystem.generated.h"

class FAlloSceneViewExtension;

struct FProjectorView
{
	FRotator Rotation;
};

struct FProjectorViews
{
	FIntPoint Size;
	double FOV;
	FRotator Rotations[2];
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

	FProjectorViews Info{
		.Size{640, 480},
		.FOV = 90.0,
		.Rotations{
			FRotator{45, 0, 0},
			FRotator{-45, 0, 0}
		}
	};
	
private:
	TSharedPtr<FAlloSceneViewExtension> SceneViewExtension;
};
