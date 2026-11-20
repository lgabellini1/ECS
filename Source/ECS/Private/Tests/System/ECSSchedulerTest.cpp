#include "Tests/ECSTestCommons.h"
#include "ECSFunctions.h"
#include "System/ECSScheduler.h"
#include "Registry/ECSRegistryImpl.h"

using namespace ECS;

class FTestSystem1 final : public ISystem
{
public:
	virtual void Update(FRegistry& Registry, float DeltaTime) override
	{
		ForEach<FTestComponentInt>(Registry,
			[&](FEntity Entity, FTestComponentInt& Int)
		{
			Int.Value *= 2;
		});
	}
};

class FTestSystem2 final : public ISystem
{
public:
	virtual void Update(FRegistry& Registry, float DeltaTime) override
	{
		ForEach<FTestComponentInt>(Registry,
			[&](FEntity Entity, FTestComponentInt& Int)
		{
			if (Int.Value >= 2)
				Int.Value += 2;
		});
	}
}; 

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScheduler_LaterSystemsSeeEarlierSystems,
	"ECS::Scheduler::LaterSystemsSeeEarlierSystems", TestFlags);

bool FScheduler_LaterSystemsSeeEarlierSystems::RunTest(const FString& Parameters)
{
	FTestRegistry TestRegistry = MakeTestRegistry();
	const FEntity Entity = TestRegistry.Registry.Create();
	TestRegistry.Impl->Add<FTestComponentInt>(Entity, {1});
	
	FScheduler Scheduler;
	Scheduler.Add(MakeUnique<FTestSystem1>());
	Scheduler.Add(MakeUnique<FTestSystem2>());
	FTestComponentInt* Int = TestRegistry.Impl->Get<FTestComponentInt>(Entity);
	Scheduler.Update(TestRegistry.Registry, 0.016f);
	TestEqual(TEXT("Systems running later see the results of those running before"),
		Int->Value, 4);
	
	return true;
}
