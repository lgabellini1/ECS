#pragma once

namespace ECS
{
	struct FComponentOps
	{
		int32 Size, Align;
		const void* Ctx;
		
		void (*Construct)(const void* Ctx, void* Dst);
		void (*Assign)   (const void* Ctx, void* Dst, const void* Src);
		void (*Destroy)  (const void* Ctx, void* Ptr);
		void (*Move)     (const void* Ctx, void* Dst, void* Src);
	};

	template <typename T>
	FComponentOps MakeOps()
	{
		return FComponentOps(
			sizeof(T), alignof(T),
			nullptr,
			[](const void*, void* Dst)
			{
				new (Dst) T();
			},
			[](const void*, void* Dst, const void* Src)
			{
				*static_cast<T*>(Dst) = *static_cast<const T*>(Src);
			},
			[](const void*, void* Ptr)
			{
				static_cast<T*>(Ptr)->~T();
			},
			[](const void*, void* Dst, void* Src)
			{
				*static_cast<T*>(Dst) = MoveTemp(*static_cast<T*>(Src));
			}
		);
	}
}
