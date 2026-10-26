#pragma once

#include "CoreMinimal.h"
#include "ECSTypes.h"
#include "ECSEntityComponent.generated.h"

UCLASS(ClassGroup=(ECS), meta=(BlueprintSpawnableComponent))
class ECS_API UECSEntityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ECS::FEntity GetEntity() const { return Entity; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PopulateEntity(ECS::FEntity InEntity) {}
	
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

private:
	ECS::FEntity Entity;
};
