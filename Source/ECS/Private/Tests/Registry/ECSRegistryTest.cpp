#include "Tests/ECSTestCommons.h"
#include "Registry/ECSRegistry.h"
#include "Registry/ECSRegistryImpl.h"

using namespace ECS;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegistryTest_EntitiesComponentsMatch,
	"ECS::Registry::EntitiesComponentsMatch", TestFlags);

bool FRegistryTest_EntitiesComponentsMatch::RunTest(const FString& Parameters)
{
	FRegistryImpl Registry;
	const FEntity Entity = Registry.Create();
	Registry.Add<FTestComponentInt>(Entity);
	Registry.Destroy(Entity);
	
	TestFalse(TEXT("Entity is not found in components"), Registry.Has<FTestComponentInt>(Entity));
	auto Storage = static_cast<TComponentStorage<FTestComponentInt>*>(
		Registry.GetStorage(TypeId<FTestComponentInt>())
	);
	TestFalse(TEXT("Entity is not found in components"), Storage->Has(Entity));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegistryTest_OperationsThroughStaleEntity,
	"ECS::Registry::OperationsThroughStaleEntity", TestFlags);

bool FRegistryTest_OperationsThroughStaleEntity::RunTest(const FString& Parameters)
{
	FRegistryImpl Registry;
	const FEntity Stale = Registry.Create();
	Registry.Add<FTestComponentInt>(Stale);
	Registry.Destroy(Stale);
	TestFalse(TEXT("Adding through stale entity is safe"), (bool)Registry.Add<FTestComponentFloat>(Stale));

	const FEntity Live = Registry.Create();
	Registry.Add<FTestComponentFloat>(Live);
	Registry.Remove<FTestComponentFloat>(Stale);
	TestTrue(TEXT("Removal through stale entity is safe"), Registry.Has<FTestComponentFloat>(Live));
	
	TestFalse(TEXT("Stale entity sees nothing"), Registry.Has<FTestComponentFloat>(Stale));
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRegistryTest_RecycleInheritsNoComponents,
	"ECS::Registry::RecycleInheritsNoComponents", TestFlags);

bool FRegistryTest_RecycleInheritsNoComponents::RunTest(const FString& Parameters)
{
	FRegistryImpl Registry;
	const FEntity Entity = Registry.Create();
	Registry.Add<FTestComponentInt>(Entity);
	Registry.Add<FTestComponentFloat>(Entity);
	Registry.Destroy(Entity);

	const FEntity Recycle = Registry.Create();
	if (Recycle.Index() != Entity.Index()) return false;
	
	TestFalse(TEXT("Recycling inherits no components"), Registry.Has<FTestComponentInt>(Recycle));
	TestFalse(TEXT("Recycling inherits no components"), Registry.Has<FTestComponentFloat>(Recycle));
	return true;
}
