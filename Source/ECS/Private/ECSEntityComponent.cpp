#include "UObjects/ECSEntityComponent.h"
#include "UObjects/ECSWorldSubsystem.h"

void UECSEntityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (auto* Subsystem =
		GetWorld() ? GetWorld()->GetSubsystem<UECSSubsystem>() : nullptr)
	{
		Entity = Subsystem->CreateEntity();
		PopulateEntity(Entity);
	}
}

void UECSEntityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (auto* Subsystem =
		GetWorld() ? GetWorld()->GetSubsystem<UECSSubsystem>() : nullptr)
	{
		Subsystem->DestroyEntity(Entity);
	}
	Entity = ECS::FEntity();
	
	Super::EndPlay(EndPlayReason);
}

void UECSEntityComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (auto* Subsystem =
		GetWorld() ? GetWorld()->GetSubsystem<UECSSubsystem>() : nullptr)
	{
		if (ECS::FTransform3D* Transform = Subsystem->GetComponent<ECS::FTransform3D>(Entity))
		{
			Transform->Location = GetOwner()->GetActorLocation();
		}
	}
}
