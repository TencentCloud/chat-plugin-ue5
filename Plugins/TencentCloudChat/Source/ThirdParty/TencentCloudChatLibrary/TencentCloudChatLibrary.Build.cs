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
            PublicIncludePaths.AddRange(
                new string[]{
                    Path.Combine(ModuleDirectory, "Windows", "ImSDK_Windows_CPP", "include")
                }
            );
			// Add the import library
			PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Windows", "ImSDK_Windows_CPP", "shared_lib","Win64","ImSDK.lib"));

			// // Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("ImSDK.dll");

			// // Ensure that the DLL is staged along with the executable
			RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Windows", "ImSDK_Windows_CPP", "shared_lib","Win64","ImSDK.dll"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicIncludePaths.AddRange(
                new string[]{
                    Path.Combine(ModuleDirectory, "Includes")
                }
            );
            PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac",  "libImSDKForMac_CPP.dylib"));
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Mac",  "libImSDKForMac_CPP.dylib"));
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
