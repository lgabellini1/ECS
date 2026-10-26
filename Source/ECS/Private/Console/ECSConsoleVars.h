#pragma once

#include "CoreMinimal.h"

namespace ECS
{
	extern TAutoConsoleVariable<int32> CVarEnableScreenDiagnostics;

	DECLARE_MULTICAST_DELEGATE(FOnCVarEnableScreenDiagnosticsChanged);
	inline FOnCVarEnableScreenDiagnosticsChanged CVarEnableScreenDiagnosticsChanged;
}
