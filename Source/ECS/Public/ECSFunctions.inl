#pragma once
#include "ECSFunctions.h"
#include "Registry/ECSRegistry.h"
#include "Storage/ECSComponentStorage.h"

namespace ECS
{
	template <component... TInclude, component... TExclude, typename Fn>
	void ForEach(FRegistry& Registry, TExcludeSet<TExclude...>, Fn&& Func)
	{
		static_assert(sizeof...(TInclude) > 0, "ForEach needs at least one included component");

		TTuple<TComponentStorage<TInclude>*...> Storages(Registry.GetStorage<TInclude>()...);
		TTuple<TComponentStorage<TExclude>*...> Excludes(Registry.GetStorage<TExclude>()...);
		
		const bool bAnyComponentMissing = Storages.ApplyAfter([](auto*... Storage)
		{
			return ((Storage == nullptr) || ...);
		});
		if (bAnyComponentMissing) return;
		
		const TArray<FEntity>* Driver = nullptr;
		Storages.ApplyAfter([&](auto*... Storage)
		{
			((Driver = (!Driver || Storage->Num() < Driver->Num()) ?
				&Storage->Entities() : Driver),
			...);
		});

		for (int32 i = Driver->Num() - 1; i >= 0; --i)
		{
			// In case of removal while iterating
			if (i >= Driver->Num()) continue;
				
			const FEntity Entity = (*Driver)[i];
			
			Storages.ApplyAfter([&](auto*... Storage)
			{
				if ((Storage->Has(Entity) && ...))
				{
					const bool bExcluded = Excludes.ApplyAfter([&](auto*... Ex)
					{
						return ((Ex != nullptr && Ex->Has(Entity)) || ...);
					});

					if (!bExcluded)
					{
						Func(Entity, *Storage->Get(Entity)...);
					}
				}
			});
		}
	}

	template <component... TInclude, typename Fn>
	void ForEach(FRegistry& Registry, Fn&& Func)
	{
		ForEach<TInclude...>(Registry, TExcludeSet{}, Forward<Fn>(Func));
	}
}
