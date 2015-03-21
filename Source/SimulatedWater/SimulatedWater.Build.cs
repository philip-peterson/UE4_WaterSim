// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;

public class SimulatedWater : ModuleRules
{
	public SimulatedWater(TargetInfo Target)
    {
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
        LoadMyLIB(Target);
	}

    private string ModulePath
    {
        get
        {
            return Path.GetDirectoryName(RulesCompiler.GetModuleFilename(this.GetType().Name));
        }
    }

    private string ThirdPartyPath
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/"));
        }
    }

    public bool LoadMyLIB(TargetInfo Target)
    {
        /*
        bool isLibrarySupported = false;

        
        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;
  
            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "LAPACK", "Libraries");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "LAPACK." + PlatformString + ".lib"));
        }

        if (isLibrarySupported)
        {
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "LAPACK", "Includes"));
        }*/

        string LibrariesPath = Path.Combine(ThirdPartyPath, "LAPACK", "Libraries");
        PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "LAPACK", "Includes"));

        PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "liblapacke.x86.lib"));

        Definitions.Add("WITH_LAPACK_BINDING=1");

        return true;
    }
}
