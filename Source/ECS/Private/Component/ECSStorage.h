#pragma once

#include "ECSTypes.h"
#include "ECSUtils.h"

namespace ECS
{
	// The ComponentManager will keep the storages like this:
	//			TArray<TStorage<?>>
	// Given that we can't supply a concrete type in the array definition,
	// we need a common virtual interface, at the cost of making Remove() a virtual call.
	
	class IStorage
	{
	public:
		virtual ~IStorage() = default;
		
		virtual void Remove(FEntity Entity) = 0;
		virtual int32 Num() const = 0;
		virtual TConstArrayView<FEntity> GetEntities() const = 0;
	};
	
	template <component T>
	class TComponentStorage final : public IStorage
	{
	public:
		T& Add(FEntity Entity, T Comp = T());
		virtual void Remove(FEntity Entity) override;

		bool Has(FEntity Entity) const;
		
		T* Get(FEntity Entity);

		virtual int32 Num() const override { return ComponentStorage.Num(); }
		virtual TConstArrayView<FEntity> GetEntities() const override { return MapComponentSlotToEntity; }
		TArray<int32> GetComponents() const { return ComponentStorage; };

	private:
		TArray<T> ComponentStorage;
		
		TArray<int32> MapEntityToComponentSlot;
		TArray<FEntity> MapComponentSlotToEntity;
		
		void UpdateMappingAdd(FEntity Entity);
		void UpdateMappingRemove(FEntity Entity);
	};

	template <component T>
	T& TComponentStorage<T>::Add(FEntity Entity, T Comp)
	{
		if (!Has(Entity))
		{
			UpdateMappingAdd(Entity);
			T& NewComp = ComponentStorage.Add_GetRef(MoveTemp(Comp));
			check(ComponentStorage.Num() == MapComponentSlotToEntity.Num());
			return NewComp;
		}

		const int32 Index = MapEntityToComponentSlot[Entity.Index()];
		return ComponentStorage[Index] = MoveTemp(Comp);
	}

	template <component T>
	void TComponentStorage<T>::Remove(FEntity Entity)
	{
		if (Has(Entity))
		{
			const int32 Index = MapEntityToComponentSlot[Entity.Index()];

			ComponentStorage.RemoveAtSwap(Index);
			UpdateMappingRemove(Entity);
			check(ComponentStorage.Num() == MapComponentSlotToEntity.Num());
		}
	}

	template <component T>
	bool TComponentStorage<T>::Has(FEntity Entity) const
	{
		return MapEntityToComponentSlot.IsValidIndex(Entity.Index())
			&& MapEntityToComponentSlot[Entity.Index()] != INDEX_NONE;
	}

	template <component T>
	T* TComponentStorage<T>::Get(FEntity Entity)
	{
		if (Has(Entity))
		{
			const int32 Index = MapEntityToComponentSlot[Entity.Index()];
			return &ComponentStorage[Index];
		}
		return nullptr;
	}

	template <component T>
	void TComponentStorage<T>::UpdateMappingAdd(FEntity Entity)
	{
		while (MapEntityToComponentSlot.Num() <= static_cast<int32>(Entity.Index()))
		{
			MapEntityToComponentSlot.Add(INDEX_NONE);
		}

		MapEntityToComponentSlot[Entity.Index()] = ComponentStorage.Num();
		MapComponentSlotToEntity.Add(Entity);
	}

	template <component T>
	void TComponentStorage<T>::UpdateMappingRemove(FEntity Entity)
	{
		const int32 CompSlot = MapEntityToComponentSlot[Entity.Index()];
		const FEntity LastEntity = MapComponentSlotToEntity.Last();
		
		MapComponentSlotToEntity.RemoveAtSwap(CompSlot);

		MapEntityToComponentSlot[LastEntity.Index()] = CompSlot;
		MapEntityToComponentSlot[Entity.Index()] = INDEX_NONE;
	}
}
