#pragma once
#include "Misc/AutomationTest.h"
#include "Registry/ECSRegistryImpl.h"
#include "Registry/ECSRegistry.h"
#include "ECSTestCommons.generated.h"

using namespace ECS;

constexpr EAutomationTestFlags TestFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter;

USTRUCT()
struct FTestComponentInt   { GENERATED_BODY() int32 Value; };
USTRUCT()
struct FTestComponentFloat { GENERATED_BODY() float Value; };

struct FTestRegistry
{
	FRegistry  Registry;
	FRegistryImpl* Impl;
};

inline FTestRegistry MakeTestRegistry()
{
	auto Impl = MakePimpl<FRegistryImpl>();
	FRegistryImpl* RawImpl = Impl.Get();

	return FTestRegistry { FRegistry(MoveTemp(Impl)), RawImpl };
}
