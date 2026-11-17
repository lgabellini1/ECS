#include "CoreMinimal.h"
#include "Misc/LowLevelTestAdapter.h"
#include "ECS/Private/ECSRegistry.h"
#include "ECS/Public/ECSFunctions.h"
#include "ECS/Public/ECSTypes.h"

namespace ViewTests
{
	struct FCompA { int32 Value = 0; };
	struct FCompB { int32 Value = 0; };
	struct FCompC { int32 Value = 0; };

	struct FTagX {};
	struct FTagY {};
	struct FTagNeverAdded {};

	struct FVisits
	{
		TMap<uint32, int32> Counts;
		
		void Record(ECS::FEntity Entity)
		{
			++Counts.FindOrAdd(Entity.Index());
		}

		int32 Total() const
		{
			int32 N = 0;
			for (const auto& P : Counts)
			{
				N += P.Value;
			}
			return N;
		}

		int32 Of(ECS::FEntity Entity) const
		{
			const int32* C = Counts.Find(Entity.Index());
			return C ? *C : 0;
		}
	};

	TEST_CASE_NAMED(ECSForEachSingle, "ECS::ForEach::Single component", "[unit][ecs][view]")
    {
        ECS::FRegistry Registry;

        SECTION("Empty registry and missing storage visit nothing")
        {
            FVisits Visits;
            ECS::ForEach<FCompA>(Registry, [&](ECS::FEntity Entity, FCompA&)
            {
                Visits.Record(Entity);
            });
            CHECK(Visits.Total() == 0);
        }

        SECTION("Visits each entity exactly once, with the right handle and value")
        {
            TArray<ECS::FEntity> Created;
            for (int32 i = 0; i < 5; ++i)
            {
                const ECS::FEntity Entity = Registry.CreateEntity();
                Registry.Add<FCompA>(Entity, FCompA{ i });
                Created.Add(Entity);
            }
            Registry.Add<FCompB>(Registry.CreateEntity(), FCompB{ 99 });

            FVisits Visits;
            ECS::ForEach<FCompA>(Registry, [&](ECS::FEntity Entity, FCompA& A)
            {
                Visits.Record(Entity);
                CHECK(Registry.IsAlive(Entity));
                CHECK(Registry.Get<FCompA>(Entity)->Value == A.Value);
            });

            CHECK(Visits.Total() == 5);
            for (const ECS::FEntity& Entity : Created)
            {
                CHECK(Visits.Of(Entity) == 1);
            }
        }

        SECTION("Writes through the reference are visible afterwards")
        {
            const ECS::FEntity Entity = Registry.CreateEntity();
            Registry.Add<FCompA>(Entity, FCompA{ 1 });

            ECS::ForEach<FCompA>(Registry, [](ECS::FEntity, FCompA& A)
            {
                A.Value += 10;
            });

            CHECK(Registry.Get<FCompA>(Entity)->Value == 11);
        }

        SECTION("After a removal (swap-and-pop) every remaining entity is visited once")
        {
            TArray<ECS::FEntity> Created;
            for (int32 i = 0; i < 4; ++i)
            {
                const ECS::FEntity Entity = Registry.CreateEntity();
                Registry.Add<FCompA>(Entity, FCompA{ i });
                Created.Add(Entity);
            }
            Registry.DestroyEntity(Created[1]);

            FVisits Visits;
            ECS::ForEach<FCompA>(Registry, [&](ECS::FEntity Entity, FCompA&)
            {
                Visits.Record(Entity);
            });

            CHECK(Visits.Total() == 3);
            CHECK(Visits.Of(Created[0]) == 1);
            CHECK(Visits.Of(Created[1]) == 0);
            CHECK(Visits.Of(Created[2]) == 1);
            CHECK(Visits.Of(Created[3]) == 1);
        }
    }

    TEST_CASE_NAMED(ECSForEachOverload, "ECS::ForEach::Overload equivalence", "[unit][ecs][view]")
    {
        ECS::FRegistry Registry;

        for (int32 i = 0; i < 3; ++i)
        {
            const ECS::FEntity E = Registry.CreateEntity();
            Registry.Add<FCompA>(E, FCompA{ i });
            Registry.Add<FCompB>(E, FCompB{ i * 10 });
        }

        SECTION("One component")
        {
            int32 Convenience = 0, Explicit = 0;
            ECS::ForEach<FCompA>(Registry, [&](ECS::FEntity, FCompA&) { ++Convenience; });
            ECS::ForEach<FCompA>(Registry, ECS::TExclude<>(), [&](ECS::FEntity, FCompA&) { ++Explicit; });

            CHECK(Convenience == 3);
            CHECK(Explicit == 3);
        }

        SECTION("Two components")
        {
            int32 Convenience = 0, Explicit = 0;
            ECS::ForEach<FCompA, FCompB>(Registry, [&](ECS::FEntity, FCompA&, FCompB&) { ++Convenience; });
            ECS::ForEach<FCompA, FCompB>(Registry, ECS::TExclude<>(), [&](ECS::FEntity, FCompA&, FCompB&) { ++Explicit; });

            CHECK(Convenience == 3);
            CHECK(Explicit == 3);
        }
    }

    TEST_CASE_NAMED(ECSForEachMultiComp, "ECS::ForEach::Multiple components", "[unit][ecs][view]")
    {
        ECS::FRegistry Registry;
    
        SECTION("Only entities with all components are visited")
        {
            const ECS::FEntity Both  = Registry.CreateEntity();
            const ECS::FEntity OnlyA = Registry.CreateEntity();
            const ECS::FEntity OnlyB = Registry.CreateEntity();
            Registry.Add<FCompA>(Both,  FCompA{ 1 });  Registry.Add<FCompB>(Both,  FCompB{ 10 });
            Registry.Add<FCompA>(OnlyA, FCompA{ 2 });
            Registry.Add<FCompB>(OnlyB, FCompB{ 30 });

            FVisits Visits;
            ECS::ForEach<FCompA, FCompB>(Registry, [&](ECS::FEntity E, FCompA&, FCompB&)
            {
                Visits.Record(E);
            });

            CHECK(Visits.Total() == 1);
            CHECK(Visits.Of(Both) == 1);
        }
    
        SECTION("Driver order: A has fewer entities than B")
        {
            TArray<ECS::FEntity> WithBoth;
            for (int32 i = 0; i < 2; ++i)
            {
                const ECS::FEntity E = Registry.CreateEntity();
                Registry.Add<FCompA>(E, FCompA{ i });
                Registry.Add<FCompB>(E, FCompB{ i * 10 });
                WithBoth.Add(E);
            }
            for (int32 i = 0; i < 4; ++i)
            {
                Registry.Add<FCompB>(Registry.CreateEntity(), FCompB{ 500 + i });
            }

            FVisits Visits;
            ECS::ForEach<FCompA, FCompB>(Registry, [&](ECS::FEntity E, FCompA& A, FCompB& B)
            {
                Visits.Record(E);
                CHECK(B.Value == A.Value * 10); // arguments arrive in the declared order
            });

            CHECK(Visits.Total() == 2);
            for (const ECS::FEntity& E : WithBoth) { CHECK(Visits.Of(E) == 1); }
        }

        SECTION("Driver order: B has fewer entities than A")
        {
            TArray<ECS::FEntity> WithBoth;
            for (int32 i = 0; i < 2; ++i)
            {
                const ECS::FEntity E = Registry.CreateEntity();
                Registry.Add<FCompA>(E, FCompA{ i });
                Registry.Add<FCompB>(E, FCompB{ i * 10 });
                WithBoth.Add(E);
            }
            for (int32 i = 0; i < 4; ++i) { Registry.Add<FCompA>(Registry.CreateEntity(), FCompA{ 500 + i }); }
    
            FVisits Visits;
            ECS::ForEach<FCompA, FCompB>(Registry, [&](ECS::FEntity E, FCompA& A, FCompB& B)
            {
                Visits.Record(E);
                CHECK(B.Value == A.Value * 10);
            });

            CHECK(Visits.Total() == 2);
            for (const ECS::FEntity& E : WithBoth) { CHECK(Visits.Of(E) == 1); }
        }

        SECTION("Three components, arguments in declared order")
        {
            const ECS::FEntity All = Registry.CreateEntity();
            Registry.Add<FCompA>(All, FCompA{ 1 });
            Registry.Add<FCompB>(All, FCompB{ 2 });
            Registry.Add<FCompC>(All, FCompC{ 3 });

            const ECS::FEntity Partial = Registry.CreateEntity();
            Registry.Add<FCompA>(Partial, FCompA{ 9 });
            Registry.Add<FCompB>(Partial, FCompB{ 9 });

            FVisits Visits;
            ECS::ForEach<FCompA, FCompB, FCompC>(Registry, [&](ECS::FEntity E, FCompA& A, FCompB& B, FCompC& C)
            {
                Visits.Record(E);
                CHECK(A.Value == 1);
                CHECK(B.Value == 2);
                CHECK(C.Value == 3);
            });

            CHECK(Visits.Total() == 1);
            CHECK(Visits.Of(All) == 1);
        }

        SECTION("A missing included storage means zero visits and no crash")
        {
            Registry.Add<FCompA>(Registry.CreateEntity(), FCompA{ 1 });   // FCompB never added

            FVisits Visits;
            ECS::ForEach<FCompA, FCompB>(Registry, [&](ECS::FEntity E, FCompA&, FCompB&) { Visits.Record(E); });
    
            CHECK(Visits.Total() == 0);
        }
    }
    
    TEST_CASE_NAMED(ECSForEachExclusion, "ECS::ForEach::Exclusion", "[unit][ecs][view]")
    {
        ECS::FRegistry Registry;

        const ECS::FEntity Plain    = Registry.CreateEntity();
        const ECS::FEntity WithX    = Registry.CreateEntity();
        const ECS::FEntity WithY    = Registry.CreateEntity();
        const ECS::FEntity WithBoth = Registry.CreateEntity();
    
        for (const ECS::FEntity& E : { Plain, WithX, WithY, WithBoth })
        {
            Registry.Add<FCompA>(E, FCompA{ 1 });
        }
        Registry.Add<FTagX>(WithX,    FTagX{});
        Registry.Add<FTagY>(WithY,    FTagY{});
        Registry.Add<FTagX>(WithBoth, FTagX{});
        Registry.Add<FTagY>(WithBoth, FTagY{});

        SECTION("Excluded entities are skipped")
        {
            FVisits Visits;
            ECS::ForEach<FCompA>(Registry, ECS::TExclude<FTagX>(), [&](ECS::FEntity E, FCompA&) { Visits.Record(E); });
            
            CHECK(Visits.Total() == 2);
            CHECK(Visits.Of(Plain) == 1);
            CHECK(Visits.Of(WithY) == 1);
        }
    
        SECTION("Several excluded types: any one of them excludes")
        {
            FVisits Visits;
            ECS::ForEach<FCompA>(Registry, ECS::TExclude<FTagX, FTagY>(), [&](ECS::FEntity E, FCompA&) { Visits.Record(E); });

            CHECK(Visits.Total() == 1);
            CHECK(Visits.Of(Plain) == 1);
        }
    
        SECTION("An excluded type with no storage excludes nothing")
        {
            FVisits Visits;
            ECS::ForEach<FCompA>(Registry, ECS::TExclude<FTagNeverAdded>(), [&](ECS::FEntity E, FCompA&) { Visits.Record(E); });
    
            CHECK(Visits.Total() == 4);
        }
	    
        SECTION("Exclusion combined with two included components")
        {
            for (const ECS::FEntity& E : { Plain, WithX }) { Registry.Add<FCompB>(E, FCompB{ 2 }); }
            
            FVisits Visits;
            ECS::ForEach<FCompA, FCompB>(Registry, ECS::TExclude<FTagX>(), [&](ECS::FEntity E, FCompA&, FCompB&) { Visits.Record(E); });
            
            CHECK(Visits.Total() == 1);
            CHECK(Visits.Of(Plain) == 1);
        }
	    
        SECTION("An entity that gains the excluded type stops being visited")
        {
            Registry.Add<FTagX>(Plain, FTagX{});
            
            FVisits Visits;
            ECS::ForEach<FCompA>(Registry, ECS::TExclude<FTagX>(), [&](ECS::FEntity E, FCompA&) { Visits.Record(E); });
            
            CHECK(Visits.Total() == 1);
            CHECK(Visits.Of(WithY) == 1);
        }
    }
}
