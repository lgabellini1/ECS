#include "Tests/ECSTestCommons.h"
#include "Algo/Contains.h"
#include "Storage/ECSComponentStorage.h"
#include "Entity/ECSEntityConsts.h"
#include "UObjects/Subsystem/ECSAutoRegistrationUTH.h"

using namespace ECS;

static_assert(sizeof(TComponentStorage<FTestComponentInt>) == sizeof(FTypeErasedStorage));

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentStorage_TypedAndErasedAreSameStorage,
	"ECS::ComponentStorage::TypedAndErasedAreSameStorage", TestFlags)

bool FComponentStorage_TypedAndErasedAreSameStorage::RunTest(const FString& Parameters)
{
	auto [ Registry, Impl ] = MakeTestRegistry();

	TComponentStorage<FTestComponentInt>& Typed = Registry.GetOrCreateStorage<FTestComponentInt>();
	FTypeErasedStorage& Erased = EnsureStorage(Registry, FTestComponentInt::StaticStruct());

	TestTrue(TEXT("Same object"), static_cast<IStorage*>(&Erased) == static_cast<IStorage*>(&Typed));
	TestTrue(TEXT("Registry resolves to it"),
		Impl->GetStorage(KeyOf<FTestComponentInt>()) == static_cast<IStorage*>(&Erased));
	
	const FEntity E1(1, 0), E2(2, 0), E3(3, 0);
	FTestComponentInt V1(1), V2(2), V3(3);

	FTestComponentInt* P1 = &Typed.Add(E1, V1);
	TestTrue(TEXT("Typed Add() seen by erased"), Erased.GetRaw(E1) == P1);

	void* P2 = Erased.AddRaw(E2, &V2);
	TestTrue(TEXT("Erased Add() seen by typed"), Typed.Get(E2) == P2);
	TestTrue(TEXT("Payload stays intact"), Typed.Get(E2)->Value == V2.Value);

	Erased.Remove(E1);
	TestFalse(TEXT("Erased remove() seen by typed"), Typed.Has(E1));
	TestEqual(TEXT("Num agrees between typed and erased"), Typed.Num(), Erased.Num());
		
	return true;
}
