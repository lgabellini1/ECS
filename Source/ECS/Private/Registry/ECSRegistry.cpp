#include "Registry/ECSRegistry.h"
#include "ECSRegistryImpl.h"

namespace ECS
{
	FRegistry::FRegistry()
		: Impl(MakePimpl<FRegistryImpl>())
	{}
	
	FRegistry::FRegistry(TPimplPtr<FRegistryImpl>&& InImpl)
		: Impl(MoveTemp(InImpl))
	{}
	
	FEntity FRegistry::Create()
	{
		check(Impl);
		return Impl->Create();
	}

	void FRegistry::Destroy(FEntity Entity)
	{
		check(Impl);
		Impl->Destroy(Entity);
	}

	bool FRegistry::IsAlive(FEntity Entity) const
	{
		check(Impl);
		return Impl->IsAlive(Entity);
	}

	void FRegistry::RemoveInternal(FEntity Entity, uint32 TypeId)
	{
		check(Impl);
		Impl->Remove(Entity, TypeId);
	}

	IStorage* FRegistry::GetStorageInternal(uint32 TypeId) const
	{
		check(Impl);
		return Impl->GetStorage(TypeId);
	}
}
