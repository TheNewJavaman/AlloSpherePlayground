// Copyright Gabriel Pizarro. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AlloSubsystem.generated.h"

class FAlloSceneViewExtension;

UCLASS()
class UAlloSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	TOptional<TSharedRef<FAlloSceneViewExtension>> AlloSceneViewExtension;
};
