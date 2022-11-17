// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class TencentCloudIMLibrary : ModuleRules
{
	public TencentCloudIMLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;




        if (Target.Platform != UnrealTargetPlatform.Win64){
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Android", "ImSDK_Android_CPP", "include"));
        }

		if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string includePath = Path.Combine(ModuleDirectory, "Windows", "ImSDK_Windows_CPP", "include");
            string DllPath = Path.Combine(ModuleDirectory, "Windows", "ImSDK_Windows_CPP", "lib", "Win64", "ImSDK.dll");
            string LibPath = Path.Combine(ModuleDirectory, "Windows", "ImSDK_Windows_CPP", "lib", "Win64", "ImSDK.lib");

            PublicIncludePaths.Add(includePath);
            // Add the import library
            PublicAdditionalLibraries.Add(LibPath);
            // PublicAdditionalLibraries.Add(DllPath);
            // Ensure that the DLL is staged along with the executable
            // PublicDelayLoadDLLs.Add(DllPath);
            // RuntimeDependencies.Add(DllPath,StagedFileType.UFS);
            PublicDelayLoadDLLs.Add(DllPath);

            RuntimeDependencies.Add("$(BinaryOutputDir)/ImSDK.dll", DllPath);


        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicFrameworks.Add(Path.Combine(ModuleDirectory, "Mac", "ImSDKForMac_CPP.framework"));

            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Mac", "ImSDKForMac_CPP.framework/Versions/A/ImSDKForMac_CPP"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			string ExampleSoPath = Path.Combine("$(PluginDir)", "Binaries", "ThirdParty", "TencentCloudIMLibrary", "Linux", "x86_64-unknown-linux-gnu", "libExampleLibrary.so");
			PublicAdditionalLibraries.Add(ExampleSoPath);
			PublicDelayLoadDLLs.Add(ExampleSoPath);
			RuntimeDependencies.Add(ExampleSoPath);
		}
		else if (Target.Platform == UnrealTargetPlatform.IOS)
        {


            // PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "IOS", "ImSDK_CPP.framework/ImSDK_CPP"));

            // RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "IOS", "ImSDK_CPP.framework/Info.plist"));

            // RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "IOS", "ImSDK_CPP.framework/ImSDK_CPP"));

            Framework MacFramework = new Framework(
                "ImSDK_CPP",
                Path.Combine(ModuleDirectory, "IOS", "ImSDK_CPP.framework.zip"),
                "ImSDK_CPP"
            );

            PublicAdditionalFrameworks.Add(
                MacFramework
            );

            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "IOS", "ImSDK_CPP.framework/ImSDK_CPP"));
            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "IOS", "ImSDK_CPP.framework/Info.plist"));

        }
		else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "Android", "ImSDK_Android_CPP", "APL_armv7.xml"));


            string[] archs = { "armeabi-v7a", "arm64-v8a", "x86", "x86_64" };
			
            for (int i = 0; i < archs.Length; i++)
            {
                string AndroidSOPath = Path.Combine(ModuleDirectory, "Android", "ImSDK_Android_CPP", "libs", archs[i], "libImSDK.so");
                PublicAdditionalLibraries.Add(AndroidSOPath);
            }
            

        }
	}
}
