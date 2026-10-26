#include "CoreMinimal.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Entity/ECSEntityManager.h"

namespace ECSEntityManagerTests
{
	TEST_CASE_NAMED(EntityManagerCreate, "ECS::EntityManager::Create", "[unit][ecs]")
    {
        SECTION("A new entity is alive, a null one is not")
        {
            ECS::FEntityManager Entities;
            const ECS::FEntity Entity = Entities.Create();
        
            CHECK(Entities.IsAlive(Entity));
            CHECK_FALSE(Entities.IsAlive(ECS::FEntity()));

            Entities.Destroy(Entity);
        }
    }

    TEST_CASE_NAMED(EntityManagerWrap, "ECS::EntityManager::GenerationWrap", "[unit][ecs]")
    {
        ECS::FEntityManager Entities;

        const ECS::FEntity First = Entities.Create();
        Entities.Destroy(First);
        
        for (int32 i = 1; i < static_cast<int32>(ECS::EntityGenerationMask) + 1; i++)
        {
            const ECS::FEntity Entity = Entities.Create();
            Entities.Destroy(Entity);
        }

        SECTION("Generation wraps correctly")
        {
            const ECS::FEntity Entity = Entities.Create();
            CHECK(Entity.Generation() == 0);
            CHECK(Entities.IsAlive(Entity));
        };

        SECTION("First-life entity reads as alive again")
        {
            CHECK(Entities.IsAlive(First));
        }

        SECTION("Generation never exceeds it's limit")
        {
            for (int32 i = 0; i < 1000; i++)
            {
                const ECS::FEntity Entity = Entities.Create();
                Entities.Destroy(Entity);
                CHECK(Entity.Generation() <= ECS::EntityGenerationMask);
            }
        }
    }

    TEST_CASE_NAMED(EntityManagerDestroy, "ECS::EntityManager::Destroy", "[unit][ecs]")
    {
        ECS::FEntityManager Entities;
        
        SECTION("Double destroy only counts once")
        {
            REQUIRE(Entities.GetLivingCount() == 0);
            
            const ECS::FEntity Entity = Entities.Create();
            Entities.Destroy(Entity);
            Entities.Destroy(Entity);
            
            CHECK(Entities.GetLivingCount() == 0);    
        }

        SECTION("Destroy invalidates the old handle and recycling bumps generation")
        {
            const ECS::FEntity Old = Entities.Create();
            Entities.Destroy(Old);
            REQUIRE(!Entities.IsAlive(Old));

            const ECS::FEntity New = Entities.Create();
            CHECK(New.Index() == Old.Index());
            CHECK(New.Generation() != Old.Generation());
            CHECK_FALSE(Entities.IsAlive(Old));
        }

        SECTION("Destroying a stale handle does not kill the new occupant")
        {
            const ECS::FEntity Old = Entities.Create();
            Entities.Destroy(Old);
            const ECS::FEntity New = Entities.Create();

            Entities.Destroy(Old);

            CHECK(Entities.IsAlive(New));
        }
    }

    TEST_CASE_NAMED(EntityManagerAlive, "ECS::EntityManager::IsAlive", "[unit][ecs]")
    {
        ECS::FEntityManager Entities;

        SECTION("Out-of-range index is not alive")
        {
            const auto OutOfRangeEntity = ECS::FEntity(999, 0); 
            CHECK_FALSE(Entities.IsAlive(OutOfRangeEntity));
        }

        SECTION("Right index, wrong generation is not alive")
        {
            const ECS::FEntity First = Entities.Create();
            const uint32 Index = First.Index();
            Entities.Destroy(First);

            const ECS::FEntity Second = Entities.Create();
            REQUIRE(Second.Index() == Index);
            REQUIRE(static_cast<int32>(Second.Generation()) >= 0);

            const ECS::FEntity Older(Index, Second.Generation() - 1);
            CHECK_FALSE(Entities.IsAlive(Older));

            const ECS::FEntity Younger(Index, Second.Generation() + 1);
            CHECK_FALSE(Entities.IsAlive(Younger));
        }
    }    
}
