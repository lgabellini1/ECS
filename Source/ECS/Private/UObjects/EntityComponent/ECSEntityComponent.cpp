#include "ECSEntityComponent.h"
#include "UObjects/Subsystem/ECSSubsystem.h"

void UECSEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (auto* Subsystem = GetWorld()->GetSubsystem<UECSSubsystem>())
	{
		Entity = Subsystem->CreateEntity();
	}
}

void UECSEntityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (auto* Subsystem =
		GetWorld() ? GetWorld()->GetSubsystem<UECSSubsystem>() : nullptr)
	{
		Subsystem->DestroyEntity(Entity);
	}

	Entity = ECS::FEntity::Null;
	
	Super::EndPlay(EndPlayReason);
}
