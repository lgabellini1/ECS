#pragma once
#include "ECSTypes.h"
#include "ECSStorageInterface.h"
#include "ECSStorageRaw.h"

namespace ECS
{
	using FStorageFactory = TFunctionRef<TUniquePtr<IStorage>()>;
	
	template <component T>
	class TComponentStorage final : public FTypeErasedStorage
	{
	public:
		TComponentStorage() : FTypeErasedStorage(MakeOps<T>())
		{}
		
		T& Add(FEntity, T Component = T());
		T* Get(FEntity);

		static TUniquePtr<IStorage> Make() { return MakeUnique<TComponentStorage>(); }
	};
}

#include "ECSComponentStorage.inl"
