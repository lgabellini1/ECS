#include "ECSTypes.h"
#include "ECSEntityInternal.h"

namespace ECS
{
	FEntity::FEntity(uint32 Id) : Id(Id)
	{}

	FEntity::FEntity(uint32 Index, uint32 Generation) : Id(
		(Generation & EntityGenerationMask) << EntityIndexBits | (Index & EntityIndexMask))
	{
		check(Index < EntityIndexMask);
	}

	uint32 FEntity::Index() const
	{
		return Id & EntityIndexMask;
	}

	uint32 FEntity::Generation() const
	{
		return (Id >> EntityIndexBits) & EntityGenerationMask;
	}

	bool FEntity::IsNull() const
	{
		return Id == InvalidEntityId;
	}

	static_assert(EntityIndexBits + EntityGenerationBits <= 31,
		"Entities need at least one spare top bit to represent sentinel value");

	static_assert(EntityGenerationBits <= 8);
	
	static_assert(sizeof(FEntity) == sizeof(uint32));

	static_assert(std::is_trivially_copyable_v<FEntity>);
}
