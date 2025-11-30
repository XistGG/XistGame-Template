// Copyright (c) 2025 Xist.GG LLC

using UnrealBuildTool;

public class XistGame : ModuleRules
{
	public XistGame(ReadOnlyTargetRules Target) : base(Target)
	{
		// Add XISTGAME DEBUG code in DebugGame
		int withXistGameDebug = Target.Configuration == UnrealTargetConfiguration.DebugGame ? 1 : 0;
		PublicDefinitions.Add("WITH_XISTGAME_DEBUG="+withXistGameDebug);

		// When not in DebugGame configuration, use Unity builds for faster build speed.
		// When in DebugGame, DISABLE Unity builds to ensure devs always include ALL headers required by cpp source.
		bUseUnity = withXistGameDebug == 0;

		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "XistGame" });

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "Niagara" });
	}
}
