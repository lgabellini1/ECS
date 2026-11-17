#pragma once

namespace ECS
{
	class FRegistry;
	
	class FCommandBuffer
	{
	public:
		template <component T>
		void Add(FEntity Entity, T Comp = T());

		template <component T>
		void Remove(FEntity Entity);

		void Destroy(FEntity Entity);

		void Flush(FRegistry& Registry);
		
	private:
		using FCommand = TFunction<void(FRegistry&)>;
		TArray<FCommand> Commands;
	};
}
