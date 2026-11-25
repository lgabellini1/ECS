#include "Tests/ECSTestCommons.h"
#include "Storage/ECSComponentStorage.h"

using namespace ECS;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentKey_Correctness,
	"ECS::ComponentKeys::Correctness", TestFlags)

bool FComponentKey_Correctness::RunTest(const FString&)
{
    const FComponentKey A = KeyOf<FTestComponentInt>(),
		B = KeyOf<FTestComponentFloat>();
	
    TestEqual(TEXT("Stability on multiple calls"), A, KeyOf<FTestComponentInt>());
    TestNotEqual(TEXT("Distinct types have distinct keys"), A, B);
    TestEqual(TEXT("USTRUCT key is its UScriptStruct"), A ,
    	FComponentKey::FromStruct(FTestComponentInt::StaticStruct()));
    TestEqual(TEXT("Hash stability"), GetTypeHash(A), GetTypeHash(KeyOf<FTestComponentInt>()));

    TestNotEqual(TEXT("Reflected and not-reflected never collide"), FComponentKey::FromId(5),
    	FComponentKey::FromStruct(reinterpret_cast<const UScriptStruct*>(uintptr_t(5))));
	
    return true;
}
