#pragma once

#include "CoreMinimal.h"
#include "ECSTypes.h"
#include "ECS/Private/ECSRegistry.h"
#include "ECS/Private/ECSScheduler.h"
#include "ECSWorldSubsystem.generated.h"

UCLASS()
class ECS_API UECSSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;

	ECS::FEntity CreateEntity();
	void DestroyEntity(ECS::FEntity Entity);

	template <ECS::component T>
	T* AddComponentToEntity(ECS::FEntity Entity, T Comp = T());
	
	template <ECS::component T>
	T* GetComponent(ECS::FEntity Entity);
	
private:
	void PrintScreenDiagnostics() const;
	
	ECS::FRegistry Registry;

protected:
	virtual void RegisterECSSystems() {}
	
	ECS::FScheduler Scheduler;
};

template <ECS::component T>
T* UECSSubsystem::AddComponentToEntity(ECS::FEntity Entity, T Comp)
{
	return Registry.Add<T>(Entity, Comp);
}

template <ECS::component T>
T* UECSSubsystem::GetComponent(ECS::FEntity Entity)
{
	return Registry.Get<T>(Entity);
}
