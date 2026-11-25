#pragma once
#include "ECSSystemRegistration.h"

namespace ECS
{
	template <typename TSystem, typename... TArgs>
	TSystem& FSystemRegistrar::Add(TArgs&&... Args)
	{
		static_assert(std::is_base_of_v<ISystem, TSystem>, "Provided system type must derive from ISystem");

		if constexpr (std::is_constructible_v<TSystem, UWorld*, TArgs...>)
		{
			return static_cast<TSystem&>(Sink(MakeUnique<TSystem>(World, Forward<TArgs>(Args)...)));
		}
		else if constexpr (std::is_constructible_v<TSystem, TArgs..., UWorld*>)
		{
			return static_cast<TSystem&>(Sink(MakeUnique<TSystem>(Forward<TArgs>(Args)..., World)));
		}
		else if constexpr (std::is_constructible_v<TSystem, UWorld&, TArgs...>)
		{
			check(World);
			return static_cast<TSystem&>(Sink(MakeUnique<TSystem>(*World, Forward<TArgs>(Args)...)));
		}
		else
		{
			static_assert(std::is_constructible_v<TSystem, TArgs...>,
				"TSystem is not constructible from the given args");
			return static_cast<TSystem&>(Sink(MakeUnique<TSystem>(Forward<TArgs>(Args)...)));
		}
	}
}