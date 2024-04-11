// Copyright Gabriel Pizarro. All Rights Reserved.

#pragma once

#include "al/app/al_DistributedApp.hpp"

class FAlloUnrealApp : public al::DistributedApp
{
	virtual void onInit() override
	{
		graphics().texture();
	}
};

class FAlloLibNative
{
private:
	TUniquePtr<FAlloUnrealApp> App;
};
