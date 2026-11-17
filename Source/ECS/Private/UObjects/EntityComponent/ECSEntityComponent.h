#pragma once
#include "CoreMinimal.h"
#include "ECSTypes.h"
#include "ECSEntityComponent.generated.h"

UCLASS()
class UECSEntityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	ECS::FEntity Entity;
};
