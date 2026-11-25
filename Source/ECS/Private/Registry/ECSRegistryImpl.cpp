#include "ECSRegistryImpl.h"

namespace ECS
{
	FEntity FRegistryImpl::Create()
	{
		return Entities.Create();
	}

	void FRegistryImpl::Destroy(FEntity Entity)
	{
		if (IsAlive(Entity))
		{
			Entities.Destroy(Entity);
			Components.RemoveFromAllStorages(Entity);	
		}
	}

	bool FRegistryImpl::IsAlive(FEntity Entity) const
	{
		return Entities.IsAlive(Entity);
	}

	void FRegistryImpl::Remove(FEntity Entity, FComponentKey Key)
	{
		if (IsAlive(Entity))
		{
			Components.RemoveFromStorage(Entity, Key);
		}
	}
	
	IStorage* FRegistryImpl::GetStorage(FComponentKey Key) const
	{
		return Components.GetStorage(Key);
	}

	IStorage& FRegistryImpl::GetOrCreateStorage(FComponentKey Key, FStorageFactory Factory)
	{
		return Components.GetOrCreateStorage(Key, Factory);
	}
}
