#include "Registry/ECSRegistry.h"
#include "ECSRegistryImpl.h"

namespace ECS
{
	FRegistry::FRegistry()
		: FRegistry(MakePimpl<FRegistryImpl>())
	{}
	
	FRegistry::FRegistry(TPimplPtr<FRegistryImpl>&& InImpl)
		: Impl(MoveTemp(InImpl))
	{
		check(Impl.IsValid());
	}
	
	FEntity FRegistry::Create()
	{
		check (Impl.IsValid());
		return Impl->Create();
	}

	void FRegistry::Destroy(FEntity Entity)
	{
		check(Impl.IsValid());
		Impl->Destroy(Entity);
	}

	bool FRegistry::IsAlive(FEntity Entity) const
	{
		check (Impl.IsValid());
		return Impl->IsAlive(Entity);
	}

	IStorage* FRegistry::GetStorageInternal(uint32 TypeId) const
	{
		check (Impl.IsValid());
		return Impl->GetStorage(TypeId);
	}

	IStorage& FRegistry::GetOrCreateStorageInternal(uint32 TypeId, FStorageFactory Factory)
	{
		return Impl->GetOrCreateStorage(TypeId, Factory);
	}
}
