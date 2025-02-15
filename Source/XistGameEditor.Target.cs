// Copyright (c) 2025 Xist.GG LLC

using UnrealBuildTool;
using System.Collections.Generic;

public class XistGameEditorTarget : TargetRules
{
	public XistGameEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("XistGame");
	}
}
