#include "../_1_LoginScene/LoginScene.h"

SCENE_NETWORK_MANAGER::LoginScene::LoginScene(): BaseScene(), PERMIT_LOGIN(Protocol::PERMIT_LOGIN), FAIL_LOGIN(Protocol::FAIL_LOGIN)
{

}

void SCENE_NETWORK_MANAGER::LoginScene::ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, NewUserDataManager& InUserData)
{
	//ProcessRecv(InRecvType, ptr, InUserData);
	//ProcessSend(InRecvType, ptr, InUserData);
	RecvDemandLogin(ptr, InUserData);
}

//void SCENE_NETWORK_MANAGER::LoginScene::ProcessRecv(const int& InRecvType, SOCKETINFO* ptr, UserDataManager& InUserData)
//{
//	// only Demand Login Recv
//}

//void SCENE_NETWORK_MANAGER::LoginScene::ProcessSend(const int& InSendType, SOCKETINFO* ptr, UserDataManager& InUserData)
//{
//	//memcpy(ptr->buf, (char*)&InSendType, sizeof(int));
//}

void SCENE_NETWORK_MANAGER::LoginScene::RecvDemandLogin(SocketInfo* ptr, NewUserDataManager& InUserData)
{
	int typeBuffer = (int&)(ptr->buf[4]);
	int pwBuffer = (int&)(ptr->buf[8]);
	int idSizeBuffer = (int&)(ptr->buf[12]);
	string idBuffer;

	for (int i = 0; i < idSizeBuffer; ++i)
	{
		//idBuffer.append(&(ptr->buf[16+i]));
		idBuffer += ptr->buf[16 + i];
	}

	int outWinCount{0};
	int outLoseCount{0};
	int outMoney{0};

	int failReason = 
		typeBuffer == 1 
		? LoginTest(ptr, InUserData, idBuffer, pwBuffer, outWinCount, outLoseCount, outMoney) 
		: SignUpTest(ptr, InUserData, idBuffer, pwBuffer);

	if (failReason)
		SendFailLogin(ptr, failReason);
	else 
	{
		SendPermitLogin(ptr, outWinCount, outLoseCount, outMoney);
		std::cout << "     [UserDataManager] "<< idBuffer <<" ´ÔÀÌ ·Î±×ÀÎ ÇÏ¼Ì½À´Ï´Ù. " << "\n";
	}
}

int SCENE_NETWORK_MANAGER::LoginScene::LoginTest(SocketInfo* ptr, NewUserDataManager& InUserData,
	const string& InIdBuffer, const int& InPwBuffer, int& outWinCount, int& outLoseCount, int& outMoney)
{
	return InUserData.SignIn(InIdBuffer, InPwBuffer, outWinCount, outLoseCount, outMoney, ptr->userIndex);
}

int SCENE_NETWORK_MANAGER::LoginScene::SignUpTest(SocketInfo* ptr, NewUserDataManager& InUserData, const string& InIdBuffer, const int& InPwBuffer)
{
	return InUserData.SignUp(InIdBuffer, InPwBuffer, ptr->userIndex);
}

void SCENE_NETWORK_MANAGER::LoginScene::SendPermitLogin(SocketInfo* ptr, const int& InWinCount, const int& InLoseCount, const int& InMoney)
{
	memcpy(ptr->buf, (char*)&PERMIT_LOGIN, sizeof(int));
	memcpy(ptr->buf + 4, (char*)&InWinCount, sizeof(int));
	memcpy(ptr->buf + 8, (char*)&InLoseCount, sizeof(int));
	memcpy(ptr->buf + 12, (char*)&InMoney, sizeof(int));

	ptr->dataSize = 16;
}

void SCENE_NETWORK_MANAGER::LoginScene::SendFailLogin(SocketInfo* ptr, const int& RetFailReason)
{
	memcpy(ptr->buf, (char*)&FAIL_LOGIN, sizeof(int));
	memcpy(ptr->buf + 4, (char*)&RetFailReason, sizeof(int));

	ptr->dataSize = 8;
}