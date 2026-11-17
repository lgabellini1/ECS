#pragma once

namespace ECS
{
	// Custom typeid to use without RTTI (disabled in Unreal).
	// These are NOT stable across boundaries!

	namespace Private
	{
		inline uint32 NextId()
		{
			static uint32 Counter = 0;
			return Counter++;
		}
	}

	template <typename T>
	uint32 TypeId()
	{
		static const uint32 Id = Private::NextId();
		return Id;
	}
}
