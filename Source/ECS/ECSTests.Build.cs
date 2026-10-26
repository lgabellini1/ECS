using UnrealBuildTool;

public class ECSTests : TestModuleRules
{
	public ECSTests(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new[] { "Core", "ECS" });
	}
}
