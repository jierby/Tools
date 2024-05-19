// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MenuTool : ModuleRules
{
	public MenuTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {/*"OpenDataManagerCore"*/
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core","WebBrowser","Persona","AssetTools","ContentBrowser", "OpenDataManagerCore","UMG","Blutility", "AnimGraphRuntime","DesktopPlatform",
                "UnrealEd","UMGEditor","PropertyEditor","Slate"//"ControlRig"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore", "AnimGraphRuntime","OpenDataManagerCore","EditorScriptingUtilities", "SequenceRecorder","RHI","RenderCore", "EditorStyle", "EditorStyle"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
