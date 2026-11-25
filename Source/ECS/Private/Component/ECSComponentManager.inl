#pragma once
#include "ECSComponentManager.h"
#include "ECSUtils.h"
#include "Storage/ECSComponentStorage.h"

namespace ECS
{
	template <component T>
	T& FComponentManager::AddToStorage(FEntity Entity, T Component)
	{
		auto& Storage = static_cast<TComponentStorage<T>&>(
			GetOrCreateStorage(KeyOf<T>(), &TComponentStorage<T>::Make));
		return Storage.Add(Entity, MoveTemp(Component));
	}

	template <component T>
	T* FComponentManager::Get(FEntity Entity)
	{
		IStorage* Storage = GetStorage(KeyOf<T>());
		return Storage
			? static_cast<TComponentStorage<T>*>(Storage)->Get(Entity)
			: nullptr;
	}

	template <component T>
	bool FComponentManager::Has(FEntity Entity) const
	{
		auto Storage = static_cast<const TComponentStorage<T>*>(
			GetStorage(KeyOf<T>())
		);
		return Storage && Storage->Has(Entity);
	}
}
