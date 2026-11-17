#pragma once
#include "ECSRegistry.h"
#include "ECSUtils.h"

namespace ECS
{
	template <component T>
	TComponentStorage<T>* FRegistry::GetStorage() const
	{
		return static_cast<TComponentStorage<T>*>(
			GetStorageInternal(TypeId<T>())
		);
	}
}
