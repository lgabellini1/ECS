#pragma once
#include "ECSComponentStorage.h"
#include "ECSTypes.h"

namespace ECS
{
	template <component T>
	T& TComponentStorage<T>::Add(FEntity Entity, T Component)
	{
		const uint32 i = Entity.Index();
		if (Has(Entity))
		{
			return Components[Sparse[i].Index()] = MoveTemp(Component);
		}
		
		// safe casts because Num() is always positive
		const uint32 Slot = static_cast<uint32>(Dense.Num());
		while (static_cast<uint32>(Sparse.Num()) <= i)
		{
			Sparse.Add(FEntity::Null);
		}

		Sparse[i] = FEntity(Slot, Entity.Generation());
		
		Dense.Add(Entity);
		T& NewComp = Components.Add_GetRef(MoveTemp(Component));
		check(Dense.Num() == Components.Num());
		
		return NewComp;
	}

	template <component T>
	void TComponentStorage<T>::Remove(FEntity Entity)
	{
		if (Has(Entity))
		{
			const uint32 i = Entity.Index();
			
			const uint32 Slot  = Sparse[i].Index();
			const FEntity Last = Dense.Last();
			
			Dense.RemoveAtSwap(Slot);
			Components.RemoveAtSwap(Slot);
			
			Sparse[Last.Index()] = FEntity(Slot, Last.Generation());
			Sparse[i] = FEntity::Null;

			check(Dense.Num() == Components.Num());
		}
	}
	
	template <component T>
	bool TComponentStorage<T>::Has(FEntity Entity) const
	{
		if (Entity.IsNull()) return false;
		
		const uint32 i = Entity.Index();
		if (i >= static_cast<uint32>(Sparse.Num()))
		{
			return false;
		}

		const FEntity Slot = Sparse[i];
		return !Slot.IsNull() && Slot.Generation() == Entity.Generation();
	}

	template <component T>
	T* TComponentStorage<T>::Get(FEntity Entity)
	{
		const uint32 i = Entity.Index();
		return Has(Entity)
			? &Components[Sparse[i].Index()]
			: nullptr;
	}
}
