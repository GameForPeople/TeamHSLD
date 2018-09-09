#include "stdafx.h"

#include "IOCPServer/IOCPServer.h"

int main(int argc, char * argv[])
{
	IOCPServer* GameServer = new IOCPServer();
	GameServer->Run();

	delete GameServer;
}