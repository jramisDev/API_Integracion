// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class API : ModuleRules
{
	public API(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemEOS", "OnlineSubsystemUtils", "UMG" });
		
		PublicIncludePaths.Add(ModuleDirectory + "/Public");
	}
}
