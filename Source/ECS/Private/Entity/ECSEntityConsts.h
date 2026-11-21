#pragma once

namespace ECS
{
	constexpr uint32 EntityIndexBits = 20;
	constexpr uint32 EntityIndexMask = (1u << EntityIndexBits) - 1;

	constexpr uint32 EntityGenerationBits = 12;
	constexpr uint32 EntityGenerationMask = (1u << EntityGenerationBits) - 1;

	constexpr uint32 NullEntityIndex = 0xFFFFFu;
	constexpr uint32 TombstoneEntityGeneration = 0xFFFu;
}
