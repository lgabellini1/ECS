#include "Tests/ECSTestCommons.h"
#include "Algo/Contains.h"
#include "Storage/ECSComponentStorage.h"
#include "UObjects/Subsystem/ECSAutoRegistrationUTH.h"

using namespace ECS;

struct FCounted
{
	static inline int32 Live = 0, Copies = 0, Moves = 0;
	static void Reset() { Live = Copies = Moves = 0; }

	int32 V = 0;
	
	FCounted()  { ++Live; }
	explicit FCounted(int32 N)
		: V(N)
	{ ++Live; }
	~FCounted() { --Live; }
	
	FCounted(const FCounted& Other) : V(Other.V) { ++Live; ++Copies; }
	FCounted(FCounted&& Other) : V(Other.V) { Other.V = -1; ++Live; ++Moves; }
	
	FCounted& operator=(const FCounted& Other) { V = Other.V; ++Copies; return *this; }
	FCounted& operator=(FCounted&& Other) { V = Other.V; Other.V = -1; ++Moves; return *this; }
	
	bool operator==(const FCounted& Other) const { return V == Other.V; }
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentOps_TypedStorageBehaviour,
        "ECS::ComponentOps::TypedStorageBehaviour", TestFlags)

bool FComponentOps_TypedStorageBehaviour::RunTest(const FString&)
{
    FCounted::Reset();
    const FComponentOps Ops = MakeOps<FCounted>();
    TestEqual(TEXT("Size"), Ops.Size, static_cast<int32>(sizeof(FCounted)));
    TestTrue(TEXT("Has move"), Ops.Move != nullptr);

    alignas(FCounted) uint8 A[sizeof(FCounted)], B[sizeof(FCounted)];
    Ops.Construct(Ops.Ctx, A);
    Ops.Construct(Ops.Ctx, B);
    TestEqual(TEXT("Two live"), FCounted::Live, 2);

    FCounted Src(7);
    Ops.Assign(Ops.Ctx, A, &Src);
    TestEqual(TEXT("Assign() copies value"), reinterpret_cast<FCounted*>(A)->V, 7);
    TestEqual(TEXT("Assign() is a copy"), FCounted::Copies, 1);
    TestEqual(TEXT("Source left untouched"), Src.V, 7);

    Ops.Move(Ops.Ctx, B, A);
    TestEqual(TEXT("Move() transfers value"), reinterpret_cast<FCounted*>(B)->V, 7);
    TestEqual(TEXT("Moved-from state"), reinterpret_cast<FCounted*>(A)->V, -1);
    TestEqual(TEXT("Move() counted"), FCounted::Moves, 1);

    Ops.Destroy(Ops.Ctx, A);
    Ops.Destroy(Ops.Ctx, B);
    TestEqual(TEXT("Only Src left alive"), FCounted::Live, 1);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentOps_Reflection,
		"ECS::ComponentOps::Reflection", TestFlags)

bool FComponentOps_Reflection::RunTest(const FString&)
{
    const FComponentOps Ops = OpsFor(FTestComponentInt::StaticStruct());
    TestTrue(TEXT("Reflected Ops have no move"), Ops.Move == nullptr);
    TestEqual(TEXT("Size matches sizeof"), Ops.Size,
    	static_cast<int32>(sizeof(FTestComponentInt)));
    TestTrue(TEXT("Alignment"), Ops.Align >= static_cast<int32>(alignof(FTestComponentInt)));

    alignas(FTestComponentInt) uint8 Buf[sizeof(FTestComponentInt)];
    const FTestComponentInt Src(5);
    Ops.Construct(Ops.Ctx, Buf);
    TestTrue(TEXT("Default constructed"),
    	reinterpret_cast<FTestComponentInt*>(Buf)->Value == FTestComponentInt().Value);
    Ops.Assign(Ops.Ctx, Buf, &Src);
    TestTrue(TEXT("Assign copies"),
    	reinterpret_cast<FTestComponentInt*>(Buf)->Value == Src.Value);
    Ops.Destroy(Ops.Ctx, Buf);
    return true;
}
