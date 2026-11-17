#include "ECSRegistryImpl.h"

namespace ECS
{
	FEntity FRegistryImpl::Create()
	{
		return Entities.Create();
	}

	void FRegistryImpl::Destroy(FEntity Entity)
	{
		Entities.Destroy(Entity);
		Components.RemoveFromAllStorages(Entity);
	}
	
	IStorage* FRegistryImpl::GetStorage(uint32 TypeId) const
	{
		return Components.GetStorage(TypeId);
	}
}
