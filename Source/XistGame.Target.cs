// Copyright (c) 2025 Xist.GG LLC

using UnrealBuildTool;
using System.Collections.Generic;

public class XistGameTarget : TargetRules
{
	public XistGameTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("XistGame");
	}
}
