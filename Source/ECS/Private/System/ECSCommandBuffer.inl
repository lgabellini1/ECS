#pragma once

#include "ECSCommandBuffer.h"

namespace ECS
{
	template <component T>
	void FCommandBuffer::Add(FEntity Entity, T Comp)
	{
		Commands.Add([Entity, Component = MoveTemp(Comp)](FRegistry& Registry) mutable
		{
			Registry.Add<T>(Entity, Component);
		});
	}

	template <component T>
	void FCommandBuffer::Remove(FEntity Entity)
	{
		Commands.Add([Entity](FRegistry& Registry) mutable
		{
			Registry.Remove<T>(Entity);
		});
	}

	inline void FCommandBuffer::Destroy(FEntity Entity)
	{
		Commands.Add([Entity](FRegistry& Registry)
		{
			Registry.DestroyEntity(Entity);
		});
	}

	inline void FCommandBuffer::Flush(FRegistry& Registry)
	{
		for (const auto& Cmd : Commands)
		{
			Cmd(Registry);
		}
		
		Commands.Reset();
	} 	
}
