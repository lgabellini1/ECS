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

	int32 FRegistry::NumEntities() const
	{
		check (Impl.IsValid());
		return Impl->NumEntities();
	}

	IStorage* FRegistry::GetStorageInternal(FComponentKey Key) const
	{
		check (Impl.IsValid());
		return Impl->GetStorage(Key);
	}

	IStorage& FRegistry::GetOrCreateStorage(FComponentKey Key, FStorageFactory Factory)
	{
		check (Impl.IsValid());
		return Impl->GetOrCreateStorage(Key, Factory);
	}

	void FRegistry::ForEachStorage(TFunctionRef<void(const FComponentKey&, const IStorage&)> Func) const
	{
		check (Impl.IsValid());
		return Impl->ForEachStorage(Func);
	}
}
