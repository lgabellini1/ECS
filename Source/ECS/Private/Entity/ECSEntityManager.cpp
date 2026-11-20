#include "ECSEntityManager.h"

namespace ECS
{
	FEntity FEntityManager::Create()
	{
		if (Available > 0)
		{
			const uint32 i = Next.Index();
			
			Next = Entities[i];
			--Available;
			
			const FEntity Recycled = FEntity(i, Entities[i].Generation());
			Entities[i] = Recycled;
			return Recycled;
		}
		else
		{
			const FEntity New(static_cast<uint32>(Entities.Num()), 0);
			Entities.Add(New);
			return New;
		}
	}

	void FEntityManager::Destroy(FEntity Entity)
	{
		if (IsAlive(Entity))
		{
			const uint32 i = Entity.Index();
			
			Entities[i] = FEntity(Next.Index(), Entity.Generation() + 1);
			Next = Entity;
			++Available;
		}
	}

	bool FEntityManager::IsAlive(FEntity Entity) const
	{
		const uint32 i = Entity.Index();
		return i < static_cast<uint32>(Entities.Num())
			&& Entities[i] == Entity
			&& !Entity.IsNull();
	}
}
