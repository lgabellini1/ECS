#include "ECSComponentManager.h"

namespace ECS
{
	void FComponentManager::RemoveFromAllStorages(FEntity Entity)
	{
		for (const auto& Storage : Storages)
		{
			Storage->Remove(Entity);
		}
	}
	
	IStorage* FComponentManager::GetStorage(uint32 TypeId) const
	{
		return Storages.IsValidIndex(TypeId) && Storages[TypeId]
			? Storages[TypeId].Get()
			: nullptr;
	}
}
