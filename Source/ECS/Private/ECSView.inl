#pragma once

#include "ECSFunctions.h"
#include "ECSRegistry.h"
#include "ECSUtils.h"

namespace ECS
{
	template<component... TInclude>
	const IStorage* FindSmallerStorage(const FRegistry& Registry)
	{
		const IStorage* SmallestStorage = nullptr;

		auto CheckSmaller = [&SmallestStorage](const IStorage* Storage)
		{
			if (!Storage) return;
			
			if (!SmallestStorage || Storage->Num() < SmallestStorage->Num())
			{
				SmallestStorage = Storage;
			}
		};

		(CheckSmaller(Registry.GetStorage<TInclude>()), ...);
		return SmallestStorage;
	}
	
	template <component... TInclude, component... TExcluded, typename Fn>
	void ForEach(FRegistry& Registry, TExclude<TExcluded...>, Fn&& Func)
	{
		static_assert(sizeof...(TInclude) > 0, "ForEach needs at least one included component");

		if ((... && (Registry.GetStorage<TInclude>() != nullptr)))
		{
			for (const IStorage* SmallestStorage = FindSmallerStorage<TInclude...>(Registry);
				const FEntity Entity : SmallestStorage->GetEntities())
			{
				const bool bHasAll = (... && Registry.Has<TInclude>(Entity));
				const bool bExcluded = (... || Registry.Has<TExcluded>(Entity));
				
				if (bHasAll && !bExcluded)
				{
					Func(Entity, *Registry.Get<TInclude>(Entity)...);
				}
			}
		}
	}
	
	// Utility with no exclusion
	template <component... TInclude, typename Fn>
	void ForEach(FRegistry& Registry, Fn&& Func)
	{
		ForEach<TInclude...>(Registry, TExclude<>{}, Forward<Fn>(Func));
	}
}
