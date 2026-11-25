#include "ECSDebugInfo.h"
#include "Registry/ECSRegistry.h"

namespace ECS
{
	TArray<FComponentDebugInfo> ScrapeDebugInfo(const FRegistry& Registry)
	{
		TArray<FComponentDebugInfo> DebugInfo;
		
		Registry.ForEachStorage([&](const FComponentKey& Key, const IStorage& Storage)
		{
			FString Name;
			if (Key.Kind == FComponentKey::EKind::Reflected)
				Name = Key.Struct ? Key.Struct->GetName() : TEXT("<null struct>");
			else
				Name = FString::Printf(TEXT("Native#%u"), Key.Id);
			DebugInfo.Add({ Name, Storage.Num(), Storage.Size() });
		});

		return DebugInfo;
	}

	SIZE_T TotalMemoryFootprint(const TArray<FComponentDebugInfo>& DebugInfo)
	{
		SIZE_T Memory = 0;
		for (auto& Info : DebugInfo)
		{
			Memory += Info.Size;
		}
		return Memory;
	}

	FString ConstructDebugMessage(const FRegistry& Registry)
	{
		const auto& DebugInfo = ScrapeDebugInfo(Registry);
		
		FString Msg = FString::Printf(TEXT("ENTITY COMPONENT SYSTEM\n")
			TEXT("Num entities: %i\n")
			TEXT("Components:\n"), Registry.NumEntities());
		
		SIZE_T Memory = 0;
		for (const auto& Info : DebugInfo)
		{
			Msg += FString::Printf(TEXT("	%6d  %8.1f KB  %s\n"),
				Info.Count, Info.Size / 1024.0, *Info.Name);
			Memory += Info.Size;
		}

		Msg += FString::Printf(
			TEXT("Total memory usage: %.1f KB\n")
			TEXT("-----------------------------------"),
			Memory / 1024.0);

		return Msg;
	}
}
