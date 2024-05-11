// Copyright Gabriel Pizarro. All Rights Reserved.

#include "AlloSubsystem.h"

#include "AlloSceneViewExtension.h"
#include "Kismet/GameplayStatics.h"

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
	
	FIntPoint ScreenResolution{Info.Size.X, Info.Size.Y * 2};
	
	size_t ViewIdx = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It && ViewIdx < 2; ++It)
	{
		APlayerController* PC = It->Get();
		if (!PC->IsLocalController()) continue;
		
		UGameViewportClient* VP = GetWorld()->GetGameViewport();
		check(VP);
		VP->SetDisplayConfiguration(&ScreenResolution, EWindowMode::Windowed);
		
		PC->FOV(Info.FOV);
		PC->SetControlRotation(Info.Rotations[ViewIdx]);
		
		ViewIdx++;
	}
}
