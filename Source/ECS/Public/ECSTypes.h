#pragma once

namespace ECS
{
	class FRegistry;
	
	template <typename T>
	concept component = std::is_trivially_copyable_v<T> && std::default_initializable<T> &&
		std::is_move_assignable_v<T> && (!std::is_const_v<T>) && (!std::is_reference_v<T>);

	class ECS_API FEntity
	{
	public:
		FEntity();
		constexpr explicit FEntity(uint32);
		FEntity(uint32 Index, uint32 Generation);

		bool operator==(FEntity Other) const { return Id == Other.Id; }
		
		uint32 Index() const;
		uint32 Generation() const;

		bool IsNull() const;

		static const FEntity Null;
	private:
		uint32 Id;
	};

	static_assert(sizeof(FEntity) == 4);

	class ECS_API ISystem
	{
	public:
		virtual ~ISystem() = default;
		virtual void Update(FRegistry& Registry, float DeltaTime) = 0;
	};
}
