// Copyright (c) 2025 Xist.GG LLC

using UnrealBuildTool;
using System.Collections.Generic;

public class XistGameEditorTarget : TargetRules
{
	public XistGameEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		ExtraModuleNames.Add("XistGame");
	}
}
