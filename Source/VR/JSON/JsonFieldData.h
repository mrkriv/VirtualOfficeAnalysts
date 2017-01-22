#pragma once

#include "JsonFieldData.generated.h"

UCLASS(BlueprintType, Blueprintable, Category = "JSON")
class UJsonFieldData : public UObject
{
	GENERATED_BODY()

private:
	void Reset();

	void WriteObject(TSharedRef<TJsonWriter<TCHAR>> writer, FString key, FJsonValue* value);

public:
	UObject* contextObject;

	TSharedPtr<FJsonObject> Data;

	UJsonFieldData();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "JSON To String", CompactNodeTitle = "->", Keywords = "cast text convert serialize"), Category = "JSON")
	FString ToString();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Check Field Exists"), Category = "JSON")
	bool HasField(const FString& key);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Create JSON", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "JSON")
	static UJsonFieldData* Create(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add String"), Category = "JSON")
	UJsonFieldData* SetString(const FString& key, const FString& value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Boolean"), Category = "JSON")
	UJsonFieldData* SetBoolean(const FString& key, const bool value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Float"), Category = "JSON")
	UJsonFieldData* SetFloat(const FString& key, const float value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Vector", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "JSON")
	UJsonFieldData* SetVector(UObject* WorldContextObject, const FString& key, const FVector value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Vector2D", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "JSON")
	UJsonFieldData* SetVector2D(UObject* WorldContextObject, const FString& key, const FVector2D value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Integer"), Category = "JSON")
	UJsonFieldData* SetInt(const FString& key, const int32 value);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Null"), Category = "JSON")
	UJsonFieldData* SetNull(const FString& key);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add String Array"), Category = "JSON")
	UJsonFieldData* SetStringArray(const FString& key, const TArray<FString> data);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Boolean Array"), Category = "JSON")
	UJsonFieldData* SetBoolArray(const FString& key, const TArray<bool> data);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Float Array"), Category = "JSON")
	UJsonFieldData* SetFloatArray(const FString& key, const TArray<float> data);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Integer Array"), Category = "JSON")
	UJsonFieldData* SetIntArray(const FString& key, const TArray<int32> data);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Null Array"), Category = "JSON")
	UJsonFieldData* SetNullArray(const FString& key, const int32& length);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Object"), Category = "JSON")
	UJsonFieldData* SetObject(const FString& key, const UJsonFieldData* objectData);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Add Object Array"), Category = "JSON")
	UJsonFieldData* SetObjectArray(const FString& key, const TArray<UJsonFieldData*> arrayData);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get String"), Category = "JSON")
	FString GetString(const FString& key, bool& success) const;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Boolean"), Category = "JSON")
	bool GetBool(const FString& key, bool& success) const;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Integer"), Category = "JSON")
	int32 GetInt(const FString& key, bool& success) const;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Float"), Category = "JSON")
	float GetFloat(const FString& key, bool& success) const;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Vector"), Category = "JSON")
	FVector GetVector(const FString& key, bool& success);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Vector2D"), Category = "JSON")
	FVector2D GetVector2D(const FString& key, bool& success);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Field Is Null"), Category = "JSON")
	bool GetIsNull(const FString& key, bool& fieldExists) const;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get String Array"), Category = "JSON")
	TArray<FString> GetStringArray(const FString& key, bool& success);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Boolean Array"), Category = "JSON")
	TArray<bool> GetBoolArray(const FString& key, bool& success);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Integer Array"), Category = "JSON")
	TArray<int32> GetIntArray(const FString& key, bool& success);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Float Array"), Category = "JSON")
	TArray<float> GetFloatArray(const FString& key, bool& success);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Object", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "JSON")
	UJsonFieldData* GetObject(const FString& key, bool& success);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Object Array", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "JSON")
	TArray<UJsonFieldData*> GetObjectArray(UObject* WorldContextObject, const FString& key, bool& success);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Object Keys", HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"), Category = "JSON")
	TArray<FString> GetObjectKeys(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "From String"), Category = "JSON")
	bool FromString(const FString& dataString);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "From File"), Category = "JSON")
	bool FromFile(const FString& FilePath);
};