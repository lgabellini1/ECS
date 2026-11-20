#include "Tests/ECSTestCommons.h"
#include "Entity/ECSEntityManager.h"

using namespace ECS;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEntityManagerTest_ImplicitListConstruction,
	"ECS::EntityManager::ImplicitListConstruction", TestFlags);

bool FEntityManagerTest_ImplicitListConstruction::RunTest(const FString& Parameters)
{
	FEntityManager  Entities;
	TArray<FEntity> Created;
	for (int32 i = 0; i < 5; ++i)
	{
		Created.Add(Entities.Create());
	}
	Entities.Destroy(Created[0]);
	Entities.Destroy(Created[2]);
	TestEqual(TEXT("Living count with no recycling is correct"),
		Entities.GetLivingCount(), 3);
	for (int32 i = 0; i < 5; ++i)
	{
		Entities.Destroy(Created[i]);
		TestTrue(TEXT("Living count always positive or equal zero"),
			Entities.GetLivingCount() >= 0);
	}
	Created.Reset();
	Created.SetNum(5);
	for (int32 i : { 0, 2, 4 })
	{
		Created[i] = Entities.Create();
	}
	TestEqual(TEXT("Living count with recycling is correct"),
		Entities.GetLivingCount(), 3);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEntityManagerTest_RejectsNull,
	"ECS::EntityManager::RejectsNull", TestFlags);

bool FEntityManagerTest_RejectsNull::RunTest(const FString& Parameters)
{
	FEntityManager Entities;
	const FEntity Entity = Entities.Create();
	Entities.Destroy(Entity);
	TestFalse(TEXT("Null is not alive"), Entities.IsAlive(FEntity::Null));
	TestEqual(TEXT("Null is not alive"), Entities.GetLivingCount(), 0);
	return true;
}
