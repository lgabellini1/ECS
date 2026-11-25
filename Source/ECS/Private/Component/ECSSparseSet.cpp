#include "Storage/ECSSparseSet.h"

namespace ECS
{
	uint32 FSparseSet::Insert(FEntity Entity, bool& bIsNew)
	{
		const uint32 i = Entity.Index();
		if (Has(Entity))
		{
			bIsNew = false;
			return Sparse[i].Index();
		}

		bIsNew = true;
		const uint32 Slot = Dense.Num();
			
		while (static_cast<uint32>(Sparse.Num()) <= i)
		{
			Sparse.Add(FEntity::Null);
		}

		Sparse[i] = FEntity(Slot, Entity.Generation());
		Dense.Add(Entity);
		return Slot;
	}

	int32 FSparseSet::Erase(FEntity Entity)
	{
		if (!Has(Entity))
			return INDEX_NONE;
		
		const uint32 i = Entity.Index();
		
		const uint32 Slot  = Sparse[i].Index();
		const FEntity Last = Dense.Last();

		Dense.RemoveAtSwap(Slot);
		Sparse[Last.Index()] = FEntity(Slot, Last.Generation());
		Sparse[i] = FEntity::Null;
		
		return Slot;
	}
	
	bool FSparseSet::Has(FEntity Entity) const
	{
		const uint32 i = Entity.Index();

		if (i >= static_cast<uint32>(Sparse.Num()))
			return false;

		const FEntity Slot = Sparse[i];
		return !Slot.IsNull() && Slot.Generation() == Entity.Generation();
	}

	int32 FSparseSet::SlotOf(FEntity Entity) const
	{
		const uint32 i = Entity.Index();
		return Has(Entity) ? Sparse[i].Index() : INDEX_NONE;
	}
}
