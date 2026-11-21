#include "Tests/ECSTestCommons.h"
#include "ECSTypes.h"
#include "Entity/ECSEntityConsts.h"

using namespace ECS;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEntityTest_DefaultIsNull,
	"ECS::Entity::Handle::DefaultIsNull", TestFlags)

bool FEntityTest_DefaultIsNull::RunTest(const FString& Parameters)
{
	const FEntity Default;
	TestEqual(TEXT("Null constructs well"),
		FEntity(NullEntityIndex, 0).Index(),NullEntityIndex);
	TestTrue(TEXT("Default entity is null"), Default.IsNull());
	TestTrue(TEXT("Default entity equals null handle"), Default == FEntity::Null);

	bool bAllNull = true;
	for (uint32 i = 0; i < EntityGenerationMask; ++i)
	{
		if (!FEntity(NullEntityIndex, i).IsNull())
			bAllNull = false;
	}
	TestTrue(TEXT("Null entity index always constructs null"), bAllNull);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEntityTest_IdStructure,
	"ECS::Entity::IdStructure", TestFlags);

bool FEntityTest_IdStructure::RunTest(const FString& Parameters)
{
	const FEntity Entity(5, 2);
	TestEqual(TEXT("Entity final Id is well-formed"),
		Entity, FEntity((2u << EntityIndexBits) | 5u));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEntityTest_GenerationWrapping,
	"ECS::Entity::GenerationWrapping", TestFlags);

bool FEntityTest_GenerationWrapping::RunTest(const FString& Parameters)
{
	const FEntity Wrapped(5, EntityGenerationMask + 1);
	TestEqual(TEXT("Index is unchanged by generation wrap"), Wrapped.Index(), 5u);
	TestEqual(TEXT("Generation wraps to zero"), Wrapped.Generation(), 0u);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEntityTest_Equality,
	"ECS::Entity::Equality", TestFlags);

bool FEntityTest_Equality::RunTest(const FString& Parameters)
{
	TestTrue(TEXT("Same index and generation are equal"), FEntity(3, 1) == FEntity(3, 1));
	TestFalse(TEXT("Different indices are unequal"), FEntity(3, 1) == FEntity(4, 1));
	TestFalse(TEXT("Different generations are unequal"), FEntity(3, 1) == FEntity(3, 2));
	return true;
}
