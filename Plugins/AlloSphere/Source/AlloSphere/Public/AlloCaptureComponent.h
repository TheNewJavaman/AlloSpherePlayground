// Copyright Gabriel Pizarro. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"

#include "AlloCaptureComponent.generated.h"

UCLASS()
class UAlloCaptureComponent : public USceneCaptureComponent2D
{
	GENERATED_BODY()

public:
	int Id;
};
