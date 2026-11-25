#pragma	once
#include "Storage/ECSComponentOps.h"
#include "Registry/ECSRegistry.h"

// This file contains functionalities for bridging user-defined components
// with the UTH-compatible FInstancedStruct, allowing plugin's users to add components in editor.

namespace ECS
{
	static FComponentOps OpsFor(const UScriptStruct* T)
	{
		return {
			T->GetStructureSize(), T->GetMinAlignment(),
			T,
			[](const void* Component, void* Dest)
			{
				static_cast<const UScriptStruct*>(Component)->InitializeStruct(Dest);
			},
			[](const void* Component, void* Dest, const void* Src)
			{
				static_cast<const UScriptStruct*>(Component)->CopyScriptStruct(Dest, Src);
			},
			[](const void* Component, void* Ptr)
			{
				static_cast<const UScriptStruct*>(Component)->DestroyStruct(Ptr);
			},
			nullptr
		};
	}

	inline FTypeErasedStorage& EnsureStorage(FRegistry& Registry, const UScriptStruct* T)
	{
		return static_cast<FTypeErasedStorage&>(
			Registry.GetOrCreateStorage(FComponentKey::FromStruct(T),
				[T]() -> TUniquePtr<IStorage> { return MakeUnique<FTypeErasedStorage>(OpsFor(T)); }
			)
		);	
	}
}
