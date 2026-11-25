#pragma once
#include "CoreMinimal.h"
#include "ECSTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "ECSEntityComponent.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class UECSEntityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInstancedStruct> Components;
private:
	ECS::FEntity Entity;
};
