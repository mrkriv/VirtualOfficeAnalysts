#pragma once

#include "EngineDefines.h"
#include "File.h"
#include "Directory.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (ShortTooltip = "Directory info class."))
class VR_API UDirectory : public UObject
{
	GENERATED_BODY()

	FString path;
public:

	UDirectory();

	UFUNCTION(BlueprintPure, Category = "IO|Directory")
	FString GetName();

	UFUNCTION(BlueprintPure, Category = "IO|Directory")
	FString GetPath();

	UFUNCTION(BlueprintCallable, Category = "IO|Directory")
	TArray<UFile*> GetFiles(FString mask);

	UFUNCTION(BlueprintCallable, Category = "IO|Directory")
	TArray<UDirectory*> GetDirectories(FString mask);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Directory", CompactNodeTitle = "Directory"), Category = "IO | Directory")
	static UDirectory* GetDirectory(FString path);
};
