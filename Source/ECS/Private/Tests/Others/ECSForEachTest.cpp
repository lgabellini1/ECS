#include "Tests/ECSTestCommons.h"
#include "ECSFunctions.h"
#include "Registry/ECSRegistryImpl.h"

using namespace ECS;

template <component... TInclude>
TArray<FEntity> Visit(FRegistry& Registry)
{
	TArray<FEntity> Out;
	ForEach<TInclude...>(Registry, [&](FEntity Entity, TInclude&...)
	{
		Out.Add(Entity);
	});
	return Out;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FForEach_Empty,
	"ECS::ForEach::Empty", TestFlags);

bool FForEach_Empty::RunTest(const FString& Parameters)
{
	FRegistry Registry;
	TestTrue(TEXT("On empty registry, visits nothing"),
		Visit<FTestComponentInt>(Registry).IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FForEach_Mutations,
	"ECS::ForEach::Mutations", TestFlags);

bool FForEach_Mutations::RunTest(const FString& Parameters)
{
	{
		FTestRegistry TestRegistry = MakeTestRegistry();
		const FEntity New = TestRegistry.Registry.Create();
		TestRegistry.Impl->Add<FTestComponentInt>(New, {2});
	
		ForEach<FTestComponentInt>(TestRegistry.Registry, [&](FEntity Entity, FTestComponentInt& Comp)
		{
			Comp.Value += 10;
		});

		TestEqual(TEXT("Component mutations in ForEach are preserved"),
			TestRegistry.Impl->Get<FTestComponentInt>(New)->Value, 12);
	}
	{
		FTestRegistry TestRegistry = MakeTestRegistry();
		TArray<FEntity> Entities, Visited;
		for (int32 i = 0; i < 10; ++i)
		{
			Entities.Add(TestRegistry.Registry.Create());
			TestRegistry.Impl->Add<FTestComponentInt>(Entities[i], {i});
		}

		ForEach<FTestComponentInt>(TestRegistry.Registry,
			[&](FEntity Entity, FTestComponentInt& Comp)
		{
			Visited.Add(Entity);
			TestRegistry.Registry.Remove<FTestComponentInt>(Entity);
		});

		TestEqual(TEXT("Component removal while iterating still visits all entities"), 
			Visited.Num(), Entities.Num());
		TestEqual(TEXT("Components get safely removed"),
			Visit<FTestComponentInt>(TestRegistry.Registry).Num(), 0);
	}
	{
		FTestRegistry TestRegistry = MakeTestRegistry();
		TArray<FEntity> Entities, Visited;
		for (int32 i = 0; i < 10; ++i)
		{
			Entities.Add(TestRegistry.Registry.Create());
			TestRegistry.Impl->Add<FTestComponentInt>(Entities[i], {i});
		}

		TArray<FEntity> FromIteration;
		ForEach<FTestComponentInt>(TestRegistry.Registry,
			[&](FEntity Entity, FTestComponentInt& Comp)
		{
			Visited.Add(Entity);
			FromIteration.Add(TestRegistry.Registry.Create());
			TestRegistry.Impl->Add<FTestComponentInt>(FromIteration.Last());	
		});

		TestEqual(TEXT("Matching entities created while iterating get skipped in the same loop"),
			Visited.Num(), Entities.Num());
	}

	return true;
}
