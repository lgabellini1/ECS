#pragma once
#include "ECSComponentManager.h"
#include "ECSUtils.h"
#include "Storage/ECSComponentStorage.h"

namespace ECS
{
	template <component T>
	T& FComponentManager::AddToStorage(FEntity Entity, T Component)
	{
		const uint32 Type = TypeId<T>();
		if (Storages.Num() <= static_cast<int32>(Type))
		{
			Storages.SetNum(static_cast<int32>(Type) + 1);
		}

		if (!Storages[Type])
		{
			Storages[Type] = MakeUnique<TComponentStorage<T>>();
		}

		auto& Storage = static_cast<TComponentStorage<T>&>(
			*Storages[Type].Get()
		);
		return Storage.Add(Entity, Component);
	}

	template <component T>
	T* FComponentManager::Get(FEntity Entity)
	{
		IStorage* Storage = GetStorage(TypeId<T>());
		return Storage
			? static_cast<TComponentStorage<T>*>(Storage)->Get(Entity)
			: nullptr;
	}

	template <component T>
	bool FComponentManager::Has(FEntity Entity)
	{
		auto Storage = static_cast<const TComponentStorage<T>*>(
			GetStorage(TypeId<T>())
		);
		return Storage && Storage->Has(Entity);
	}
}
