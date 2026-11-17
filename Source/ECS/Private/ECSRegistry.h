#pragma once

#include "Entity/ECSEntityManager.h"
#include "Component/ECSComponentManager.h"
#include "System/ECSCommandBuffer.h"

namespace ECS
{
	class ECS_API FRegistry
	{
	public:
		FRegistry() = default;
		// Non-copyable: it owns the TUniquePtrs from the managers
		FRegistry(const FRegistry&) = delete;
		FRegistry& operator=(const FRegistry&) = delete;

		FEntity CreateEntity();
		void DestroyEntity(FEntity Entity);

		template <component T>
		T* Add(FEntity Entity, T Comp = T());

		template <component T>
		void Remove(FEntity Entity);		

		template <component T>
		bool Has(FEntity Entity) const;

		template <component T>
		T* Get(FEntity Entity);

		bool IsAlive(FEntity Entity) const;

		int32 NumEntities() const { return Entities.GetLivingCount(); }

		template <component T>
		const TComponentStorage<T>* GetStorage() const;

		FCommandBuffer& Commands() { return CommandBuffer; }
		void FlushCmds();

	private:
		FEntityManager Entities;
		FComponentManager Components;
		FCommandBuffer CommandBuffer;
	};

	template <component T>
	T* FRegistry::Add(FEntity Entity, T Comp)
	{
		if (Entities.IsAlive(Entity))
		{
			return &Components.Add<T>(Entity, MoveTemp(Comp));
		}

		return nullptr;
	}

	template <component T>
	bool FRegistry::Has(FEntity Entity) const
	{
		return Entities.IsAlive(Entity) && Components.Has<T>(Entity);
	}

	template <component T>
	T* FRegistry::Get(FEntity Entity)
	{
		return Entities.IsAlive(Entity) ? Components.Get<T>(Entity) : nullptr;
	}
	
	template <component T>
	const TComponentStorage<T>* FRegistry::GetStorage() const
	{
		return Components.GetStorage<T>();
	}

	template <component T>
	void FRegistry::Remove(FEntity Entity)
	{
		if (Entities.IsAlive(Entity))
		{
			Components.Remove<T>(Entity);
		}
	}
}

#include "System/ECSCommandBuffer.inl"
