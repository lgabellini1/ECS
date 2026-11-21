#include "Tests/ECSTestCommons.h"
#include "Algo/Contains.h"
#include "Storage/ECSComponentStorage.h"
#include "Entity/ECSEntityConsts.h"

using namespace ECS;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentStorage_HasRefusesNullForAnyGeneration,
	"ECS::ComponentStorage::HasRefusesNullForAnyGeneration", TestFlags);

bool FComponentStorage_HasRefusesNullForAnyGeneration::RunTest(const FString& Parameters)
{
	TComponentStorage<FTestComponentInt> Storage;
	const FEntity Entity = FEntity(1024, 0);
	Storage.Add(Entity);
	
	bool bAllFalse = true;
	for (uint32 i = 0; i < EntityGenerationMask; ++i)
	{
		if (const FEntity TestNull = FEntity(NullEntityIndex, i);
			Storage.Has(TestNull))
		{
			bAllFalse = false;	
		}
	}

	TestTrue(TEXT("Has() refuses null for any generation"), bAllFalse);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentStorage_Compaction,
	"ECS::ComponentStorage::Compaction", TestFlags)

bool FComponentStorage_Compaction::RunTest(const FString& Parameters)
{
	TComponentStorage<FTestComponentInt> Storage;
	TArray<FEntity> Expected;
	for (int32 i = 0; i < 10; ++i)
	{
		Storage.Add(FEntity(i));
		Expected.Add(FEntity(i));
	}
	
	const std::initializer_list ToRemove{ 1, 4, 6, 9, 10 };
	for (int32 i : ToRemove)
	{
		Storage.Remove(FEntity(i));
		Expected.Remove(FEntity(i));
	}
	
	TestEqual(TEXT("Storage is of correct size"), Storage.Num(), Expected.Num());
	for (int32 i = 0; i < 10; ++i)
	{
		Algo::Contains(ToRemove, i)
			? TestFalse(TEXT("Storage has all correct elements"), Storage.Has(FEntity(i)))
			: TestTrue(TEXT("Storage has all correct elements"), Storage.Has(FEntity(i)));
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentStorage_NoDuplication,
	"ECS::ComponentStorage::NoDuplication", TestFlags)

bool FComponentStorage_NoDuplication::RunTest(const FString& Parameters)
{
	TComponentStorage<FTestComponentInt> Storage;
	const FEntity Entity = FEntity(2);

	Storage.Add(Entity, FTestComponentInt(0));
	Storage.Add(Entity, FTestComponentInt(1));
	TestEqual(TEXT("Storage count stays one"), Storage.Num(), 1);
	
	return true;
}
