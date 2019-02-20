#include "../Server/PCH/stdafx.h"
#include "../Server/IOCPServer/IOCPServer.h"

auto main(int argc, char * argv[]) -> int
{
	unique_ptr<IOCPServer> GameServer 
		= make_unique<IOCPServer>
		(
			[/* void */]() noexcept(false) -> const IOCPServer::SERVER_INIT_TYPE
			{
#ifdef _DEBUG_MODE_
				std::cout << "\n   Caution! : the current '_DEBUG_MODE_' has been Defined \n";
				Sleep(1000);
#endif
				std::cout << "\n"
					<< "   Please select the server IP you want!\n"
					<< "      1. External IP\n" 
					<< "      2. LocalHost\n"
					<< "      3. AWS Public IP\n"		
					<< "                       ---> ";

				int serverInitType{};
				std::cin >> serverInitType;
				system("cls");

				if (serverInitType == 1) return IOCPServer::SERVER_INIT_TYPE::EXTERNAL_IP;
				else if (serverInitType == 2)  return IOCPServer::SERVER_INIT_TYPE::LOCAL_HOST;
				else if (serverInitType == 3) return IOCPServer::SERVER_INIT_TYPE::PUBLIC_IP;
				else throw ERROR;
			}()
		);

	GameServer->Run();

	/*GameServer.reset();*/		// auto! 
}

