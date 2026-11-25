#pragma once
#include "ECSUtils.h"
#include "ECSSparseSet.h"

class UScriptStruct;

namespace ECS
{
	class FEntity;
	
	class ECS_API IStorage
	{
	public:
		virtual ~IStorage() = default;
		virtual void Remove(FEntity) = 0;
	};

	struct FComponentKey
	{
		enum class EKind : uint8 { Reflected, Native };
		EKind Kind;
		
		const UScriptStruct* Struct;
		uint32				 Id;

		static FComponentKey FromStruct(const UScriptStruct* S) { return { EKind::Reflected, S, 0 }; }
		static FComponentKey FromId(uint32 I)                   { return { EKind::Native, nullptr, I }; }

		friend bool operator==(const FComponentKey& A, const FComponentKey& B)
		{
			return A.Kind == B.Kind && A.Struct == B.Struct && A.Id == B.Id;
		}

		friend uint32 GetTypeHash(const FComponentKey& K)
		{
			return HashCombine(::GetTypeHash(static_cast<uint8>(K.Kind)),
				   HashCombine(::GetTypeHash(K.Struct), ::GetTypeHash(K.Id)));
		}
	};
	
	template <typename T>
	FComponentKey KeyOf()
	{
		if constexpr (requires { T::StaticStruct(); })
		{
			return FComponentKey::FromStruct(T::StaticStruct());
		}
		else
		{
			return FComponentKey::FromId(TypeId<T>());
		}
	}
}
