#include "ECSConsoleVars.h"

namespace ECS
{
	TAutoConsoleVariable<int32> CVarEnableScreenDiagnostics(
		TEXT("r.ECS.EnableScreenDiagnostics"),
		1,
		TEXT("Show general ECS diagnostics on screen"),
		FConsoleVariableDelegate::CreateLambda(
			[](IConsoleVariable*)
			{
				CVarEnableScreenDiagnosticsChanged.Broadcast();
			})
	);
}