#include "../Server/PCH/stdafx.h"
#include "../Server/IOCPServer/IOCPServer.h"

int main(int argc, char * argv[])
{
	unique_ptr<IOCPServer> GameServer 
		= make_unique<IOCPServer>
		(
			[/* void */]() noexcept(false) -> const bool
			{
				enum SERVER_INIT_TYPE
				{
					EXTERNAL_IP = 1
					, LOCAL_HOST = 2
				};

				int serverInitType{};
				std::cout << "# Server IP를 선택해주세요.\n"
					<< "   1. External IP\n" 
					<< "   2. LocalHost\n"
					<< "   ------------> ";
				std::cin >> serverInitType;
				system("cls");

				if (serverInitType == SERVER_INIT_TYPE::EXTERNAL_IP) return true;
				else if (serverInitType == SERVER_INIT_TYPE::LOCAL_HOST)  return false;
				else throw ERROR;
			}()
		);

	GameServer->Run();

	/*GameServer.reset();*/		// auto! 
}

