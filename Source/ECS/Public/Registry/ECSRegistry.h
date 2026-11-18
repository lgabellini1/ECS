#pragma once
#include "ECSTypes.h"
#include "Storage/ECSComponentStorage.h"

class UECSSubsystem;

namespace ECS
{
	class IStorage;
	class FRegistryImpl;
	
	class ECS_API FRegistry final
	{
		// private subsystem has access to both the public view and internal impl,
		// so that it can dispatch calls to one or the other according to necessity.
		friend class UECSSubsystem;
		
	public:
		FRegistry() = default;
		explicit FRegistry(TPimplPtr<FRegistryImpl>&&);

		FRegistry(const FRegistry&)			   = delete;
		FRegistry& operator=(const FRegistry&) = delete;

		FEntity Create();
		void    Destroy(FEntity);
		
		template <component T>
		TComponentStorage<T>* GetStorage() const;
	private:
		TPimplPtr<FRegistryImpl> Impl;
		
		IStorage* GetStorageInternal(uint32 TypeId) const;
	};
}

#include "ECSRegistry.inl"
