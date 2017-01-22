#include "VR.h"
#include "JsonFieldData.h"

UJsonFieldData::UJsonFieldData()
{
	Reset();
}

UJsonFieldData* UJsonFieldData::Create(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);

	UJsonFieldData* fieldData = NewObject<UJsonFieldData>();
	fieldData->contextObject = WorldContextObject;
	return fieldData;
}

void UJsonFieldData::WriteObject(TSharedRef<TJsonWriter<TCHAR>> writer, FString key, FJsonValue* value)
{
	if (value->Type == EJson::Null)
	{
		if (key.Len() > 0)
			writer->WriteNull(key);
		else
			writer->WriteNull();
	}
	if (value->Type == EJson::String)
	{
		if (key.Len() > 0)
			writer->WriteValue(key, value->AsString());
		else
			writer->WriteValue(value->AsString());
	}
	if (value->Type == EJson::Boolean)
	{
		if (key.Len() > 0)
			writer->WriteValue(key, value->AsBool());
		else
			writer->WriteValue(value->AsBool());
	}
	if (value->Type == EJson::Number)
	{
		if (key.Len() > 0)
			writer->WriteValue(key, value->AsNumber());
		else
			writer->WriteValue(value->AsNumber());
	}
	else if (value->Type == EJson::Object)
	{
		if (key.Len() > 0)
			writer->WriteObjectStart(key);
		else
			writer->WriteObjectStart();

		TSharedPtr<FJsonObject> objectData = value->AsObject();
		for (auto objectValue = objectData->Values.CreateIterator(); objectValue; ++objectValue)
			WriteObject(writer, objectValue.Key(), objectValue.Value().Get());

		writer->WriteObjectEnd();
	}
	else if (value->Type == EJson::Array)
	{
		writer->WriteArrayStart(key);

		TArray<TSharedPtr<FJsonValue>> objectArray = value->AsArray();
		for (int32 i = 0; i < objectArray.Num(); i++)
			WriteObject(writer, "", objectArray[i].Get());

		writer->WriteArrayEnd();
	}
}

UJsonFieldData* UJsonFieldData::SetString(const FString& key, const FString& value)
{
	Data->SetStringField(*key, *value);
	return this;
}

UJsonFieldData* UJsonFieldData::SetBoolean(const FString& key, const bool value)
{
	Data->SetBoolField(*key, value);
	return this;
}

UJsonFieldData* UJsonFieldData::SetFloat(const FString& key, const float value)
{
	Data->SetNumberField(*key, static_cast<double>(value));
	return this;
}

UJsonFieldData* UJsonFieldData::SetInt(const FString& key, const int32 value)
{
	Data->SetNumberField(*key, static_cast<double>(value));
	return this;
}

UJsonFieldData* UJsonFieldData::SetVector(UObject* WorldContextObject, const FString& key, const FVector value)
{
    auto World = GEngine->GetWorldFromContextObject(WorldContextObject);
	auto vec = NewObject<UJsonFieldData>();

	vec->Data->SetNumberField("x", static_cast<double>(value.X));
	vec->Data->SetNumberField("y", static_cast<double>(value.Y));
	vec->Data->SetNumberField("z", static_cast<double>(value.Z));

	Data->SetObjectField(*key, vec->Data);
	return this;
}

UJsonFieldData* UJsonFieldData::SetVector2D(UObject* WorldContextObject, const FString& key, const FVector2D value)
{
    auto World = GEngine->GetWorldFromContextObject(WorldContextObject);
	auto vec = NewObject<UJsonFieldData>();

	vec->Data->SetNumberField("x", static_cast<double>(value.X));
	vec->Data->SetNumberField("y", static_cast<double>(value.Y));
	
	Data->SetObjectField(*key, vec->Data);

	return this;
}

UJsonFieldData* UJsonFieldData::SetNull(const FString& key)
{
	Data->SetObjectField(*key, NULL);
	return this;
}

UJsonFieldData* UJsonFieldData::SetObject(const FString& key, const UJsonFieldData* objectData)
{
	Data->SetObjectField(*key, objectData->Data);
	return this;
}

UJsonFieldData* UJsonFieldData::SetObjectArray(const FString& key, const TArray<UJsonFieldData*> objectData)
{
	TArray<TSharedPtr<FJsonValue>> *dataArray = new TArray<TSharedPtr<FJsonValue>>();

	for (int32 i = 0; i < objectData.Num(); i++)
		dataArray->Add(MakeShareable(new FJsonValueObject(objectData[i]->Data)));

	Data->SetArrayField(*key, *dataArray);
	return this;
}

UJsonFieldData* UJsonFieldData::SetStringArray(const FString& key, const TArray<FString> stringData)
{
	TArray<TSharedPtr<FJsonValue>> *dataArray = new TArray<TSharedPtr<FJsonValue>>();

	for (int32 i = 0; i < stringData.Num(); i++)
		dataArray->Add(MakeShareable(new FJsonValueString(stringData[i])));

	Data->SetArrayField(*key, *dataArray);
	return this;
}

UJsonFieldData* UJsonFieldData::SetBoolArray(const FString& key, const TArray<bool> data)
{
	TArray<TSharedPtr<FJsonValue>> *dataArray = new TArray<TSharedPtr<FJsonValue>>();

	for (int32 i = 0; i < data.Num(); i++)
		dataArray->Add(MakeShareable(new FJsonValueBoolean(data[i])));

	Data->SetArrayField(*key, *dataArray);
	return this;
}

UJsonFieldData* UJsonFieldData::SetFloatArray(const FString& key, const TArray<float> data)
{
	TArray<TSharedPtr<FJsonValue>> *dataArray = new TArray<TSharedPtr<FJsonValue>>();

	for (int32 i = 0; i < data.Num(); i++)
		dataArray->Add(MakeShareable(new FJsonValueNumber(static_cast<double>(data[i]))));

	Data->SetArrayField(*key, *dataArray);
	return this;
}

UJsonFieldData* UJsonFieldData::SetIntArray(const FString& key, const TArray<int32> data)
{
	TArray<TSharedPtr<FJsonValue>> *dataArray = new TArray<TSharedPtr<FJsonValue>>();

	for (int32 i = 0; i < data.Num(); i++)
		dataArray->Add(MakeShareable(new FJsonValueNumber(static_cast<double>(data[i]))));

	Data->SetArrayField(*key, *dataArray);
	return this;
}

UJsonFieldData* UJsonFieldData::SetNullArray(const FString& key, const int32& length)
{
	TArray<TSharedPtr<FJsonValue>> *dataArray = new TArray<TSharedPtr<FJsonValue>>();

	for (int32 i = 0; i < length; i++)
		dataArray->Add(MakeShareable(new FJsonValueNull()));

	Data->SetArrayField(*key, *dataArray);
	return this;
}

UJsonFieldData* UJsonFieldData::GetObject(const FString& key, bool& success)
{
	UJsonFieldData* fieldObj = NULL;

	const TSharedPtr<FJsonObject> *outPtr;
	if (!Data->TryGetObjectField(*key, outPtr))
	{
		UE_LOG(LogJson, Error, TEXT("Entry '%s' not found in the field data!"), *key);
		success = false;
		return NULL;
	}

	fieldObj = UJsonFieldData::Create(contextObject);
	fieldObj->Data = *outPtr;

	success = true;
	return fieldObj;
}

TArray<FString> UJsonFieldData::GetStringArray(const FString& key, bool& success)
{
	TArray<FString> stringArray;

	const TArray<TSharedPtr<FJsonValue>> *arrayPtr;
	if (Data->TryGetArrayField(*key, arrayPtr))
	{
		for (int32 i = 0; i < arrayPtr->Num(); i++)
			stringArray.Add((*arrayPtr)[i]->AsString());
		success = true;
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("Array entry '%s' not found in the field data!"), *key);
		success = false;
	}

	return stringArray;
}

TArray<bool> UJsonFieldData::GetBoolArray(const FString& key, bool& success)
{
	TArray<bool> array;

	const TArray<TSharedPtr<FJsonValue>> *arrayPtr;
	if (Data->TryGetArrayField(*key, arrayPtr))
	{
		for (int32 i = 0; i < arrayPtr->Num(); i++)
			array.Add((*arrayPtr)[i]->AsBool());
		success = true;
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("Array entry '%s' not found in the field data!"), *key);
		success = false;
	}

	return array;
}

TArray<int32> UJsonFieldData::GetIntArray(const FString& key, bool& success)
{
	TArray<int32> array;

	const TArray<TSharedPtr<FJsonValue>> *arrayPtr;
	if (Data->TryGetArrayField(*key, arrayPtr))
	{
		for (int32 i = 0; i < arrayPtr->Num(); i++)
			array.Add(static_cast<int32>((*arrayPtr)[i]->AsNumber()));
		success = true;
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("Array entry '%s' not found in the field data!"), *key);
		success = false;
	}

	return array;
}

TArray<float> UJsonFieldData::GetFloatArray(const FString& key, bool& success)
{
	TArray<float> array;
	const TArray<TSharedPtr<FJsonValue>> *arrayPtr;
	if (Data->TryGetArrayField(*key, arrayPtr))
	{
		for (int32 i = 0; i < arrayPtr->Num(); i++)
			array.Add(static_cast<float>((*arrayPtr)[i]->AsNumber()));
		success = true;
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("Array entry '%s' not found in the field data!"), *key);
		success = false;
	}

	return array;
}

TArray<UJsonFieldData*> UJsonFieldData::GetObjectArray(UObject* WorldContextObject, const FString& key, bool& success)
{
	TArray<UJsonFieldData*> objectArray;

	const TArray<TSharedPtr<FJsonValue>> *arrayPtr;
	if (Data->TryGetArrayField(*key, arrayPtr))
	{
		for (int32 i = 0; i < arrayPtr->Num(); i++)
		{
			UJsonFieldData* pageData = Create(WorldContextObject);
			pageData->Data = (*arrayPtr)[i]->AsObject();
			objectArray.Add(pageData);
		}
		success = true;
	}
	else
	{
		UE_LOG(LogJson, Error, TEXT("Array entry '%s' not found in the field data!"), *key);
		success = false;
	}

	return objectArray;
}

TArray<FString> UJsonFieldData::GetObjectKeys(UObject* WorldContextObject)
{
	TArray<FString> stringArray;

	for (auto currJsonValue = Data->Values.CreateConstIterator(); currJsonValue; ++currJsonValue)
	{
		stringArray.Add((*currJsonValue).Key);
	}

	return stringArray;
}

FString UJsonFieldData::GetString(const FString& key, bool& success) const
{
	FString outString;

	if (!Data->TryGetStringField(*key, outString))
	{
		UE_LOG(LogJson, Error, TEXT("String entry '%s' not found in the field data!"), *key);
		success = false;
		return "";
	}

	success = true;
	return outString;
}

bool UJsonFieldData::GetBool(const FString& key, bool& success) const
{
	bool value;

	if (!Data->TryGetBoolField(*key, value))
	{
		UE_LOG(LogJson, Error, TEXT("Boolean entry '%s' not found in the field data!"), *key);
		success = false;
		return false;
	}

	success = true;
	return value;
}

int32 UJsonFieldData::GetInt(const FString& key, bool& success) const
{
	int32 value;

	if (!Data->TryGetNumberField(*key, value))
	{
		UE_LOG(LogJson, Error, TEXT("Number entry '%s' not found in the field data!"), *key);
		success = false;
		return 0;
	}

	success = true;
	return value;
}

float UJsonFieldData::GetFloat(const FString& key, bool& success) const
{
	double value;

	if (!Data->TryGetNumberField(*key, value))
	{
		UE_LOG(LogJson, Error, TEXT("Number entry '%s' not found in the field data!"), *key);
		success = false;
		return 0.0f;
	}

	success = true;
	return static_cast<float>(value);
}

FVector UJsonFieldData::GetVector(const FString& key, bool& success)
{
	auto vec = GetObject(key, success);
	float x = vec->GetFloat("x", success);
	float y = vec->GetFloat("y", success);
	float z = vec->GetFloat("z", success);

	return FVector(x, y, z);
}

FVector2D UJsonFieldData::GetVector2D(const FString& key, bool& success)
{
	auto vec = GetObject(key, success);
	float x = vec->GetFloat("x", success);
	float y = vec->GetFloat("y", success);
	
	return FVector2D(x, y);
}

bool UJsonFieldData::GetIsNull(const FString& key, bool& success) const
{
	if (!Data->HasField(key))
	{
		UE_LOG(LogJson, Error, TEXT("Number entry '%s' not found in the field data!"), *key);
		success = false;
		return false;
	}

	success = true;
	return Data->HasTypedField<EJson::Null>(key);
}

void UJsonFieldData::Reset()
{
	if (Data.IsValid())
		Data.Reset();

	Data = MakeShareable(new FJsonObject());
}

bool UJsonFieldData::FromString(const FString& dataString)
{
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(dataString);

	// Deserialize the JSON data
	bool isDeserialized = FJsonSerializer::Deserialize(JsonReader, Data);

	if (!isDeserialized || !Data.IsValid())
	{
		UE_LOG(LogJson, Error, TEXT("JSON data is invalid! Input:\n'%s'"), *dataString);
		return false;
	}

	return true;
}

bool UJsonFieldData::FromFile(const FString& FilePath) {

	FString Result;
	FString FullJsonPath = FPaths::ConvertRelativePathToFull(FPaths::GameContentDir() / FilePath);
	if (!FFileHelper::LoadFileToString(Result, *FullJsonPath))
	{
		UE_LOG(LogJson, Error, TEXT("Can't load json data from %s"), *FilePath);
		return false;
	}
	return FromString(Result);
}

FString UJsonFieldData::ToString()
{
	FString outStr;
	TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&outStr);

	// Start writing the response
	WriteObject(JsonWriter, "", new FJsonValueObject(Data));
	JsonWriter->Close();

	return outStr;
}

bool UJsonFieldData::HasField(const FString& key)
{
	return Data->HasField(key);
}