#include "../_2_MainUIScene/MainUIScene.h"

#include "../../IOCPServer/SocketInfo.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::MainUiScene::MainUiScene() 
	: BaseScene()
	, PERMIT_FRIEND_INFO(Protocol::PERMIT_FRIEND_INFO), NOTIFY_FRIEND_INVITE(Protocol::NOTIFY_FRIEND_INVITE)
	, GUESTCHECK_FRIEND_INVITE(Protocol::GUESTCHECK_FRIEND_INVITE), HOSTCHECK_FRIEND_INVITE(Protocol::HOSTCHECK_FRIEND_INVITE)
	//, ANSWER_FRIEND_INVITE(Protocol::ANSWER_FRIEND_INVITE)
	, CONST_TRUE(1) , CONST_FALSE (0)
{}

void SCENE_NETWORK_MANAGER::MainUiScene::ProcessData(const int& InRecvType, SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager)
{
	if (InRecvType == DEMAND_FRIEND_INFO)
		_DemandFriendInfoProcess(ptr, InRoomData, InUserData);
	else if (InRecvType == DEMAND_FRIEND_INVITE)
		_DemandFriendInviteProcess(ptr, InRoomData, InUserData, InUDPManager);
	else if (InRecvType == ANSWER_FRIEND_INVITE)
		_AnswerFriendInviteProcess(ptr, InRoomData, InUserData);
	else if (InRecvType == DELAY_FRIEND_INVITE)
		_DelayFriendInviteProcess(ptr, InRoomData, InUserData);
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInfoProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	memcpy(ptr->buf, reinterpret_cast<const char*>(&PERMIT_FRIEND_INFO), sizeof(int));

	//UserData buffer = 
	int friendNum = ptr->pUserNode->SetValue().GetFriendStringCont().size();
	memcpy(ptr->buf + 4, reinterpret_cast<char*>(&friendNum), sizeof(int));

	ptr->dataSize = 8;

	std::cout << " " << ptr->pUserNode->GetKey() << "의 친구 수는 : " << friendNum << "\n";

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
				//stringSize = stringBuffer.size();
			std::cout << " " << ptr->pUserNode->GetKey() << "의 친구 : " << stringBuffer << "\n";

			// 아이디 스트링 사이즈와, 아이디 적재.
				//memcpy(ptr->buf + ptr->dataSize, reinterpret_cast<char*>(&stringSize), sizeof(int));
				//memcpy(ptr->buf + ptr->dataSize + 4, stringBuffer.data(), stringSize);
				//ptr->dataSize += 4 + stringSize;

			// 해당 아이디로 현재 상태 판정, True일경우 1, false일 경우 0
			pBuffer = InUserData.SearchUserNode(stringBuffer, isOnLogin);

			// 로그인 햇을 때, 해당 유저 닉네임 사이즈와, 닉네임, 현재 상태를 보냄.
			if (isOnLogin)
			{
				// 현재 클라이언트의, UserNode에, 친구 소켓 인포 저장
					//ptr->pUserNode->SetValue().SetFreindSocketInfo(pBuffer->SetValue().GetSocketInfo(), i); 				// 이거 쪼금 위험할 수도 있음. 테스트 필요.

				memcpy(ptr->buf + ptr->dataSize, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));


				stringBuffer = pBuffer->SetValue().GetNickName();
				stringSize = stringBuffer.size();

				memcpy(ptr->buf + ptr->dataSize + 4, reinterpret_cast<char*>(&stringSize), sizeof(int));
				memcpy(ptr->buf + ptr->dataSize + 8, stringBuffer.data(), stringSize);
				
				ptr->dataSize += (12 + stringSize); // 4 Bool isOnLogin, 4 nickNameSize, 4 (바로 뒷줄) 방 들어가 있는지 없는지 여부.

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

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInviteProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager)
{
	int indexBuffer = reinterpret_cast<int&>(ptr->buf[4]);

	// 기존에 저장한 거 쓰지 말고, 새로 Search해서 씁시다? 성능 버리고, 안정성?
	//ptr->pUserNode->SetValue().GetFriendSocketInfo(indexBuffer);
	bool isOnUser{ false };

	rbTreeNode<string, UserData> *pBuffer = InUserData.SearchUserNode(ptr->pUserNode->SetValue().GetFriendStringWithIndex(indexBuffer), isOnUser);

	if (isOnUser && pBuffer->SetValue().GetSocketInfo()->pRoomIter == nullptr)
	{
		// 방 생성 필요
		ptr->pRoomIter = new GameRoom(ptr->pUserNode, nullptr, nullptr);

		// 나는야 방장님이시다.
		ptr->isHost = true;

		// 친구 방도 등록.
		pBuffer->SetValue().GetSocketInfo()->pRoomIter = ptr->pRoomIter;

		// UDP Packet 등록.
		InUDPManager.Push(1, pBuffer->SetValue().GetSocketInfo());

		// TCP True Send 필요.
		memcpy(ptr->buf, reinterpret_cast<const char*>(&NOTIFY_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}
	else
	{
		// TCP False Send 필요.
		memcpy(ptr->buf, reinterpret_cast<const char*>(&NOTIFY_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerFriendInviteProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	int isTrueBuffer = reinterpret_cast<int&>(ptr->buf[4]);

	if (ptr->pRoomIter != nullptr)
	{
		if (isTrueBuffer == 0)
		{
			// 동기화 오바칠 수 있음.
			//ptr->pRoomIter->SetDataProtocol(false, );
			ptr->pRoomIter->RetEnemyUserIter(ptr->isHost)->SetValue().GetSocketInfo()->pRoomIter = nullptr;

			delete ptr->pRoomIter;
			ptr->pRoomIter = nullptr;

			// 거절에 의해 방 삭제.
			memcpy(ptr->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		}
		else
		{
			ptr->pRoomIter->JoinRoom(ptr->pUserNode);

			// 방 접속.
			memcpy(ptr->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
		}
	}
	else
	{
		//이미 10초가 지난 방.
		memcpy(ptr->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DelayFriendInviteProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	if (ptr->pRoomIter == nullptr)
	{
		// 이대로 끝 OK!
		memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}
	else if (ptr->pRoomIter->RetEnemyUserIter(ptr->isHost) == nullptr)
	{
		// 방 제거해줌. 여기서 이대로 끝! -> guest의 UDP 손실 가능성또는 동기화 실패 가능성-> 이거를 검사할 수 있으면 좋지 않을 까?
		delete ptr->pRoomIter;
		ptr->pRoomIter = nullptr;

		memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}
	else
	{
		// 적이 딱 맥스타임에 들어온거겠지? 해당 부분 어케한다냐 // 뭘어째 False보내면 돼지 뭐.
		memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	ptr->dataSize = 8;
}