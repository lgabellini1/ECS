#pragma once

#include "ECSRegistry.h"
#include "ECSTypes.h"

namespace ECS
{
	class FScheduler
	{
	public:
		FScheduler() = default;
		FScheduler(const FScheduler&) = delete;
		FScheduler& operator=(const FScheduler&) = delete;

		template <typename TSystem, typename... TArgs>
		TSystem& Add(TArgs&&... Args);
		
		void Update(FRegistry& Registry, float DeltaTime);

	private:
		TArray<TUniquePtr<ISystem>> Systems;
	};

	template <typename TSystem, typename... TArgs>
	TSystem& FScheduler::Add(TArgs&&... Args)
	{
		TUniquePtr<ISystem> System = MakeUnique<TSystem>(Forward<TArgs>(Args)...);

		return static_cast<TSystem&>(
			*Systems.Add_GetRef(MoveTemp(System))
		);
	}

	inline void FScheduler::Update(FRegistry& Registry, float DeltaTime)
	{
		for (const auto& System : Systems)
		{
			System->Update(Registry, DeltaTime);
			Registry.FlushCmds(); // Each system sees the world as updated by the previous.
			// Alternatively, one could flush between frames for performance, losing some consistency.
		}
	}
}
