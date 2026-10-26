#pragma once

namespace ECS
{
	constexpr uint32 EntityIndexBits = 22;
	constexpr uint32 EntityIndexMask = (1u << EntityIndexBits) - 1;
	
	constexpr int32 MaxIndices = EntityIndexMask + 1;

	constexpr uint32 EntityGenerationBits = 8;
	constexpr uint32 EntityGenerationMask = (1u << EntityGenerationBits) - 1;

	constexpr uint32 InvalidEntityId = 0xFFFFFFFFu;
}
