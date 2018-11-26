#include "../Server/PCH/stdafx.h"
#include "../Server/IOCPServer/IOCPServer.h"

int main(int argc, char * argv[])
{
	unique_ptr<IOCPServer> GameServer 
		= make_unique<IOCPServer>
		(
			[/* void */]() throw() -> const bool
			{
				enum SERVER_INIT_TYPE
				{
					EXTERN_IP = 1
					, LOCAL_HOST = 2
				};

				int serverInitType{};
				std::cout << "Server IP를 선택해주세요.\n   1. Extern IP\n   2. LocalHost\n   --> ";
				std::cin >> serverInitType;
				system("cls");

				if (serverInitType == SERVER_INIT_TYPE::EXTERN_IP) return true;
				else if (serverInitType == SERVER_INIT_TYPE::LOCAL_HOST)  return false;
				else throw ERROR;
			}()
		);

	GameServer->Run();

	/*GameServer.reset();*/		// auto! 
}

