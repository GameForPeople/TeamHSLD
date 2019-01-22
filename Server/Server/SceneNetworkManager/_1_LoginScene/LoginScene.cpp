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

void SCENE_NETWORK_MANAGER::LoginScene::ProcessData(const int InRecvType, SocketInfo* pClient)
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
	BYTE outActiveCharacterBit{ 0 };

	//typeBuffer == 1 
	//? LoginTest(pClient, InUserData, idBuffer, pwBuffer, outWinCount, outLoseCount, outMoney) 
	//: SignUpTest(pClient, InUserData, idBuffer, pwBuffer);

	if (int failReason 
		= pUserData->LoginProcess(pClient, idBuffer, outNickname, outWinCount, outLoseCount, outMoney,outAchievementBit, outTitleBit, outCharacterBit, outActiveCharacterBit, outFriendStringCont)
		; failReason)
		__SendFailLogin(pClient, failReason);
	else 
		__SendPermitLogin(pClient, outNickname, outWinCount, outLoseCount, outMoney, outAchievementBit, outTitleBit, outCharacterBit, static_cast<int>(outActiveCharacterBit), outFriendStringCont);
	
	std::cout << "     [UserDataManager] " << outNickname << " (" << idBuffer << ")" <<" 님이 로그인 하셨습니다. " << "\n";
}

void SCENE_NETWORK_MANAGER::LoginScene::__SendFailLogin(SocketInfo* pClient, const int RetFailReason)
{
	memcpy(pClient->buf, reinterpret_cast<const char*>(&FAIL_LOGIN), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&RetFailReason), sizeof(int));

	pClient->dataSize = 8;
}

/*
	__SendPermitLogin()

	서버에 저장된 유저 데이터들을 클라이언트에게 전송합니다.
*/
void SCENE_NETWORK_MANAGER::LoginScene::__SendPermitLogin(SocketInfo* pClient, const Type_Nickname& InNickname, 
	const int InWinCount, const int InLoseCount, const int InMoney,
	const int InAchievementBit, const int InTitleBit, const int InCharacterBit, const int InActiveCharacterIndex, const vector<Type_Nickname>& InFriendStringCont)
{
	//DEV_78 로그인 시, 동기화를 위해, 닉네임 트리와, UserNode의 ActiveCharacterIndex를 UserNode의 ActiveCharacterIndex으로 통일함.
	pUserData->SetActiveCharacterIndexWithNickname(pClient->pUserNode->GetNickname(), static_cast<char>(pClient->pUserNode->GetActiveCharacterIndex()));

	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_LOGIN), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&InWinCount), sizeof(int));
	memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&InLoseCount), sizeof(int));
	memcpy(pClient->buf + 12, reinterpret_cast<const char*>(&InMoney), sizeof(int));
	memcpy(pClient->buf + 16, reinterpret_cast<const char*>(&InAchievementBit), sizeof(int));
	memcpy(pClient->buf + 20, reinterpret_cast<const char*>(&InTitleBit), sizeof(int));
	memcpy(pClient->buf + 24, reinterpret_cast<const char*>(&InCharacterBit), sizeof(int));
	memcpy(pClient->buf + 28, reinterpret_cast<const char*>(&InActiveCharacterIndex), sizeof(int));

	int stringSizeBuffer = MultiByteToWideChar(CP_ACP, 0, &InNickname[0], InNickname.size(), NULL, NULL);
	//int stringSizeBuffer = InNickname.size(); // DEV_66 char * 2 = wchar
	wstring uniStrBuffer(stringSizeBuffer, 0);
	MultiByteToWideChar(CP_ACP, 0, &InNickname[0], InNickname.size(), &uniStrBuffer[0], stringSizeBuffer);

	stringSizeBuffer = uniStrBuffer.size() * 2;
	memcpy(pClient->buf + 32, reinterpret_cast<const char*>(&stringSizeBuffer), sizeof(int));
	memcpy(pClient->buf + 36, uniStrBuffer.data(), stringSizeBuffer);

	pClient->dataSize = 36 + stringSizeBuffer;

	// 친구 관련해서는, Friend Ui On일 떄 처리하도록 수정함.
}


// About Nickname ---------------------------------------------------------------------------------

void SCENE_NETWORK_MANAGER::LoginScene::_RecvChangeNickname(SocketInfo* pClient)
{
	int nicknameSizeBuf = reinterpret_cast<int&>(pClient->buf[4]);
	
	// UTF-16 스트링 생성
	std::wstring wstrBuf(nicknameSizeBuf * 0.5, 0);
	memcpy(&wstrBuf[0], &pClient->buf[8], nicknameSizeBuf);

	// UTF-16 -> MBCS
	nicknameSizeBuf = WideCharToMultiByte(CP_ACP, 0, &wstrBuf[0], -1, NULL, 0, NULL, NULL);
	Type_Nickname nicknameBuf(nicknameSizeBuf, 0);
	WideCharToMultiByte(CP_ACP, 0, &wstrBuf[0], -1, &nicknameBuf[0], nicknameSizeBuf, NULL, NULL);

	if (pUserData->SetNewNickname(pClient, nicknameBuf))
	{
		pClient->pUserNode->SetMoney(0);
		__SendChangeNickname(pClient, true);
	}
	else
	{
		__SendChangeNickname(pClient, false);
	}
}

void SCENE_NETWORK_MANAGER::LoginScene::__SendChangeNickname(SocketInfo* pClient, bool InBValue)
{
	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_Nickname), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&InBValue), sizeof(bool));

	pClient->dataSize = 5;
}