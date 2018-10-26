#include "../Server/PCH/stdafx.h"
#include "../Server/IOCPServer/IOCPServer.h"

int main(int argc, char * argv[])
{
	IOCPServer* GameServer = new IOCPServer();
	GameServer->Run();

	delete GameServer;
}