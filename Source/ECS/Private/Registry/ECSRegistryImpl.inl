#pragma once
#include "ECSRegistryImpl.h"

namespace ECS
{
	template <component T>
	T* FRegistryImpl::Add(FEntity Entity, T Component)
	{
		return Entities.IsAlive(Entity)
			? &Components.AddToStorage(Entity, MoveTemp(Component))
			: nullptr;
	}

	template <component T>
	void FRegistryImpl::Remove(FEntity Entity)
	{
		if (Entities.IsAlive(Entity))
		{
			Components.RemoveFromStorage<T>(Entity);
		}
	}

	template <component T>
	bool FRegistryImpl::Has(FEntity Entity)
	{
		return Entities.IsAlive(Entity) && Components.Has<T>(Entity);
	}

	template <component T>
	T* FRegistryImpl::Get(FEntity Entity)
	{
		return Components.Get<T>(Entity);
	}
}
