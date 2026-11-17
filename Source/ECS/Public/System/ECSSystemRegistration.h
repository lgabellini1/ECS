#pragma once

namespace ECS
{
	class ISystem;

	class FSystemRegistrar
	{
	public:
		using FSink = TFunctionRef<ISystem&(TUniquePtr<ISystem>)>;
		
		explicit FSystemRegistrar(const FSink& InSink)
			: Sink(InSink)
		{}

		template <typename TSystem, typename... TArgs>
		TSystem& Add(TArgs&&... Args);
	private:
		FSink Sink;
	};

	DECLARE_DELEGATE_OneParam(FSystemsProvider, FSystemRegistrar&);

	FDelegateHandle RegisterSystemsProvider(FSystemsProvider);
	void UnregisterSystemsProvider(FDelegateHandle);
}

#include "ECSSystemRegistration.inl"
