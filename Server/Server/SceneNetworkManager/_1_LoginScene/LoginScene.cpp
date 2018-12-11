#include "../_1_LoginScene/LoginScene.h"
#include "../../IOCPServer/SocketInfo.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::LoginScene::LoginScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager) 
	: BaseScene(pInRoomData, pInUserData, pInUDPManager)
	, PERMIT_LOGIN(Protocol::PERMIT_LOGIN)
	, FAIL_LOGIN(Protocol::FAIL_LOGIN)
	, PERMIT_Nickname(Protocol::PERMIT_Nickname)
{
}

void SCENE_NETWORK_MANAGER::LoginScene::ProcessData(const int& InRecvType, SocketInfo* pClient)
{
	if (InRecvType == DEMAND_LOGIN)
		_RecvDemandLogin(pClient);
	else if (InRecvType == CHANGE_Nickname)
		_RecvChangeNickname(pClient);
	// 나머지는 날림.
}

// About Login ---------------------------------------------------------------------------------

void SCENE_NETWORK_MANAGER::LoginScene::_RecvDemandLogin(SocketInfo* pClient)
{
	int idSizeBuffer = (int&)(pClient->buf[4]);
	pClient->buf[8 + idSizeBuffer] = '\0';

	string idBuffer(pClient->buf + 8);

	int outWinCount{0};
	int outLoseCount{0};
	int outMoney{0};
	Type_Nickname outNickname{};
	int outAchievementBit{ 0 };
	int outTitleBit{ 0 };
	int outCharacterBit{ 0 };
	vector<Type_Nickname> outFriendStringCont;

	//typeBuffer == 1 
	//? LoginTest(pClient, InUserData, idBuffer, pwBuffer, outWinCount, outLoseCount, outMoney) 
	//: SignUpTest(pClient, InUserData, idBuffer, pwBuffer);

	if (int failReason 
		= pUserData->LoginProcess(pClient, idBuffer, outNickname, outWinCount, outLoseCount, outMoney,outAchievementBit, outTitleBit, outCharacterBit, outFriendStringCont)
		; failReason)
		__SendFailLogin(pClient, failReason);
	else 
		__SendPermitLogin(pClient, outNickname, outWinCount, outLoseCount, outMoney, outAchievementBit, outTitleBit, outCharacterBit, outFriendStringCont );
		std::cout << "     [UserDataManager] " << outNickname << " (" << idBuffer << ")" <<" 님이 로그인 하셨습니다. " << "\n";
}

void SCENE_NETWORK_MANAGER::LoginScene::__SendFailLogin(SocketInfo* pClient, const int& RetFailReason)
{
	memcpy(pClient->buf, reinterpret_cast<const char*>(&FAIL_LOGIN), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&RetFailReason), sizeof(int));

	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::LoginScene::__SendPermitLogin(SocketInfo* pClient, const Type_Nickname& InNickname, 
	const int& InWinCount, const int& InLoseCount, const int& InMoney,
	const int& InAchievementBit, const int& InTitleBit, const int& InCharacterBit, const vector<Type_Nickname>& InFriendStringCont)
{
	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_LOGIN), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&InWinCount), sizeof(int));
	memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&InLoseCount), sizeof(int));
	memcpy(pClient->buf + 12, reinterpret_cast<const char*>(&InMoney), sizeof(int));
	memcpy(pClient->buf + 16, reinterpret_cast<const char*>(&InAchievementBit), sizeof(int));
	memcpy(pClient->buf + 20, reinterpret_cast<const char*>(&InTitleBit), sizeof(int));
	memcpy(pClient->buf + 24, reinterpret_cast<const char*>(&InCharacterBit), sizeof(int));

	int stringSizeBuffer = InNickname.size(); // DEV_66 char * 2 = wchar

	memcpy(pClient->buf + 28, reinterpret_cast<const char*>(&stringSizeBuffer), sizeof(int));
	memcpy(pClient->buf + 32, InNickname.data(), stringSizeBuffer);

	pClient->dataSize = 32 + stringSizeBuffer;

	// 친구 기능 일단 주석처리함. // 이 때 굳이 보낼 필요 없음.
	/*
	int stringSizeBuffer = InFriendStringCont.size();
	int totalSizeBuffer{ 0 };

	memcpy(pClient->buf + 28, (char*)&stringSizeBuffer, sizeof(int));

	if (stringSizeBuffer > 0)
	{
		//for (auto iter = InFriendStringCont.begin(); iter != InFriendStringCont.end(); iter++)
		for (auto iter = 0; iter < InFriendStringCont.size(); ++iter)
		{
			stringSizeBuffer = InFriendStringCont[iter].size();

			memcpy(pClient->buf + 32 + totalSizeBuffer, (char*)&stringSizeBuffer, sizeof(int));
			memcpy(pClient->buf + 36 + totalSizeBuffer, InFriendStringCont[iter].data(), stringSizeBuffer);
			//for (int j = 0; j < stringSizeBuffer; ++j)
			//{
			//	pClient->buf[36 + totalSizeBuffer + j] = InFriendStringCont[iter][j];
			//}

			totalSizeBuffer += (4 + stringSizeBuffer);
		}
	}
	
	pClient->dataSize = 32 + totalSizeBuffer;
	*/
}


// About Nickname ---------------------------------------------------------------------------------

void SCENE_NETWORK_MANAGER::LoginScene::_RecvChangeNickname(SocketInfo* pClient)
{
	int NicknameSizeBuffer = reinterpret_cast<int&>(pClient->buf[4]);
	
	//for (int i = 0; i < NicknameSizeBuffer; ++i)
	//{
	//	// Refactor 필요.
	//	NicknameBuffer += pClient->buf[8 + i];
	//}

	pClient->buf[8 + NicknameSizeBuffer] = '\0';
	
	Type_Nickname NicknameBuffer(pClient->buf + 8);

	pClient->pUserNode->SetNickname(NicknameBuffer);
}

void SCENE_NETWORK_MANAGER::LoginScene::__SendChangeNickname(SocketInfo* pClient)
{
	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_Nickname), sizeof(int));

	pClient->dataSize = 4;
}