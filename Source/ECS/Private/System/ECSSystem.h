#pragma once

namespace ECS
{
	class FRegistry;

	class FSystemContext
	{
	public:
		template <component T>
		T* Get(FEntity Entity) const;

		template <component T>
		bool Has(FEntity Entity) const;

		template <component T>
		TComponentStorage<T>* GetStorage() const;

	private:
		FRegistry Registry;
	};
}
