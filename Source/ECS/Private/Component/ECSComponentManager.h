#pragma once

#include "ECSUtils.h"
#include "ECSStorage.h"

namespace ECS
{
	class FComponentManager
	{
	public:
		template <component T>
		T& Add(FEntity Entity, T Comp = T());
		
		template <component T>
		void Remove(FEntity Entity);

		template <component T>
		bool Has(FEntity Entity) const;

		template <component T>
		T* Get(FEntity Entity);

		template <component T>
		const TComponentStorage<T>* GetStorage() const;

		void SignalEntityDestroyed(FEntity Entity);

	private:
		TArray<TUniquePtr<IStorage>> Storages;

		template <component T>
		TComponentStorage<T>* FindStorage() const;
	};

	template <component T>
	T& FComponentManager::Add(FEntity Entity, T Comp)
	{
		const uint32 Id = TypeId<T>();
		
		if (Storages.Num() <= static_cast<int32>(Id))
		{
			Storages.SetNum(static_cast<int32>(Id) + 1);
		}

		if (!Storages[Id])
		{
			Storages[Id] = MakeUnique<TComponentStorage<T>>();
		}
		
		auto& Storage = static_cast<TComponentStorage<T>&>(*Storages[Id]);
		return Storage.Add(Entity, Comp);
	}

	template <component T>
	void FComponentManager::Remove(FEntity Entity)
	{
		if (auto* Storage = FindStorage<T>())
		{
			Storage->Remove(Entity);
		}
	}

	template <component T>
	T* FComponentManager::Get(FEntity Entity)
	{
		if (auto* Storage = FindStorage<T>())
		{
			return Storage->Get(Entity);
		}
		return nullptr;
	}

	template <component T>
	const TComponentStorage<T>* FComponentManager::GetStorage() const
	{
		return FindStorage<T>();
	}

	template <component T>
	TComponentStorage<T>* FComponentManager::FindStorage() const
	{
		const uint32 Id = TypeId<T>();
		return Storages.IsValidIndex(static_cast<int32>(Id)) && Storages[Id]
			? static_cast<TComponentStorage<T>*>(Storages[Id].Get())
			: nullptr;
	}

	template <component T>
	bool FComponentManager::Has(FEntity Entity) const
	{
		const TComponentStorage<T>* Storage = FindStorage<T>();
		return Storage && Storage->Has(Entity);
	}

	inline void FComponentManager::SignalEntityDestroyed(FEntity Entity)
	{
		for (const auto& Storage : Storages)
		{
			Storage->Remove(Entity);
		}
	}
}
