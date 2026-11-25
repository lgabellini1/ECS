#pragma once
#include "CoreMinimal.h"
#include "ECSTypes.h"
#include "Registry/ECSRegistry.h"
#include "System/ECSScheduler.h"
#include "ECSSubsystem.generated.h"

struct FInstancedStruct;

UCLASS()
class UECSSubsystem final : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override;

	ECS::FEntity CreateEntity();
	void		 DestroyEntity(ECS::FEntity);

	template <ECS::component T>
	T*    AddComponentToEntity(ECS::FEntity, T Component = T());
	void* AddComponentToEntity(ECS::FEntity, const FInstancedStruct&);
	
	template <ECS::component T>
	T* GetComponent(ECS::FEntity);
private:
	ECS::FRegistry	Registry;
	ECS::FScheduler	Scheduler;
	
	void SetupSystemsProvider();

	void PrintScreenDiagnostics() const;
};

template <ECS::component T>
T* UECSSubsystem::AddComponentToEntity(ECS::FEntity	Entity, T Component)
{
	return Registry.Add<T>(Entity, Component);
}

template <ECS::component T>
T* UECSSubsystem::GetComponent(ECS::FEntity Entity)
{
	return Registry.Get<T>(Entity);
}
