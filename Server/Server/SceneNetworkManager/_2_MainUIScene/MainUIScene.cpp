#include "../_2_MainUIScene/MainUIScene.h"

#include "../../IOCPServer/SocketInfo.h"

SCENE_NETWORK_MANAGER::MainUiScene::MainUiScene() 
	: BaseScene()
	, PERMIT_FRIEND_INFO(Protocol::PERMIT_FRIEND_INFO)
	, CONST_TRUE(1) , CONST_FALSE (0)
{}

void SCENE_NETWORK_MANAGER::MainUiScene::ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	if (InRecvType == DEMAND_FRIEND_INFO)
		_DemandFriendInfoProcess(ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInfoProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	memcpy(ptr->buf, reinterpret_cast<const char*>(&PERMIT_FRIEND_INFO), sizeof(int));

	//UserData buffer = 
	int friendNum = ptr->pUserNode->SetValue().GetFriendStringCont().size();
	memcpy(ptr->buf + 4, reinterpret_cast<char*>(&friendNum), sizeof(int));

	ptr->dataSize = 8;

	if (friendNum)
	{
		int stringSize{};
		string stringBuffer{};
		bool isOnLogin{};
		rbTreeNode<string, UserData> *pBuffer = nullptr;

		for (int i = 0; i < friendNum; ++i)
		{
			// 해당 컨테이너의 i번째 아이디 스트링의 사이즈.
			stringBuffer = ptr->pUserNode->SetValue().GetFriendStringCont()[i];
			stringSize = stringBuffer.size();

			// 아이디 스트링 사이즈와, 아이디 적재.
			memcpy(ptr->buf + ptr->dataSize, reinterpret_cast<char*>(&stringSize), sizeof(int));
			memcpy(ptr->buf + ptr->dataSize + 4, stringBuffer.data(), stringSize);

			ptr->dataSize += 4 + stringSize;

			// 해당 아이디로 현재 상태 판정, True일경우 1, false일 경우 0
			pBuffer = InUserData.SearchUserNode(stringBuffer, isOnLogin);

			// 로그인 햇을 때, 해당 유저 닉네임 사이즈와, 닉네임, 현재 상태를 보냄.
			if (isOnLogin)
			{
				memcpy(ptr->buf + ptr->dataSize, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

				stringBuffer = ptr->pUserNode->GetValue().GetNickName();
				stringSize = stringBuffer.size();

				memcpy(ptr->buf + ptr->dataSize + 4, reinterpret_cast<char*>(&stringSize), sizeof(int));
				memcpy(ptr->buf + ptr->dataSize + 8, stringBuffer.data(), stringSize);
				
				ptr->dataSize += 12 + stringSize; // 4 Bool isOnLogin, 4 nickNameSize, 4 (바로 뒷줄) 방 들어가 있는지 없는지 여부.

				if (pBuffer->GetValue().GetSocketInfo()->pRoomIter != nullptr)
				{
					memcpy(ptr->buf + ptr->dataSize - 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
				}
				else
				{
					memcpy(ptr->buf + ptr->dataSize - 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				}
			}
			// 로그인 하지 않았을 때, 해당 데이터 적재.
			else
			{
				memcpy(ptr->buf + ptr->dataSize, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				ptr->dataSize += sizeof(int);	// 4 Bool isOnLogin
			}
		}
	}
}