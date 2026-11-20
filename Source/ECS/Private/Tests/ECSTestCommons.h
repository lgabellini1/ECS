#pragma once
#include "Misc/AutomationTest.h"
#include "ECSTypes.h"
#include "Registry/ECSRegistryImpl.h"
#include "Registry/ECSRegistry.h"

using namespace ECS;

constexpr EAutomationTestFlags TestFlags = EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter;

struct FTestComponentInt   { int32 Value; };
struct FTestComponentFloat { float Value; };

struct FTestRegistry
{
	FRegistry  Registry;
	FRegistryImpl* Impl;
};

inline auto MakeTestRegistry()
{
	auto Impl = MakePimpl<FRegistryImpl>();
	FRegistryImpl* RawImpl = Impl.Get();

	return FTestRegistry { FRegistry(MoveTemp(Impl)), RawImpl };
}
