#pragma once
#include "ECSTypes.h"

namespace ECS
{
	class FComponentManager
	{
	public:
		template <component T>
		T& AddToStorage(FEntity, T Component = T());
		
		template <component T>
		void RemoveFromStorage(FEntity);

		void RemoveFromAllStorages(FEntity);

		template <component T>
		bool Has(FEntity);

		template <component T>
		T* Get(FEntity);
		
		IStorage* GetStorage(uint32 TypeId) const;
	private:
		// ptr-to-base-interface pattern required because the set of types is open-ended
		TArray<TUniquePtr<IStorage>> Storages;
	};
}

#include "ECSComponentManager.inl"
