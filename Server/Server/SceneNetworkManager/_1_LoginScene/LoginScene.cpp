#include "../_1_LoginScene/LoginScene.h"
#include "../../IOCPServer/SocketInfo.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::LoginScene::LoginScene(): 
	BaseScene(), PERMIT_LOGIN(Protocol::PERMIT_LOGIN), FAIL_LOGIN(Protocol::FAIL_LOGIN), PERMIT_NICKNAME(Protocol::PERMIT_NICKNAME)
{

}

void SCENE_NETWORK_MANAGER::LoginScene::ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager)
{
	//ProcessRecv(InRecvType, ptr, InUserData);
	//ProcessSend(InRecvType, ptr, InUserData);
	if (InRecvType == DEMAND_LOGIN)
		RecvDemandLogin(ptr, InUserData);
	else if (InRecvType == CHANGE_NICKNAME)
		RecvChangeNickName(ptr, InUserData);
	// 나머지는 날림.
}

//void SCENE_NETWORK_MANAGER::LoginScene::ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, UserDataManager& InUserData)
//{
//	// only Demand Login Recv
//}

//void SCENE_NETWORK_MANAGER::LoginScene::ProcessSend(const int& InSendType, SOCKETINFO* ptr, UserDataManager& InUserData)
//{
//	//memcpy(ptr->buf, (char*)&InSendType, sizeof(int));
//}

void SCENE_NETWORK_MANAGER::LoginScene::RecvDemandLogin(SocketInfo* ptr, UserDataManager& InUserData)
{
	int idSizeBuffer = (int&)(ptr->buf[4]);

	ptr->buf[8 + idSizeBuffer] = '\0';

	string idBuffer(ptr->buf + 8);

	std::cout << " 로그인을 요청받은 아이디는 : " << idBuffer << "  , idSize는 " << idSizeBuffer << "\n";
	
	int outWinCount{0};
	int outLoseCount{0};
	int outMoney{0};
	string outNickName;
	int outAchievementBit{ 0 };
	int outTitleBit{ 0 };
	int outCharacterBit{ 0 };
	vector<string> outFriendStringCont;

	int failReason = LoginProcess(ptr, InUserData, idBuffer, outNickName, outWinCount, outLoseCount, outMoney,
		outAchievementBit, outTitleBit, outCharacterBit, outFriendStringCont);

	//typeBuffer == 1 
	//? LoginTest(ptr, InUserData, idBuffer, pwBuffer, outWinCount, outLoseCount, outMoney) 
	//: SignUpTest(ptr, InUserData, idBuffer, pwBuffer);

	if (failReason)
		SendFailLogin(ptr, failReason);
	else 
	{
		SendPermitLogin(ptr, outNickName, outWinCount, outLoseCount, outMoney, outAchievementBit, outTitleBit, outCharacterBit, outFriendStringCont );
		std::cout << "     [UserDataManager] "<< outNickName << " (" << idBuffer << ")" <<" 님이 로그인 하셨습니다. " << "\n";
	}

}

//int SCENE_NETWORK_MANAGER::LoginScene::LoginTest(SocketInfo* ptr, UserDataManager& InUserData,
//	const string& InIdBuffer, const int& InPwBuffer, int& outWinCount, int& outLoseCount, int& outMoney)
//{
//	return InUserData.SignIn(InIdBuffer, InPwBuffer, outWinCount, outLoseCount, outMoney, ptr->userIndex);
//}
//
//int SCENE_NETWORK_MANAGER::LoginScene::SignUpTest(SocketInfo* ptr, UserDataManager& InUserData, const string& InIdBuffer, const int& InPwBuffer)
//{
//	return InUserData.SignUp(InIdBuffer, InPwBuffer, ptr->userIndex);
//}

int SCENE_NETWORK_MANAGER::LoginScene::LoginProcess
(SocketInfo* pInSocketInfo, UserDataManager& InUserData, const string& InID, string& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
	int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<string>& RetFriendStringCont)
{
	return InUserData.LoginProcess(pInSocketInfo, InID, RetNickName, RetWinCount, RetLoseCount, RetMoney,
		RetAchievementBit, RetTitleBit, RetCharacterBit, RetFriendStringCont);
}

void SCENE_NETWORK_MANAGER::LoginScene::SendPermitLogin(SocketInfo* ptr, const string& InNickName, 
	const int& InWinCount, const int& InLoseCount, const int& InMoney,
	const int& InAchievementBit, const int& InTitleBit, const int& InCharacterBit, const vector<string>& InFriendStringCont)
{
	memcpy(ptr->buf, (char*)&PERMIT_LOGIN, sizeof(int));
	memcpy(ptr->buf + 4, (char*)&InWinCount, sizeof(int));
	memcpy(ptr->buf + 8, (char*)&InLoseCount, sizeof(int));
	memcpy(ptr->buf + 12, (char*)&InMoney, sizeof(int));
	memcpy(ptr->buf + 16, (char*)&InAchievementBit, sizeof(int));
	memcpy(ptr->buf + 20, (char*)&InTitleBit, sizeof(int));
	memcpy(ptr->buf + 24, (char*)&InCharacterBit, sizeof(int));

	int stringSizeBuffer = InNickName.size();

	memcpy(ptr->buf + 28, (char*)&stringSizeBuffer, sizeof(int));
	memcpy(ptr->buf + 32, InNickName.data(), stringSizeBuffer);

	ptr->dataSize = 32 + stringSizeBuffer;

	// 친구 기능 일단 주석처리함. // 이 때 굳이 보낼 필요 없음.
	/*
	int stringSizeBuffer = InFriendStringCont.size();
	int totalSizeBuffer{ 0 };

	memcpy(ptr->buf + 28, (char*)&stringSizeBuffer, sizeof(int));

	if (stringSizeBuffer > 0)
	{
		//for (auto iter = InFriendStringCont.begin(); iter != InFriendStringCont.end(); iter++)
		for (auto iter = 0; iter < InFriendStringCont.size(); ++iter)
		{
			stringSizeBuffer = InFriendStringCont[iter].size();

			memcpy(ptr->buf + 32 + totalSizeBuffer, (char*)&stringSizeBuffer, sizeof(int));
			memcpy(ptr->buf + 36 + totalSizeBuffer, InFriendStringCont[iter].data(), stringSizeBuffer);
			//for (int j = 0; j < stringSizeBuffer; ++j)
			//{
			//	ptr->buf[36 + totalSizeBuffer + j] = InFriendStringCont[iter][j];
			//}

			totalSizeBuffer += (4 + stringSizeBuffer);
		}
	}
	
	ptr->dataSize = 32 + totalSizeBuffer;
	*/
}


void SCENE_NETWORK_MANAGER::LoginScene::SendFailLogin(SocketInfo* ptr, const int& RetFailReason)
{
	memcpy(ptr->buf, (char*)&FAIL_LOGIN, sizeof(int));
	memcpy(ptr->buf + 4, (char*)&RetFailReason, sizeof(int));

	ptr->dataSize = 8;
}


void SCENE_NETWORK_MANAGER::LoginScene::RecvChangeNickName(SocketInfo* ptr, UserDataManager& InUserData)
{
	int nickNameSizeBuffer = (int&)(ptr->buf[4]);

	string nickNameBuffer;

	for (int i = 0; i < nickNameSizeBuffer; ++i)
	{
		// Refactor 필요.

		nickNameBuffer += ptr->buf[8 + i];
	}

	ptr->pUserNode->SetValue().SetNickName(nickNameBuffer);

	SendChangeNickName(ptr);
}

void SCENE_NETWORK_MANAGER::LoginScene::SendChangeNickName(SocketInfo* ptr)
{
	memcpy(ptr->buf, (char*)&PERMIT_NICKNAME, sizeof(int));

	ptr->dataSize = 4;
}