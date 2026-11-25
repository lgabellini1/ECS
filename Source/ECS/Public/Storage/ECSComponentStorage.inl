#pragma once
#include "ECSComponentStorage.h"
#include "ECSTypes.h"

namespace ECS
{
	template <component T>
	T& TComponentStorage<T>::Add(FEntity Entity, T Component)
	{
		return *static_cast<T*>(AddRaw(Entity, &Component));
	}

	template <component T>
	T* TComponentStorage<T>::Get(FEntity Entity)
	{
		return static_cast<T*>(GetRaw(Entity));
	}
}
