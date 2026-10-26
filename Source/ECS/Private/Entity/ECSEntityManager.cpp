#include "ECSEntityManager.h"

ECS::FEntity ECS::FEntityManager::Create()
{
	int32 Index = INDEX_NONE;
	if (!FreeIndices.Dequeue(Index))
	{
		Index = Generations.Add(0);
	}
	check(Index != INDEX_NONE);
	check(Generations.Num() <= MaxIndices);

	++LivingCount;
	return FEntity(static_cast<uint32>(Index), Generations[Index]);
}

void ECS::FEntityManager::Destroy(FEntity Entity)
{
	if (IsAlive(Entity))
	{
		++Generations[Entity.Index()];
		FreeIndices.Enqueue(Entity.Index());
		
		--LivingCount;
	}
}

bool ECS::FEntityManager::IsStale(FEntity Entity) const
{
	check(Generations.IsValidIndex(Entity.Index()));
	
	return Entity.Generation() != Generations[Entity.Index()];
}

bool ECS::FEntityManager::IsAlive(FEntity Entity) const
{
	return !Entity.IsNull()
		&& Generations.IsValidIndex(Entity.Index())
		&& !IsStale(Entity);
}
