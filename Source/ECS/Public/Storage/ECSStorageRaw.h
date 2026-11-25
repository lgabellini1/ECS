#pragma once
#include "ECSStorageInterface.h"
#include "ECSComponentOps.h"

namespace ECS
{
	class ECS_API FTypeErasedStorage : public IStorage
	{
	public:
		explicit FTypeErasedStorage(const FComponentOps& InOps);
		virtual ~FTypeErasedStorage() override;
		
		void* AddRaw(FEntity, const void* ComponentBytes);
		virtual void Remove(FEntity) override;
		
		void* GetRaw(FEntity);
		bool  Has(FEntity) const;

		virtual int32 Num() const override { return Set.Num(); }
		const TArray<FEntity>& Entities() const { return Set.Entities(); }

		virtual SIZE_T Size() const override { return Set.Size() + Bytes.GetAllocatedSize(); }
	private:
		uint8* SlotInternal(int32 Index);

		FComponentOps Ops;
		FSparseSet    Set;
		// Component memory payload 
		TArray<uint8, TAlignedHeapAllocator<16>> Bytes;
	};
}
