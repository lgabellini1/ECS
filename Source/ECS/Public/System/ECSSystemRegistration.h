#pragma once

namespace ECS
{
	class ISystem;

	class FSystemRegistrar
	{
	public:
		using FSink = TFunctionRef<ISystem&(TUniquePtr<ISystem>)>;
		
		explicit FSystemRegistrar(const FSink& InSink, UWorld* InWorld)
			: Sink(InSink), World(InWorld)
		{}

		template <typename TSystem, typename... TArgs>
		TSystem& Add(TArgs&&... Args);
	private:
		FSink   Sink;
		UWorld* World;
	};

	DECLARE_DELEGATE_OneParam(FSystemsProvider, FSystemRegistrar&);

	ECS_API FDelegateHandle RegisterSystemsProvider(FSystemsProvider);
	ECS_API void UnregisterSystemsProvider(FDelegateHandle);
	ECS_API void ForEachProvider(TFunctionRef<void(const FSystemsProvider&)> Fn);
}

#include "ECSSystemRegistration.inl"
