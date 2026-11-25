#include "UObjects/Subsystem/ECSSubsystem.h"
#include "System/ECSSystemRegistration.h"
#include "StructUtils/InstancedStruct.h"
#include "ECSAutoRegistrationUTH.h"
#include "Debug/CVars/ECSConsoleVars.h"
#include "Debug/ECSDebugInfo.h"

void UECSSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	SetupSystemsProvider();
}

void UECSSubsystem::Tick(float DeltaTime)
{
	Scheduler.Update(Registry, DeltaTime);
#if WITH_EDITOR
	PrintScreenDiagnostics();
#endif
}

TStatId UECSSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UECSSubsystem, STATGROUP_Tickables);
}

ECS::FEntity UECSSubsystem::CreateEntity()
{
	return Registry.Create();
}

void UECSSubsystem::DestroyEntity(ECS::FEntity Entity)
{
	Registry.Destroy(Entity);
}

void* UECSSubsystem::AddComponentToEntity(ECS::FEntity Entity, const FInstancedStruct& Struct)
{
	const UScriptStruct* T = Struct.GetScriptStruct();
	return T ? ECS::EnsureStorage(Registry, T).AddRaw(Entity, Struct.GetMemory())
			 : nullptr;
}

void UECSSubsystem::SetupSystemsProvider()
{
	auto Sink = [this](TUniquePtr<ECS::ISystem> System) -> ECS::ISystem&
	{
		return Scheduler.Add(MoveTemp(System));
	};

	ECS::FSystemRegistrar Registrar(Sink, GetWorld());

	ECS::ForEachProvider([&](const ECS::FSystemsProvider& Provider)
	{
		Provider.ExecuteIfBound(Registrar);
	});
}

void UECSSubsystem::PrintScreenDiagnostics() const
{
	if (!GEngine) return;
	
	if (ECS::CVarEnableScreenDiagnostics.GetValueOnGameThread() == 1)
	{
		const FString& DebugMessage = ConstructDebugMessage(Registry);
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Turquoise, DebugMessage);
	}
	else
	{
		GEngine->ClearOnScreenDebugMessages();
	}
}
