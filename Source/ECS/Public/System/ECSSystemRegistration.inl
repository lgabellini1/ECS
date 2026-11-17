#pragma once
#include "ECSSystemRegistration.h"

namespace ECS
{
	template <typename TSystem, typename... TArgs>
	TSystem& FSystemRegistrar::Add(TArgs&&... Args)
	{
		return static_cast<TSystem&>(
			Sink(MakeUnique<TSystem>(Forward<TArgs>(Args)...))
		);
	}
}