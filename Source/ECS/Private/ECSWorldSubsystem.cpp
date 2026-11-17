#include "UObjects/ECSWorldSubsystem.h"
#include "Console/ECSConsoleVars.h"

void UECSSubsystem::Tick(float DeltaTime)
{
	Scheduler.Update(Registry, DeltaTime);
	PrintScreenDiagnostics();
}

TStatId UECSSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UECSSubsystem, STATGROUP_Tickables);
}

void UECSSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RegisterECSSystems();
}

bool UECSSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return false; // Only subclassing subsystems should be created.
}

bool UECSSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::PIE || WorldType == EWorldType::Game;
}

ECS::FEntity UECSSubsystem::CreateEntity()
{
	return Registry.CreateEntity();
}

void UECSSubsystem::DestroyEntity(ECS::FEntity Entity)
{
	Registry.Commands().Destroy(Entity);
}

void UECSSubsystem::PrintScreenDiagnostics() const
{
	if (!GEngine) return;
	
	if (ECS::CVarEnableScreenDiagnostics.GetValueOnGameThread() == 1)
	{
		const FString& DebugMessage = FString::Printf(
			TEXT("ENTITY COMPONENT SYSTEM\n"
				"Num entities: %i"), Registry.NumEntities());
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Turquoise, DebugMessage);
	}
	else
	{
		GEngine->ClearOnScreenDebugMessages();
	}
}
