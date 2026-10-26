#include "CoreMinimal.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Component/ECSStorage.h"

namespace ECSComponentStorageTests
{
	struct FTestComponent
	{
		int32 Value = 0;
	};

	using FStorage = ECS::TComponentStorage<FTestComponent>;

	void VerifyConsistency(const FStorage& Storage)
	{
		const auto Entities = Storage.GetEntities();
		const auto Components = Storage.GetComponents();

		CHECK(Entities.Num() == Storage.Num());
		CHECK(Storage.Num() == Components.Num());

		TSet<uint32> Seen;
		for (int32 i = 0; i < Entities.Num(); ++i)
		{
			CHECK(Storage.Has(Entities[i]));
			CHECK_FALSE(Seen.Contains(Entities[i]));
			Seen.Add(Entities[i].Index());
		}
	}
	
	TEST_CASE_NAMED(CompStorageAdd, "ECS::ComponentStorage::Add", "[unit][ecs]")
	{
		FStorage Storage;

		SECTION("Map holes and out-of-range")
		{
			const ECS::FEntity Entity = ECS::FEntity(5);
	        Storage.Add(Entity, FTestComponent(42));
			
	        REQUIRE(Storage.Has(Entity));
	        REQUIRE(Storage.Get(Entity) != nullptr);
	        CHECK(Storage.Get(Entity)->Value == 42);
			
	        CHECK_FALSE(Storage.Has(ECS::FEntity(4))); // Hole
			CHECK(Storage.Get(ECS::FEntity(4)) == nullptr);
			
	        CHECK_FALSE(Storage.Has(ECS::FEntity(100))); // Out-of-range
			
	        CHECK(Storage.Num() == 1);
	        VerifyConsistency(Storage);

			Storage.Remove(Entity);
		}

		SECTION("Adding twice overwrites and does not duplicate")
		{
			const ECS::FEntity Entity = ECS::FEntity(3);
			Storage.Add(Entity, FTestComponent(1));
			Storage.Add(Entity, FTestComponent(2));
			REQUIRE(Storage.Has(Entity));
			
			CHECK(Storage.Num() == 1);
			CHECK(Storage.Get(Entity)->Value == 2);
			VerifyConsistency(Storage);
		}
	}

	TEST_CASE_NAMED(CompStorageRemove, "ECS::ComponentStorage::Remove", "[unit][ecs]")
	{
		FStorage Storage;

		SECTION("Removing swaps correctly")
		{
			Storage.Add(ECS::FEntity(1), FTestComponent(10));
			Storage.Add(ECS::FEntity(2), FTestComponent(20));
			Storage.Add(ECS::FEntity(3), FTestComponent(30));

			Storage.Remove(ECS::FEntity(1));

			REQUIRE(!Storage.Has(ECS::FEntity(1)));
			REQUIRE(Storage.Has(ECS::FEntity(2)));
			REQUIRE(Storage.Has(ECS::FEntity(3)));
		
			CHECK(Storage.Get(ECS::FEntity(2))->Value == 20);
			CHECK(Storage.Get(ECS::FEntity(3))->Value == 30);
			CHECK(Storage.Num() == 2);
			VerifyConsistency(Storage);

			Storage.Remove(ECS::FEntity(2));
			Storage.Remove(ECS::FEntity(3));
		}

		SECTION("Re-adding after removal")
		{
			Storage.Add(ECS::FEntity(1), FTestComponent(10));
			Storage.Add(ECS::FEntity(2), FTestComponent(20));
			Storage.Remove(ECS::FEntity(1));
			Storage.Add(ECS::FEntity(1), FTestComponent(99));
			
			CHECK(Storage.Get(ECS::FEntity(1))->Value == 99);
			CHECK(Storage.Get(ECS::FEntity(2))->Value == 20);
			CHECK(Storage.Num() == 2);
			VerifyConsistency(Storage);
			
			Storage.Remove(ECS::FEntity(2));
		}

		SECTION("Remove works through the IStorage interface")
		{
			Storage.Add(ECS::FEntity(1), FTestComponent(10));
			ECS::IStorage* Base = &Storage;
			Base->Remove(ECS::FEntity(1));
			
			CHECK_FALSE(Storage.Has(ECS::FEntity(1)));
			CHECK(Storage.Num() == 0);
		}
	}

	TEST_CASE_NAMED(CompStorageGetEntities, "ECS::ComponentStorage::GetEntities", "[unit][ecs]")
	{
		FStorage Storage;

		SECTION("Empty storage returns an empty view")
		{
			CHECK(Storage.Num() == 0);
			CHECK(Storage.GetEntities().Num() == 0);
		}

		SECTION("Adding one component returns its entity")
		{
			const ECS::FEntity A{0};
			Storage.Add(A);

			const auto Entities = Storage.GetEntities();

			CHECK(Entities.Num() == 1);
			CHECK(Entities[0] == A);
			CHECK(Storage.Num() == 1);
		}

		SECTION("Adding multiple components returns every entity")
		{
			const ECS::FEntity A{0};
			const ECS::FEntity B{1};
			const ECS::FEntity C{2};

			Storage.Add(A);
			Storage.Add(B);
			Storage.Add(C);

			const auto Entities = Storage.GetEntities();

			CHECK(Entities.Num() == 3);
			CHECK(Entities.Contains(A));
			CHECK(Entities.Contains(B));
			CHECK(Entities.Contains(C));
			CHECK(Storage.Num() == 3);
		}

		SECTION("Removing a component removes its entity from the view")
		{
			const ECS::FEntity A{0};
			const ECS::FEntity B{1};
			const ECS::FEntity C{2};

			Storage.Add(A);
			Storage.Add(B);
			Storage.Add(C);

			Storage.Remove(B);

			const auto Entities = Storage.GetEntities();

			CHECK(Entities.Num() == 2);
			CHECK(Entities.Contains(A));
			CHECK_FALSE(Entities.Contains(B));
			CHECK(Entities.Contains(C));
			CHECK(Storage.Num() == 2);
		}
	}
}
