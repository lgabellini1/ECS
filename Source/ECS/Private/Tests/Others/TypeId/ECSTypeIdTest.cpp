#include "Tests/ECSTestCommons.h"
#include "ECSUtils.h"

using namespace ECS;

uint32 Get_TU1();
uint32 Get_TU2();

namespace 
{
	template <int32 I>
	struct TUniqueProbe {};
}

template <template <int32> class TProbe, int32... Is>
static TArray<uint32> IdsOf(TIntegerSequence<int32, Is...>)
{
	TArray<uint32> Out;
	(Out.Add(TypeId<TProbe<Is>>()), ...);
	return Out;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECSTypeIdTest_DistinctTypesYieldDistinctIds,
	"ECS::TypeId::DistinctTypesYieldDistinctIds", TestFlags);

bool FECSTypeIdTest_DistinctTypesYieldDistinctIds::RunTest(const FString& Parameters)
{
	const TArray<uint32>& Ids = IdsOf<TUniqueProbe>(
		TMakeIntegerSequence<int32, 128>()
	);
	const TSet Uniques(Ids);
	TestEqual(TEXT("Instantiations of one template get distinct ids"),
		Uniques.Num(), 128);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECSTypeIdTest_StableAcrossTranslationUnits,
	"ECS::TypeId::StableAcrossTranslationUnits", TestFlags);

bool FECSTypeIdTest_StableAcrossTranslationUnits::RunTest(const FString& Parameters)
{
	{
		TestEqual(TEXT("TypeIds are consistent across TUs"),
			Get_TU1(), Get_TU2());	
	}
	{
		TestEqual(TEXT("TypeIds are consistent across TUs"),
			Get_TU2(), Get_TU1());	
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECSTypeIdTest_AliasesHaveSameId,
	"ECS::TypeId::AliasesHaveSameId", TestFlags);

bool FECSTypeIdTest_AliasesHaveSameId::RunTest(const FString& Parameters)
{
	struct FTestComponent {};
	using FAlias = FTestComponent;
	TestEqual(TEXT("Aliases have same id of the original type"),
		TypeId<FTestComponent>(), TypeId<FAlias>());	
	return true;
}
