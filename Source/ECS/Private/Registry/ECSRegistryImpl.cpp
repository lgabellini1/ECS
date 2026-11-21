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

	void FRegistryImpl::Remove(FEntity Entity, uint32 TypeId)
	{
		if (IsAlive(Entity))
		{
			Components.RemoveFromStorage(Entity, TypeId);
		}
	}
	
	IStorage* FRegistryImpl::GetStorage(uint32 TypeId) const
	{
		return Components.GetStorage(TypeId);
	}

	IStorage& FRegistryImpl::GetOrCreateStorage(uint32 TypeId, FStorageFactory Factory)
	{
		return Components.GetOrCreateStorage(TypeId, Factory);
	}
}
