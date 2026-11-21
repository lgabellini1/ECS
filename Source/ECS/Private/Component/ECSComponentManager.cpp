#include "ECSComponentManager.h"

namespace ECS
{
	void FComponentManager::RemoveFromStorage(FEntity Entity, uint32 TypeId)
	{
		if (auto* Storage = GetStorage(TypeId))
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
	
	IStorage* FComponentManager::GetStorage(uint32 TypeId) const
	{
		return Storages.IsValidIndex(TypeId) && Storages[TypeId]
			? Storages[TypeId].Get()
			: nullptr;
	}

	IStorage& FComponentManager::GetOrCreateStorage(uint32 TypeId, FStorageFactory Factory)
	{
		if (Storages.Num() <= static_cast<int32>(TypeId))
		{
			Storages.SetNum(static_cast<int32>(TypeId) + 1);
		}
		TUniquePtr<IStorage>& Slot = Storages[TypeId];
		if (!Slot)
		{
			Slot = Factory();
			check(Slot.IsValid());
		}
		return *Slot;
	}
}
