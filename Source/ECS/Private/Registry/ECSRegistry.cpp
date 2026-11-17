#include "Registry/ECSRegistry.h"
#include "ECSRegistryImpl.h"

namespace ECS
{
	FRegistry::FRegistry(TPimplPtr<FRegistryImpl>&& InImpl)
		: Impl(MoveTemp(InImpl))
	{}
	
	FEntity FRegistry::Create()
	{
		return Impl->Create();
	}

	void FRegistry::Destroy(FEntity Entity)
	{
		Impl->Destroy(Entity);
	}

	IStorage* FRegistry::GetStorageInternal(uint32 TypeId) const
	{
		return Impl->GetStorage(TypeId);
	}
}
