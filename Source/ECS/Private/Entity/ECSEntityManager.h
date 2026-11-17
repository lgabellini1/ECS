#pragma once
#include "ECSTypes.h"

namespace ECS
{
	class FEntityManager final
	{
	public:
		[[nodiscard]] FEntity Create();
		void Destroy(FEntity);

		bool  IsAlive(FEntity) const;
		int32 GetLivingCount() const { return Entities.Num() - Available; }
	private:
		TArray<FEntity> Entities;
		
		int32	Available = 0;
		// Head of the implicit destroyed entities list
		FEntity Next;
	};
}
