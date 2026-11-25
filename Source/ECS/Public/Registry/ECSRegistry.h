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
		FRegistry();
		explicit FRegistry(TPimplPtr<FRegistryImpl>&&);

		FRegistry(const FRegistry&)			   = delete;
		FRegistry& operator=(const FRegistry&) = delete;

		[[nodiscard]] FEntity Create();
		void Destroy(FEntity);
		bool IsAlive(FEntity) const;

		template <component T>
		T& Add(FEntity, T Component = T());

		template <component T>
		void Remove(FEntity);

		template <component T>
		bool Has(FEntity);

		template <component T>
		T* Get(FEntity);

		template <component T>
		T& GetChecked(FEntity);
		
		template <component T>
		TComponentStorage<T>* GetStorage() const;

		template <component T>
		TComponentStorage<T>& GetOrCreateStorage();
		IStorage&			  GetOrCreateStorage(FComponentKey, FStorageFactory);
	private:
		TPimplPtr<FRegistryImpl> Impl;
		
		IStorage* GetStorageInternal(FComponentKey) const;
	};
}

#include "ECSRegistry.inl"
