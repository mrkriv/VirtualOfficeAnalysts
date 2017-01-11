#pragma once

#include "EngineDefines.h"
#include "File.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (ShortTooltip = "File info class."))
class VR_API UFile : public UObject
{
	GENERATED_BODY()

	FString path;
public:

	UFile();

	UFUNCTION(BlueprintPure, Category = "IO|File")
	FString GetName();

	UFUNCTION(BlueprintPure, Category = "IO|File")
	FString GetPath();

	UFUNCTION(BlueprintPure, Category = "IO|File")
	int GetSize();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get File Info", CompactNodeTitle = "File"), Category = "IO|File")
	static UFile* GetFileInfo(FString path);
};
