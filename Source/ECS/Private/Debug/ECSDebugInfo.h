#pragma once

namespace ECS
{
	class FRegistry;
	
	struct FComponentDebugInfo
	{
		FString Name;
		int32   Count;
		SIZE_T  Size;
	};

	TArray<FComponentDebugInfo> ScrapeDebugInfo(const FRegistry&);

	FString ConstructDebugMessage(const FRegistry&);
}
