#include "../Server/PCH/stdafx.h"
#include "../Server/IOCPServer/IOCPServer.h"

auto main(int argc, char * argv[]) -> int
{
	unique_ptr<IOCPServer> GameServer 
		= make_unique<IOCPServer>
		(
			[/* void */]() noexcept(false) -> const bool
			{
				enum SERVER_INIT_TYPE : int
				{
					  EXTERNAL_IP = 1
					, LOCAL_HOST = 2
				};

#ifdef _DEBUG_MODE_
				std::cout << "\n"
					<< "   ���� : ���� _DEBUG_MODE_�� Define�Ǿ����ϴ�. \n";
				_sleep(1000);
#endif
				std::cout << "\n"
					<< "   Server IP�� �������ּ���.\n"
					<< "      1. External IP\n" 
					<< "      2. LocalHost\n"
					<< "                       ---> ";

				int serverInitType{};
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

