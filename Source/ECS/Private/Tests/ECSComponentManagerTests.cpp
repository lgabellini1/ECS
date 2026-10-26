#include "CoreMinimal.h"
#include "Misc/LowLevelTestAdapter.h"
#include "ECSUtils.h"
#include "Component/ECSComponentManager.h"
#include "Entity/ECSEntityManager.h"

namespace ECSComponentManagerTests
{
	struct FComponentA { int32 Value = 0; };
	struct FComponentB { float Value = 0.f; };

	TEST_CASE_NAMED(CompManagerTypeId, "ECS::ComponentManager::TypeId", "[unit][ecs]")
	{
		SECTION("Stable per type, distinct across types")
		{
			const uint32 A1 = ECS::TypeId<FComponentA>();
			const uint32 A2 = ECS::TypeId<FComponentA>();
			const uint32 B  = ECS::TypeId<FComponentB>();
				
			CHECK(A1 == A2);
			CHECK(A1 != B);
		}
	}

	TEST_CASE_NAMED(CompManagerAdd, "ECS::ComponentManager::Add", "[unit][ecs]")
	{
		ECS::FComponentManager Components;

		SECTION("Each type is stored independently")
		{
			Components.Add<FComponentA>(ECS::FEntity(3), FComponentA{ 7 });
			Components.Add<FComponentB>(ECS::FEntity(5), FComponentB{ 2.5f });
			
			REQUIRE(Components.Has<FComponentA>(ECS::FEntity(3)));
			REQUIRE(Components.Has<FComponentB>(ECS::FEntity(5)));
			
			CHECK(Components.Get<FComponentA>(ECS::FEntity(3))->Value == 7);
			CHECK(Components.Get<FComponentB>(ECS::FEntity(5))->Value == 2.5f);
			CHECK_FALSE(Components.Has<FComponentA>(ECS::FEntity(5)));
			CHECK_FALSE(Components.Has<FComponentB>(ECS::FEntity(3)));
		}
	}

	TEST_CASE_NAMED(CompManagerRemove, "ECS::ComponentManager::Remove", "[unit][ecs]")
	{
		ECS::FComponentManager Components;

		SECTION("Remove only affects the requested type")
		{
			Components.Add<FComponentA>(ECS::FEntity(1));
			Components.Add<FComponentB>(ECS::FEntity(1));
			
			Components.Remove<FComponentA>(ECS::FEntity(1));
			
			CHECK_FALSE(Components.Has<FComponentA>(ECS::FEntity(1)));
			CHECK(Components.Has<FComponentB>(ECS::FEntity(1)));

			Components.Remove<FComponentB>(ECS::FEntity(1));
		}
	}

	TEST_CASE_NAMED(CompManagerEntityDestroyed, "ECS::ComponentManager::EntityDestroyed", "[unit][ecs]")
	{
		ECS::FComponentManager Components;

		SECTION("Destroying an entity with no components, or on an empty manager, is a no-op")
		{
			Components.SignalEntityDestroyed(ECS::FEntity(42));

			Components.Add<FComponentA>(ECS::FEntity(1));
			Components.SignalEntityDestroyed(ECS::FEntity(42));
			CHECK(Components.Has<FComponentA>(ECS::FEntity(1)));

			Components.Remove<FComponentA>(ECS::FEntity(1));
		}
		
		SECTION("Clears every storage for that entity and leaves other entities alone")
		{
			Components.Add<FComponentA>(ECS::FEntity(3));
			Components.Add<FComponentB>(ECS::FEntity(3), FComponentB{ 1.f });
			Components.Add<FComponentB>(ECS::FEntity(5), FComponentB{ 5.f });
			
			Components.SignalEntityDestroyed(ECS::FEntity(3));
			
			CHECK_FALSE(Components.Has<FComponentA>(ECS::FEntity(3)));
			CHECK_FALSE(Components.Has<FComponentB>(ECS::FEntity(3)));
			
			REQUIRE(Components.Has<FComponentB>(ECS::FEntity(5)));
			CHECK(Components.Get<FComponentB>(ECS::FEntity(5))->Value == 5.f);
		}
	}

	TEST_CASE_NAMED(CompManagerQueries, "ECS::ComponentManager::Queries", "[unit][ecs]")
	{
		ECS::FComponentManager Components;
		
		SECTION("Queries on a type nobody added are 'no' and do not create a storage")
		{
			struct FCompNeverAdded {};
			
			CHECK_FALSE(Components.Has<FCompNeverAdded>(ECS::FEntity(3)));
			CHECK_FALSE(Components.Get<FCompNeverAdded>(ECS::FEntity(3)));
			CHECK_FALSE(Components.GetStorage<FCompNeverAdded>());
		}
	}
}
