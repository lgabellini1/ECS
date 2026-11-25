#pragma once
#include "ECSRegistry.h"
#include "ECSUtils.h"

namespace ECS
{
	template <component T>
	T& FRegistry::Add(FEntity Entity, T Component)
	{
		check(IsAlive(Entity));
		TComponentStorage<T>& Storage = GetOrCreateStorage<T>();
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
	bool FRegistry::Has(FEntity Entity)
	{
		auto*  Storage = GetStorage<T>();
		return Storage ? Storage->Has(Entity) : false;
	}

	template <component T>
	T* FRegistry::Get(FEntity Entity)
	{
		auto*  Storage = GetStorage<T>();
		return Storage ? Storage->Get(Entity) : nullptr;
	}

	template <component T>
	T& FRegistry::GetChecked(FEntity Entity)
	{
		T* Component = Get<T>(Entity);
		check  (Component);
		return *Component;
	}
	
	template <component T>
	TComponentStorage<T>* FRegistry::GetStorage() const
	{
		return static_cast<TComponentStorage<T>*>(
			GetStorageInternal(KeyOf<T>())
		);
	}

	template <component T>
	TComponentStorage<T>& FRegistry::GetOrCreateStorage()
	{
		return static_cast<TComponentStorage<T>&>(
			GetOrCreateStorage(
				KeyOf<T>(),
				[]() { return TComponentStorage<T>::Make(); }
			)
		);
	}
}
