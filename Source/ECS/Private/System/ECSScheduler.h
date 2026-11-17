#pragma once
#include "ECSTypes.h"

namespace ECS
{
	class ISystem;
	
	class FScheduler
	{
	public:
		FScheduler() = default;
		
		FScheduler(const FScheduler&)			 = delete;
		FScheduler& operator=(const FScheduler&) = delete;
		
		ISystem& Add(TUniquePtr<ISystem>);

		void Update(FRegistry& Registry, float DeltaTime)
		{
			for (const auto& System : Systems)
				System->Update(Registry, DeltaTime);
		}
	private:
		TArray<TUniquePtr<ISystem>> Systems;
	};
}
