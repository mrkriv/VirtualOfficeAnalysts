

#include "VR.h"
#include "ExtendGameMode.h"

int AExtendGameMode::server_ID = 0;

AExtendGameMode::AExtendGameMode()
{
	int id;
	if (FParse::Value(FCommandLine::Get(), TEXT("ServerID"), id))
		server_ID = id;
}

int AExtendGameMode::GetServerID()
{
	return server_ID;
}

void AExtendGameMode::SetServerID(int ID)
{
	server_ID = ID;
}

