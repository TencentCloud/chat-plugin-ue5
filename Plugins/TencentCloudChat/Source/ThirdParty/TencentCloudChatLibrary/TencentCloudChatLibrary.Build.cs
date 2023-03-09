// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class TencentCloudChatLibrary : ModuleRules
{
	public TencentCloudChatLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			// PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "x64", "Release", "ExampleLibrary.lib"));

			// // Delay-load the DLL, so we can load it from the right place first
			// PublicDelayLoadDLLs.Add("ExampleLibrary.dll");

			// // Ensure that the DLL is staged along with the executable
			// RuntimeDependencies.Add("$(PluginDir)/Binaries/ThirdParty/TencentCloudChatLibrary/Win64/ExampleLibrary.dll");
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            // PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libExampleLibrary.dylib"));
            // RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/TencentCloudChatLibrary/Mac/Release/libExampleLibrary.dylib");
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			// string ExampleSoPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "TencentCloudChatLibrary", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
			// PublicAdditionalLibraries.Add(ExampleSoPath);
			// PublicDelayLoadDLLs.Add(ExampleSoPath);
			// RuntimeDependencies.Add(ExampleSoPath);
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
        {   
            PublicAdditionalFrameworks.Add(new Framework("ImSDK_CPP",Path.Combine(ModuleDirectory, "iOS", "ImSDK.embeddedframework.zip"), "Frameworks", true));

            PublicIncludePaths.AddRange(
                new string[]{
                    Path.Combine(ModuleDirectory, "Includes")
                }
            );

        }else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			PublicIncludePaths.AddRange(
                new string[]{
                    Path.Combine(ModuleDirectory, "Android","ImSDK_Android_CPP","include")
                }
            );

			PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "APL_TencentCloudChat.xml"));


            string[] archs = { "armeabi-v7a", "arm64-v8a", "x86", "x86_64" };
			
            for (int i = 0; i < archs.Length; i++)
            {
                string AndroidSOPath = Path.Combine(ModuleDirectory, "Android", "ImSDK_Android_CPP", "libs", archs[i], "libImSDK.so");

                PublicAdditionalLibraries.Add(AndroidSOPath);
            }
		}
	}
}
