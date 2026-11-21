#pragma once
#include "ECSTypes.h"

namespace ECS
{
	using FStorageFactory = TUniquePtr<IStorage>(*)();
	
	template <component T>
	class TComponentStorage final : public IStorage
	{
	public:
		T& Add(FEntity, T Component = T());
		virtual void Remove(FEntity) override;
		
		bool Has(FEntity) const;
		T* Get(FEntity);

		static TUniquePtr<IStorage> Make() { return MakeUnique<TComponentStorage>(); }

		int32 Num() const { return Dense.Num(); }
		const TArray<FEntity>& Entities() const { return Dense; }
	private:
		TArray<FEntity> Sparse;

		TArray<FEntity> Dense;
		// Component payload parallel with Dense
		TArray<T>	    Components; 
	};
}

#include "ECSComponentStorage.inl"
