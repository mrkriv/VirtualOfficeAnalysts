

#pragma once

#include "GameFramework/GameModeBase.h"
#include "ExtendGameMode.generated.h"

/**
 * 
 */
UCLASS()
class VR_API AExtendGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	static int server_ID;

public:
	AExtendGameMode();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Server ID"), Category = "Online")
	static int GetServerID();
	
	UFUNCTION(Exec, Category = "Console Command")
	static void SetServerID(int ID);
};
