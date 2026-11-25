#include "Storage/ECSStorageRaw.h"

namespace ECS
{
	FTypeErasedStorage::FTypeErasedStorage(const FComponentOps& InOps)
	: Ops(InOps)
	{
		check(Ops.Align <= 16);
	}
	
	FTypeErasedStorage::~FTypeErasedStorage()
	{
		for (int32 i = 0; i < Set.Num(); ++i)
		{
			Ops.Destroy(Ops.Ctx, SlotInternal(i));
		}
	}
	
	void* FTypeErasedStorage::AddRaw(FEntity Entity, const void* ComponentBytes)
	{
		bool bNew;
		const uint32 Slot = Set.Insert(Entity, bNew);
		if (bNew)
		{
			Bytes.AddUninitialized(Ops.Size);
			Ops.Construct(Ops.Ctx, SlotInternal(Slot));
		}
		
		Ops.Assign(Ops.Ctx, SlotInternal(Slot), ComponentBytes);
		return SlotInternal(Slot);
	}

	void FTypeErasedStorage::Remove(FEntity Entity)
	{
		if (const int32 Slot = Set.Erase(Entity);
			Slot != INDEX_NONE)
		{
			const int32 Last = Set.Num();
			if (Slot != Last)
				Ops.Assign(Ops.Ctx, SlotInternal(Slot), SlotInternal(Last));
			
			Ops.Destroy(Ops.Ctx, SlotInternal(Last));
			Bytes.SetNum(Bytes.Num() - Ops.Size, EAllowShrinking::No);	
		}
	}

	void* FTypeErasedStorage::GetRaw(FEntity Entity)
	{
		const int32 Slot = Set.SlotOf(Entity);
		return Slot != INDEX_NONE
			? SlotInternal(Slot)
			: nullptr;
	}

	bool FTypeErasedStorage::Has(FEntity Entity) const
	{
		return Set.Has(Entity);
	}

	uint8* FTypeErasedStorage::SlotInternal(int32 Index)
	{
		return Bytes.GetData() + static_cast<SIZE_T>(Index) * Ops.Size;
	}
}
