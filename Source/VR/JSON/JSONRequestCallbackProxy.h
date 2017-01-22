#pragma once

#include "Net/OnlineBlueprintCallProxyBase.h"
#include "JsonFieldData.h"
#include "JSONRequestCallbackProxy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBlueprintJSONRequestResultDelegate, UJsonFieldData*, Json, bool, Success);

DECLARE_LOG_CATEGORY_EXTERN(JSONRequest, Log, All);

UCLASS(BlueprintType, Blueprintable, Category = "JSON")
class VR_API UJSONRequestCallbackProxy : public UOnlineBlueprintCallProxyBase
{
	GENERATED_BODY()
	
private:
	UObject* WorldContextObject;
	TWeakObjectPtr<UJsonFieldData> JsonRequestObject;
	TWeakObjectPtr<UJsonFieldData> JsonResultObject;
	FString URL;

	static FString Host;
	static FString Port;
	static FString Postfix;
	static FString UserAgent;
	static bool IsPrintToLog;

	void OnCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	UPROPERTY(BlueprintAssignable)
	FBlueprintJSONRequestResultDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FBlueprintJSONRequestResultDelegate OnFailure;

	UFUNCTION(BlueprintCallable, Category = "JSON|Request")
	static void SetJSONServerHost(const FString& host);
	
	UFUNCTION(BlueprintCallable, Category = "JSON|Request")
	static void SetJSONServerPort(const FString& port);
	
	UFUNCTION(BlueprintCallable, Category = "JSON|Request")
	static void SetJSONServerPostfix(const FString& postfix);
	
	UFUNCTION(BlueprintCallable, Category = "JSON|Request")
	static void SetJSONUserAgent(const FString& userAgent);
	
	UFUNCTION(BlueprintCallable, Category = "JSON|Request")
	static void SetJSONPrintToLog(const bool isPrintToLog);
	
	UFUNCTION(BlueprintPure, Category = "JSON|Request")
	static FString GetJSONServerUrl();

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"), Category = "JSON|Request")
	static UJSONRequestCallbackProxy* JSONGetRequest(UObject* WorldContextObject, const FString& url);

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"), Category = "JSON|Request")
	static UJSONRequestCallbackProxy* JSONPostRequest(UObject* WorldContextObject, UJsonFieldData* json, const FString& url);

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"), Category = "JSON|Request")
	static UJSONRequestCallbackProxy* JSONServerGetRequest(UObject* WorldContextObject, const FString& request);

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = "true", WorldContext="WorldContextObject"), Category = "JSON|Request")
	static UJSONRequestCallbackProxy* JSONServerPostRequest(UObject* WorldContextObject, UJsonFieldData* json, const FString& request);

	virtual void Activate() override;
};
