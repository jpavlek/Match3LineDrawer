// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Match3LineDrawer : ModuleRules
{
	public Match3LineDrawer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
