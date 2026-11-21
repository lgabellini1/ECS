#include "UObjects/Subsystem/ECSSubsystem.h"
#include "System/ECSSystemRegistration.h"
#include "System/ECSSystemRegistrationInternal.h"

void UECSSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetupSystemsProvider();
}

void UECSSubsystem::Tick(float DeltaTime)
{
	Scheduler.Update(Registry, DeltaTime);
}

ECS::FEntity UECSSubsystem::CreateEntity()
{
	return Registry.Create();
}

void UECSSubsystem::DestroyEntity(ECS::FEntity Entity)
{
	Registry.Destroy(Entity);
}

void UECSSubsystem::SetupSystemsProvider()
{
	auto Sink = [this](TUniquePtr<ECS::ISystem> System) -> ECS::ISystem&
	{
		return Scheduler.Add(MoveTemp(System));
	};

	ECS::FSystemRegistrar Registrar(Sink);

	ECS::ForEachProvider([&](const ECS::FSystemsProvider& Provider)
	{
		Provider.ExecuteIfBound(Registrar);
	});
}
