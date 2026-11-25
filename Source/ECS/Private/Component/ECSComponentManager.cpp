#include "ECSComponentManager.h"

namespace ECS
{
	void FComponentManager::RemoveFromStorage(FEntity Entity, FComponentKey Key)
	{
		if (auto* Storage = GetStorage(Key))
		{
			Storage->Remove(Entity);
		}
	}
	
	void FComponentManager::RemoveFromAllStorages(FEntity Entity)
	{
		for (const auto& Storage : Storages)
		{
			if (Storage.Get())
			{
				Storage->Remove(Entity);
			}
		}
	}
	
	IStorage* FComponentManager::GetStorage(FComponentKey Key) const
	{
		const int32* Index = KeyToIndex.Find(Key);
		return Index ? Storages[*Index].Get() : nullptr;
	}

	IStorage& FComponentManager::GetOrCreateStorage(FComponentKey Key, FStorageFactory Factory)
	{
		if (const int32* Index = KeyToIndex.Find(Key))
		{
			return *Storages[*Index];
		}

		const int32 NewIndex = Storages.Add(Factory());
		KeyToIndex.Add(Key, NewIndex);
		return *Storages[NewIndex];
	}

	void FComponentManager::ForEachStorage(TFunctionRef<void(const FComponentKey&, const IStorage&)> Func) const
	{
		for (const TPair<FComponentKey, int32>& Pair : KeyToIndex)
		{
			Func(Pair.Key, *Storages[Pair.Value]);
		}
	}
}
