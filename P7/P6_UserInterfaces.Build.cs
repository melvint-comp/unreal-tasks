// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class P6_UserInterfaces : ModuleRules
{
	public P6_UserInterfaces(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate", 
			"NavigationSystem", 
			"AIModule", 
			"GameplayTasks"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"P6_UserInterfaces",
			"P6_UserInterfaces/Variant_Platforming",
			"P6_UserInterfaces/Variant_Platforming/Animation",
			"P6_UserInterfaces/Variant_Combat",
			"P6_UserInterfaces/Variant_Combat/AI",
			"P6_UserInterfaces/Variant_Combat/Animation",
			"P6_UserInterfaces/Variant_Combat/Gameplay",
			"P6_UserInterfaces/Variant_Combat/Interfaces",
			"P6_UserInterfaces/Variant_Combat/UI",
			"P6_UserInterfaces/Variant_SideScrolling",
			"P6_UserInterfaces/Variant_SideScrolling/AI",
			"P6_UserInterfaces/Variant_SideScrolling/Gameplay",
			"P6_UserInterfaces/Variant_SideScrolling/Interfaces",
			"P6_UserInterfaces/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
