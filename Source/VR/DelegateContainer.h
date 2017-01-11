#pragma once

#include "EngineDefines.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DelegateContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEvent);

UCLASS(BlueprintType, Blueprintable, meta = (ShortTooltip = "Simple callback proxy."))
class VR_API UDelegateContainer : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
	FOnEvent OnEvent;

	UFUNCTION(BlueprintCallable, Category = "Event")
	void InvokeEvent();

	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", DisplayName = "Create Delegate Container", CompactNodeTitle = "DC", Keywords = "new delegate container"), Category = Game)
	static UDelegateContainer* CreateContainer(UObject* WorldContextObject);
};
