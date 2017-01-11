

using UnrealBuildTool;

public class VR : ModuleRules
{
	public VR(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Voice",
            "HTTP",
            "CoreUObject",
            "Engine",
            "Json",
            "OnlineSubsystem",
            "OnlineSubsystemUtils" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
