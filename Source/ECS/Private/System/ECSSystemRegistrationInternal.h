#pragma once
#include "System/ECSSystemRegistration.h"

namespace ECS
{
	void ForEachProvider(TFunctionRef<void(const FSystemsProvider&)> Fn);
}
