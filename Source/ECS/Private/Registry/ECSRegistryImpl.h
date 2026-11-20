#pragma once
#include "Component/ECSComponentManager.h"
#include "Entity/ECSEntityManager.h"

namespace ECS
{
	class IStorage;
	
	class FRegistryImpl final
	{
	public:
		FRegistryImpl() = default;

		FRegistryImpl(const FRegistryImpl&)			   = delete;
		FRegistryImpl& operator=(const FRegistryImpl&) = delete;
		
		FEntity Create();
		void    Destroy(FEntity);
		bool	IsAlive(FEntity) const;

		template <component T>
		T* Add(FEntity, T Component = T());

		template <component T>
		void Remove(FEntity);

		void Remove(FEntity, uint32 TypeId);

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
