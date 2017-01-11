#include "VR.h"
#include "JSONRequestCallbackProxy.h"

FString UJSONRequestCallbackProxy::Host = "localhost";
FString UJSONRequestCallbackProxy::Port = "80";
FString UJSONRequestCallbackProxy::Postfix = "";
FString UJSONRequestCallbackProxy::UserAgent = "UnrealEngine4Client/1.0";

UJSONRequestCallbackProxy*
UJSONRequestCallbackProxy::JSONGetRequest(UObject* WorldContextObject, const FString& url)
{
	UJSONRequestCallbackProxy* Proxy = NewObject<UJSONRequestCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->JsonRequestObject = nullptr;
	Proxy->URL = url;
	return Proxy;
}

UJSONRequestCallbackProxy*
UJSONRequestCallbackProxy::JSONPostRequest(UObject* WorldContextObject, UJsonFieldData* json, const FString& url)
{
	UJSONRequestCallbackProxy* Proxy = NewObject<UJSONRequestCallbackProxy>();
	Proxy->WorldContextObject = WorldContextObject;
	Proxy->JsonRequestObject = json;
	Proxy->URL = url;
	return Proxy;
}

UJSONRequestCallbackProxy*
UJSONRequestCallbackProxy::JSONServerGetRequest(UObject* WorldContextObject, const FString& request)
{
	FString url = GetJSONServerUrl() + request;
	return JSONGetRequest(WorldContextObject, url);
}

UJSONRequestCallbackProxy*
UJSONRequestCallbackProxy::JSONServerPostRequest(UObject* WorldContextObject, UJsonFieldData* json, const FString& request)
{
	FString url = GetJSONServerUrl() + request;
	return JSONPostRequest(WorldContextObject, json, url);
}

FString
UJSONRequestCallbackProxy::GetJSONServerUrl()
{
	return FString::Printf(TEXT("http://%s:%s/%s/"), *Host, *Port, *Postfix);
}

void UJSONRequestCallbackProxy::SetJSONServerHost(const FString& host)
{
	Host = host;
}

void UJSONRequestCallbackProxy::SetJSONServerPort(const FString& port)
{
	Port = port;
}

void UJSONRequestCallbackProxy::SetJSONServerPostfix(const FString& postfix)
{
	Postfix = postfix;
}

void UJSONRequestCallbackProxy::SetJSONUserAgent(const FString& userAgent)
{
	UserAgent = userAgent;
}

void UJSONRequestCallbackProxy::Activate()
{
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	JsonResultObject = NewObject<UJsonFieldData>();
	JsonResultObject->contextObject = WorldContextObject;

	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetHeader("User-Agent", UserAgent);
	HttpRequest->SetURL(URL);
	HttpRequest->SetHeader("Content-Type", "application/json");
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UJSONRequestCallbackProxy::OnCompleted);

	if(JsonRequestObject.IsValid())
	{
		HttpRequest->SetContentAsString(JsonRequestObject->ToString());
		HttpRequest->SetVerb("POST");
	}
	else
		HttpRequest->SetVerb("GET");

	JsonResultObject->AddToRoot();
	HttpRequest->ProcessRequest();
}

void UJSONRequestCallbackProxy::OnCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	JsonResultObject->RemoveFromRoot();

	if (!bWasSuccessful)
	{
		OnFailure.Broadcast(nullptr, false);
		return;
	}

	if (!JsonResultObject->FromString(Response->GetContentAsString()))
	{
		OnFailure.Broadcast(nullptr, false);
        return;
	}

	OnSuccess.Broadcast(JsonResultObject.Get(), true);
}