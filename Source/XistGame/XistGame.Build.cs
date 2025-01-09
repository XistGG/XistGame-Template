// Copyright (c) 2025 Xist.GG LLC

using UnrealBuildTool;

public class XistGame : ModuleRules
{
	public XistGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "Niagara" });
	}
}
