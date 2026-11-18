#pragma once
#include "Component/ECSComponentManager.h"
#include "Entity/ECSEntityManager.h"

namespace ECS
{
	class IStorage;
	
	class FRegistryImpl final
	{
	public:
		FEntity Create();
		void    Destroy(FEntity);

		template <component T>
		T* Add(FEntity, T Component = T());

		template <component T>
		void Remove(FEntity);

		template <component T>
		bool Has(FEntity);

		template <component T>
		T* Get(FEntity);
		
		IStorage* GetStorage(uint32 TypeId) const;
	private:
		FEntityManager    Entities;
		FComponentManager Components;
	};
}

#include "ECSRegistryImpl.inl"
