#pragma once

namespace ECS
{
	// Custom typeid to use without RTTI (disabled in Unreal).
	
	// These are NOT stable across boundaries, meaning that, in any case
	// one should not expect a type to have the same id inside and outside
	// the plugin. They work fine for internal use only though.

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
