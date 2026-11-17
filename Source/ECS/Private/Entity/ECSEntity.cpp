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
	#if WITH_EDITOR
		check(Index < EntityIndexMask);
	#endif
	}
	
	uint32 FEntity::Index() const
	{
		return Id & EntityIndexMask;
	}

	uint32 FEntity::Generation() const
	{
		return (Id >> EntityIndexBits) & EntityGenerationMask;
	}

	constexpr FEntity FEntity::Null = FEntity(NullEntityId);

	bool FEntity::IsNull() const
	{
		return Index() == NullEntityIndex;	
	}
}
