#include "ECSEntityComponent.h"
#include "UObjects/Subsystem/ECSSubsystem.h"

void UECSEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (auto* ECS = GetWorld()->GetSubsystem<UECSSubsystem>())
	{
		Entity = ECS->CreateEntity();
		for (const FInstancedStruct& Struct : Components)
		{
			ECS->AddComponentToEntity(Entity, Struct);
		}
	}
}

void UECSEntityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (auto* ECS =
		GetWorld() ? GetWorld()->GetSubsystem<UECSSubsystem>() : nullptr)
	{
		ECS->DestroyEntity(Entity);
	}

	Entity = ECS::FEntity::Null;
	
	Super::EndPlay(EndPlayReason);
}
