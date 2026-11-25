#pragma once
#include "ECSTypes.h"

namespace ECS
{
	class FSparseSet
	{
	public:
		uint32 Insert(FEntity Entity, bool& bIsNew);
		int32  Erase(FEntity);
		
		bool  Has(FEntity) const;
		int32 SlotOf(FEntity) const;

		int32 Num() const { return Dense.Num(); }
		const TArray<FEntity>& Entities() const { return Dense; }

		SIZE_T Size() const { return Sparse.GetAllocatedSize() + Dense.GetAllocatedSize(); }
	private:
		TArray<FEntity> Sparse;
		TArray<FEntity> Dense;
	};	
}
