#pragma once

#include "ECSTypes.h"

namespace ECS
{
	// 'Generations' is the entities' source-of-truth. 
	// A[i] = gen fully encodes an entity id (i+gen).
	
	class FEntityManager
	{
	public:
		[[nodiscard]] FEntity Create();
		void Destroy(FEntity Entity);

		bool ECS_API IsAlive(FEntity Entity) const;

		int32 GetLivingCount() const { return LivingCount; }
		
	private:
		TArray<uint8> Generations;
		TQueue<int32> FreeIndices;

		int32 LivingCount = 0;

		bool IsStale(FEntity Entity) const;
	};
}
