#include "VR.h"
#include "DelegateContainer.h"

void UDelegateContainer::InvokeEvent()
{
	OnEvent.Broadcast();
}

UDelegateContainer* UDelegateContainer::CreateContainer(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
	return NewObject<UDelegateContainer>(UDelegateContainer::StaticClass());
}	