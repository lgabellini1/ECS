#pragma once

namespace ECS
{
	template <typename T>
	concept component = std::is_trivially_copyable_v<T>
		&& std::default_initializable<T>
		&& (!std::is_const_v<T>) && (!std::is_reference_v<T>);
	
	ECS_API uint32 NextTypeId();
	
	template <component T> // templating allows each type to have its own static 
	uint32 TypeId()
	{
		static const uint32 Id = NextTypeId();
		return Id;
	}
}
