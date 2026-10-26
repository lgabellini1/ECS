#include "ECSRegistry.h"

ECS::FEntity ECS::FRegistry::CreateEntity()
{
	return Entities.Create();
}

void ECS::FRegistry::DestroyEntity(ECS::FEntity Entity)
{
	if (Entities.IsAlive(Entity))
	{
		Components.SignalEntityDestroyed(Entity);
		Entities.Destroy(Entity);
	}
}

bool ECS::FRegistry::IsAlive(ECS::FEntity Entity) const
{
	return Entities.IsAlive(Entity);
}

void ECS::FRegistry::FlushCmds()
{
	CommandBuffer.Flush(*this);
}
