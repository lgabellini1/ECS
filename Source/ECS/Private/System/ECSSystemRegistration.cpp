#include "System/ECSSystemRegistration.h"

namespace ECS
{
	using FProviderList = TArray<TPair<FDelegateHandle, FSystemsProvider>>;
	
	FProviderList& Providers()
	{
		static FProviderList Providers;
		return Providers;
	}
	
	FDelegateHandle RegisterSystemsProvider(FSystemsProvider Provider)
	{
		FDelegateHandle Handle(FDelegateHandle::GenerateNewHandle);
		Providers().Emplace(Handle, MoveTemp(Provider));
		return Handle;
	}

	void UnregisterSystemsProvider(FDelegateHandle Handle)
	{
		Providers().RemoveAll([&](const auto& Provider)
		{
			return Provider.Key == Handle;
		});
	}

	void ForEachProvider(TFunctionRef<void(const FSystemsProvider&)> Fn)
	{
		for (auto& Provider : Providers())
		{
			Fn(Provider.Value);
		}
	}
}
