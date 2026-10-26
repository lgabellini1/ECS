#pragma once

#include "ECS/Private/Entity/ECSEntityInternal.h"

namespace ECS
{
	class FRegistry;
	
	struct ECS_API FEntity
	{
		FEntity(uint32 Id = InvalidEntityId);
		FEntity(uint32 Index, uint32 Generation);

		operator uint32() const { return Id; }

		uint32 Index() const;
		uint32 Generation() const;

		bool IsNull() const;
		
	private:
		uint32 Id;
	};
	
	struct ECS_API FTransform3D
	{
		FVector Location = FVector::ZeroVector;
	};

	class ECS_API ISystem
	{
	public:
		virtual ~ISystem() = default;
		virtual void Update(FRegistry& Registry, float DeltaTime) = 0;		
	};
}
