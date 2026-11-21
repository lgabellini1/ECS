#include "ECSTypes.h"
#include "ECSEntityConsts.h"

namespace ECS
{
	FEntity::FEntity() : FEntity(Null)
	{}

	constexpr FEntity::FEntity(uint32 InId) : Id(InId)
	{}

	FEntity::FEntity(uint32 Index, uint32 Generation) : Id(
		(Generation & EntityGenerationMask) << EntityIndexBits | (Index & EntityIndexMask))
	{
		check(Index <= EntityIndexMask);
	}
	
	uint32 FEntity::Index() const
	{
		return Id & EntityIndexMask;
	}

	uint32 FEntity::Generation() const
	{
		return (Id >> EntityIndexBits) & EntityGenerationMask;
	}

	const FEntity FEntity::Null	= FEntity(NullEntityIndex, 0);

	bool FEntity::IsNull() const
	{
		return Index() == NullEntityIndex;	
	}
}
