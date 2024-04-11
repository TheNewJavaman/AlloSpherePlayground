// Copyright Gabriel Pizarro. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AlloSubsystem.generated.h"

class UAlloCaptureComponent;
class FAlloSceneViewExtension;

UCLASS()
class UAlloSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;

private:
	TSharedPtr<FAlloSceneViewExtension> SceneViewExtension;
	TArray<TWeakObjectPtr<UAlloCaptureComponent>> CaptureComponents;
};
