#pragma once
#include "ECSRegistry.h"
#include "ECSUtils.h"

namespace ECS
{
	template <component T>
	T& FRegistry::Add(FEntity Entity, T Component)
	{
		check (IsAlive(Entity));
		auto& Storage = static_cast<TComponentStorage<T>&>(
			GetOrCreateStorageInternal(TypeId<T>(), &TComponentStorage<T>::Make)
		);
		return Storage.Add(Entity, MoveTemp(Component));
	}
	
	template <component T>
	void FRegistry::Remove(FEntity Entity)
	{
		if (IsAlive(Entity))
		{
			if (auto* Storage = GetStorage<T>())
			{
				Storage->Remove(Entity);
			}
		}
	}

	template <component T>
	T* FRegistry::Get(FEntity Entity)
	{
		auto*  Storage = GetStorage<T>();
		return Storage ? Storage->Get(Entity) : nullptr;
	}
	
	template <component T>
	TComponentStorage<T>* FRegistry::GetStorage() const
	{
		return static_cast<TComponentStorage<T>*>(
			GetStorageInternal(TypeId<T>())
		);
	}
}
