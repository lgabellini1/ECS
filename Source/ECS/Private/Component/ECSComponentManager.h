#pragma once
#include "ECSTypes.h"
#include "Storage/ECSComponentStorage.h"

namespace ECS
{
	class FComponentManager
	{
	public:
		FComponentManager() = default;

		FComponentManager(const FComponentManager&)			   = delete;
		FComponentManager& operator=(const FComponentManager&) = delete;
		
		template <component T>
		T& AddToStorage(FEntity, T Component = T());
		
		void RemoveFromStorage(FEntity, uint32 TypeId);
		void RemoveFromAllStorages(FEntity);

		template <component T>
		bool Has(FEntity) const;

		template <component T>
		T* Get(FEntity);

		IStorage* GetStorage(uint32 TypeId) const;
		IStorage& GetOrCreateStorage(uint32 TypeId, FStorageFactory);  
	private:
		// ptr-to-base-interface pattern required because the set of types is open-ended
		TArray<TUniquePtr<IStorage>> Storages;
	};
}

#include "ECSComponentManager.inl"
