#include "CoreMinimal.h"
#include "Misc/LowLevelTestAdapter.h"
#include "ECSRegistry.h"

namespace ECSRegistryTests
{
	struct FComponentA { int32 Value = 0; };
	struct FComponentB { float Value = 0.f; };

	TEST_CASE_NAMED(RegistryBasics, "ECS::Registry::Basics", "[unit][ecs]")
	{
		ECS::FRegistry Registry;

		SECTION("Create an alive entity")
		{
			const ECS::FEntity Entity = Registry.CreateEntity();
			CHECK(Registry.IsAlive(Entity));
		}

		SECTION("Add(), Has(), Get(), Remove() per type")
		{
			const ECS::FEntity Entity = Registry.CreateEntity();
			
			const auto* Added = Registry.Add<FComponentA>(Entity, FComponentA{ 7 });
			REQUIRE(Added != nullptr);
			REQUIRE(Registry.Has<FComponentA>(Entity));
			
			CHECK(Registry.Get<FComponentA>(Entity)->Value == 7);
			CHECK_FALSE(Registry.Has<FComponentB>(Entity));
			
			Registry.Remove<FComponentA>(Entity);
			CHECK_FALSE(Registry.Has<FComponentA>(Entity));
			CHECK(Registry.IsAlive(Entity));
		}
	}

	TEST_CASE_NAMED(RegistryDestroy, "ECS::Registry::Destroy", "[unit][ecs]")
	{
	    SECTION("A newly created entity is alive")
	    {
	        ECS::FRegistry Registry;

	        const ECS::FEntity Entity = Registry.CreateEntity();
	        CHECK(Registry.IsAlive(Entity));
	    }

	    SECTION("A destroyed entity is no longer alive")
	    {
	        ECS::FRegistry Registry;

	        const ECS::FEntity Entity = Registry.CreateEntity();
	        Registry.DestroyEntity(Entity);
	        CHECK_FALSE(Registry.IsAlive(Entity));
	    }

	    SECTION("Destroying an entity removes its components")
	    {
	        ECS::FRegistry Registry;

	        const ECS::FEntity Entity = Registry.CreateEntity();
	        Registry.Add<FComponentA>(Entity, FComponentA{ 7 });
	        Registry.Add<FComponentB>(Entity, FComponentB{ 42 });

	        REQUIRE(Registry.Has<FComponentA>(Entity));
	        REQUIRE(Registry.Has<FComponentB>(Entity));

	        Registry.DestroyEntity(Entity);

	        CHECK_FALSE(Registry.Has<FComponentA>(Entity));
	        CHECK_FALSE(Registry.Has<FComponentB>(Entity));
	    }

	    SECTION("Destroying an entity without components is safe")
	    {
	        ECS::FRegistry Registry;

	        const ECS::FEntity Entity = Registry.CreateEntity();
	    	Registry.DestroyEntity(Entity);
	        CHECK_FALSE(Registry.IsAlive(Entity));
	    }

	    SECTION("A recycled index starts with no components")
	    {
	        ECS::FRegistry Registry;

	        const ECS::FEntity Old = Registry.CreateEntity();
	        Registry.Add<FComponentA>(Old, FComponentA{ 7 });

	        Registry.DestroyEntity(Old);

	        REQUIRE(!Registry.IsAlive(Old));

	        const ECS::FEntity New = Registry.CreateEntity();

	        REQUIRE(New.Index() == Old.Index());

	        CHECK(Registry.IsAlive(New));
	        CHECK_FALSE(Registry.Has<FComponentA>(New));
	        CHECK(Registry.Get<FComponentA>(New) == nullptr);
	    }

	    SECTION("Destroying one entity preserves other entities")
	    {
	        ECS::FRegistry Registry;

	        const ECS::FEntity A = Registry.CreateEntity();
	        const ECS::FEntity B = Registry.CreateEntity();

	        Registry.Add<FComponentA>(A, FComponentA{ 1 });
	        Registry.Add<FComponentA>(B, FComponentA{ 2 });

	        Registry.DestroyEntity(A);

	        CHECK_FALSE(Registry.IsAlive(A));
	        CHECK(Registry.IsAlive(B));
	        REQUIRE(Registry.Has<FComponentA>(B));
	        CHECK(Registry.Get<FComponentA>(B)->Value == 2);
	    }

	    SECTION("Destroying an entity twice is safe")
	    {
	        ECS::FRegistry Registry;

	        const ECS::FEntity Entity = Registry.CreateEntity();
	        Registry.DestroyEntity(Entity);
	    	Registry.DestroyEntity(Entity);
	        CHECK_FALSE(Registry.IsAlive(Entity));
	    }

		SECTION("A stale handle cannot access a recycled entity")
	    {
	    	ECS::FRegistry Registry;

	    	const ECS::FEntity Old = Registry.CreateEntity();
	    	Registry.Add<FComponentA>(Old, FComponentA{ 7 });

	    	Registry.DestroyEntity(Old);

	    	const ECS::FEntity New = Registry.CreateEntity();
	    	REQUIRE(New.Index() == Old.Index());

	    	CHECK_FALSE(Registry.IsAlive(Old));
	    	CHECK(Registry.IsAlive(New));
	    	CHECK_FALSE(Registry.Has<FComponentA>(Old));
	    }
	}

	TEST_CASE_NAMED(RegistryHandles, "ECS::Registry::Handles", "[unit][ecs]")
    {
        ECS::FRegistry Registry;
    
        SECTION("Operations on a destroyed handle are safe no-ops")
        {
            const ECS::FEntity Old = Registry.CreateEntity();
            Registry.DestroyEntity(Old);
            const ECS::FEntity New = Registry.CreateEntity();
        	
            CHECK_FALSE(Registry.Add<FComponentA>(Old));
            CHECK_FALSE(Registry.Has<FComponentA>(Old));
            CHECK_FALSE(Registry.Get<FComponentA>(Old));
            Registry.Remove<FComponentA>(Old);
        	
            CHECK_FALSE(Registry.Has<FComponentA>(New));
            CHECK(Registry.IsAlive(New));
        }
    
        SECTION("Null handle is safe everywhere")
        {
            const ECS::FEntity Null;
            CHECK_FALSE(Registry.IsAlive(Null));
            CHECK_FALSE(Registry.Add<FComponentA>(Null));
            CHECK_FALSE(Registry.Has<FComponentA>(Null));
            CHECK_FALSE(Registry.Get<FComponentA>(Null));
            Registry.Remove<FComponentA>(Null);
            Registry.DestroyEntity(Null);
        }
    }

	TEST_CASE_NAMED(RegistryCmdBuf, "ECS::Registry::CommandBuffer", "[unit][ecs]")
	{
		ECS::FRegistry Registry;

		SECTION("Destroy is deferred")
	    {
	        const ECS::FEntity A = Registry.CreateEntity();
	        const ECS::FEntity B = Registry.CreateEntity();
	
	        Registry.Commands().Destroy(A);
	        CHECK(Registry.IsAlive(A));
	
	        Registry.FlushCmds();
	        CHECK_FALSE(Registry.IsAlive(A));
	        CHECK(Registry.IsAlive(B));
	    }
	
	    SECTION("Add is deferred")
	    {
	        const ECS::FEntity Entity = Registry.CreateEntity();
	
	        Registry.Commands().Add<FComponentA>(Entity);
	        CHECK_FALSE(Registry.Has<FComponentA>(Entity));
	
	        Registry.FlushCmds();
	        CHECK(Registry.Has<FComponentA>(Entity));
	    }
	
	    SECTION("Commands on an entity destroyed earlier in the same flush are skipped")
	    {
	        const ECS::FEntity Entity = Registry.CreateEntity();
			
	        Registry.Commands().Destroy(Entity);
	        Registry.Commands().Add<FComponentA>(Entity);

			REQUIRE(Registry.IsAlive(Entity));
	        Registry.FlushCmds();
	        CHECK_FALSE(Registry.IsAlive(Entity));
	    }
		
	    SECTION("Commands run in queue order")
	    {
	        const ECS::FEntity Entity = Registry.CreateEntity();
	
	        Registry.Commands().Add<FComponentA>(Entity);
	        Registry.Commands().Remove<FComponentA>(Entity);
	        Registry.FlushCmds();
	        CHECK_FALSE(Registry.Has<FComponentA>(Entity));
			
	        Registry.Commands().Remove<FComponentA>(Entity);
	        Registry.Commands().Add<FComponentA>(Entity, FComponentA{ 2 });
	        Registry.FlushCmds();
	        REQUIRE(Registry.Has<FComponentA>(Entity));
	        CHECK(Registry.Get<FComponentA>(Entity)->Value == 2);
	    }
		
	    SECTION("Queueing destroy operations while walking a storage does not disturb the walk")
	    {
	        TArray<ECS::FEntity> Created;
	        for (int32 i = 0; i < 5; ++i)
	        {
	            const ECS::FEntity Entity = Registry.CreateEntity();
	        	REQUIRE(Registry.IsAlive(Entity));
	            Registry.Add<FComponentA>(Entity);
	            Created.Add(Entity);
	        }
			
	        auto* Storage = Registry.GetStorage<FComponentA>();
	        REQUIRE(Storage != nullptr);

			CHECK(Storage->Num() == Created.Num());
			CHECK(Storage->GetEntities().Num() == Created.Num());

			for (const ECS::FEntity Entity : Created)
			{
				CHECK(Storage->Has(Entity));
			}
	
	        int32 Visited = 0;
	        for (const ECS::FEntity& StoredEntity : Storage->GetEntities())
	        {
	            Registry.Commands().Destroy(StoredEntity);
	            ++Visited;
	        }

			REQUIRE(Storage->GetEntities().Num() == 5);
	        CHECK(Visited == 5);
	        CHECK(Storage->Num() == 5);
	
	        Registry.FlushCmds();
	        CHECK(Storage->Num() == 0);
	    }
	}
}
