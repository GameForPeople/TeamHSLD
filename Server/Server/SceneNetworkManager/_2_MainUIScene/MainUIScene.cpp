#include "../_2_MainUIScene/MainUIScene.h"

#include "../../IOCPServer/SocketInfo.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::MainUiScene::MainUiScene() 
	: BaseScene()
	, PERMIT_FRIEND_INFO(Protocol::PERMIT_FRIEND_INFO), NOTIFY_FRIEND_INVITE(Protocol::NOTIFY_FRIEND_INVITE)
	, GUESTCHECK_FRIEND_INVITE(Protocol::GUESTCHECK_FRIEND_INVITE), HOSTCHECK_FRIEND_INVITE(Protocol::HOSTCHECK_FRIEND_INVITE)
	//, ANSWER_FRIEND_INVITE(Protocol::ANSWER_FRIEND_INVITE)
	, CHECK_DEMAND_MAKE_FRIEND(Protocol::CHECK_DEMAND_MAKE_FRIEND)
	, NOTIFY_MAKE_FRIEND_INFO(Protocol::NOTIFY_MAKE_FRIEND_INFO)
	, CHECK_ANSWER_MAKE_FRIEND(Protocol::CHECK_ANSWER_MAKE_FRIEND)
	, CONST_TRUE(1) , CONST_FALSE (0), CONST_2(2), CONST_3(3)
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
	else if (InRecvType == DEMAND_MAKE_FRIEND)
		_DemandMakeFriendProcess(ptr, InRoomData, InUserData, InUDPManager);
	else if (InRecvType == DEMAND_MAKE_FRIEND_INFO)
		_DemandMakeFriendInfoProcess(ptr, InRoomData, InUserData);
	else if (InRecvType == ANSWER_MAKE_FRIEND)
		_AnswerMakeFriendProcess(ptr, InRoomData, InUserData, InUDPManager);
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

		// 친구 방과 친구의 소켓주소구조체도 등록.
		pBuffer->SetValue().GetSocketInfo()->pRoomIter = ptr->pRoomIter;
		ptr->pRoomIter->SetFriendUserPtr(pBuffer);

		// UDP Packet 등록.
		InUDPManager.Push(UDP_PROTOCOL::INVITE_FRIEND, pBuffer->SetValue().GetSocketInfo());

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
			ptr->pRoomIter->SetUserPtr(true, nullptr);

			// 방 삭제 호스트가 하도록 변경됨.
			//delete ptr->pRoomIter;
			//ptr->pRoomIter = nullptr;

			memcpy(ptr->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		}
		else
		{
			ptr->pRoomIter->JoinRoom(ptr->pUserNode);
			ptr->isHost = false;

			// 방 접속.
			memcpy(ptr->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

			int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
			ptr->pRoomIter->GetRoomGameData(ptr->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

			memcpy(ptr->buf + 8, (char*)&retIsHostFirst, sizeof(int));
			memcpy(ptr->buf + 12, (char*)&retPlayerMissionIndex, sizeof(int));
			memcpy(ptr->buf + 16, (char*)&retEnemyMissionIndex, sizeof(int));
			memcpy(ptr->buf + 20, (char*)&retSubMissionIndex, sizeof(int));

			rbTreeNode<string, UserData>* EnemyPtrBuffer = ptr->pRoomIter->RetEnemyUserIter(ptr->isHost);
			string stringBuffer(EnemyPtrBuffer->GetValue().GetNickName());
			int sizeBuffer = stringBuffer.size();
			
			memcpy(ptr->buf + 24, (char*)&sizeBuffer, sizeof(int));
			memcpy(ptr->buf + 28, stringBuffer.data(), sizeBuffer);

			ptr->dataSize = 32 + sizeBuffer;
		}
	}
	else
	{
		//이미 호스트가 이상하거나 시간이 늦은 방.
		memcpy(ptr->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DelayFriendInviteProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	if (rbTreeNode<string, UserData> *pBuffer = ptr->pRoomIter->RetEnemyUserIter(ptr->isHost);  
	pBuffer == nullptr)
	{
		// 방 제거해줌.-> 친구(적) 유저 나간 케이스. 여기서 이대로 끝!
		delete ptr->pRoomIter;
		ptr->pRoomIter = nullptr;

		memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		ptr->dataSize = 8;
	}
	else if (ptr->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_PLAY)
	{
		// 일반적인 친구 입장과 동일하게 처리.
		int _ANSWER_FRIEND_JOIN = Protocol::ANSWER_FRIEND_JOIN;

		memcpy(ptr->buf, reinterpret_cast<const char*>(&_ANSWER_FRIEND_JOIN), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
		ptr->pRoomIter->GetRoomGameData(ptr->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);
		memcpy(ptr->buf + 8, (char*)&retIsHostFirst, sizeof(int));
		memcpy(ptr->buf + 12, (char*)&retPlayerMissionIndex, sizeof(int));
		memcpy(ptr->buf + 16, (char*)&retEnemyMissionIndex, sizeof(int));
		memcpy(ptr->buf + 20, (char*)&retSubMissionIndex, sizeof(int));

		ptr->dataSize = 24;
	}
	// 정상적으로 방삭제 (UDP 손실 가능성에 의한 경우의 수 존재) - 혹은 상대방 타임아웃
	else
	{
		// 야 미안 내가 너방 잘못 만들었어. 
		if (ptr->pRoomIter == pBuffer->SetValue().GetSocketInfo()->pRoomIter) 
		{
			pBuffer->SetValue().GetSocketInfo()->pRoomIter = nullptr;
		}

		delete ptr->pRoomIter;
		ptr->pRoomIter = nullptr;

		memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		ptr->dataSize = 8;
	}
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager)
{
	int iBuffer = reinterpret_cast<int&>(ptr->buf[4]);
	ptr->buf[8 + iBuffer] = '\0';
	
	string idBuffer(ptr->buf + 8);

	std::cout << "[Debug] DemandMakeFriend "<< ptr->pUserNode->GetKey() <<"가 친구요청할 ID는 : " << idBuffer << "입니다. \n";

	bool isOnLogin{ false };
	rbTreeNode<string, UserData>* pBuffer = InUserData.SearchUserNode(idBuffer, isOnLogin);

	// 상대방이 로그인 하지 않은 상황 : ANSWER_MAKE_FRIEND + 0 + 0
	if (!isOnLogin)
	{
		memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(ptr->buf + 8, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		ptr->dataSize = 12;

		//std::cout << "상대방이 로그인 하지 않은 상황 : ANSWER_MAKE_FRIEND + 0 + 0 \n";
		return;
	}

	iBuffer = pBuffer->SetValue().GetFriendStringContSize();

	// 상대방이 맥스 프렌드인 상황 : ANSWER_MAKE_FRIEND + 0 + 1 
	if (iBuffer >= 4)
	{
		memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(ptr->buf + 8, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		ptr->dataSize = 12;
		//std::cout << "상대방이 맥스 프렌드인 상황 : ANSWER_MAKE_FRIEND + 0 + 1  \n";

		return;
	}

	// 상대방이 이미 친구 관련 중인 상황 : ANSWER_MAKE_FRIEND + 0 + 2
	if (pBuffer->SetValue().GetDemandFriendContIndex() != -1)
	{
		memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(ptr->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		ptr->dataSize = 12;
		//std::cout << "상대방이 이미 친구 관련 중인 상황 : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	// 상대방에게 친구 요청을 보냄 : ANSWER_MAKE_FRIEND + 1
	pBuffer->SetValue().SetDemandFriendContIndex(pBuffer->SetValue().SetInsertFriendID(ptr->pUserNode->GetKey()));
	InUDPManager.Push(UDP_PROTOCOL::DEMAND_FRIEND, pBuffer->SetValue().GetSocketInfo());
	//ptr->pUserNode->SetValue().SetInsertFriendID(pBuffer->GetKey());

	memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
	memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

	//std::cout << "상대방에게 친구 요청을 보냄 : ANSWER_MAKE_FRIEND + 1 \n";
	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendInfoProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	string stringBuffer = ptr->pUserNode->SetValue().GetFriendStringWithIndex(ptr->pUserNode->SetValue().GetDemandFriendContIndex());
	int stringSize = stringBuffer.size();

	memcpy(ptr->buf, reinterpret_cast<const char*>(&NOTIFY_MAKE_FRIEND_INFO), sizeof(int));
	memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&stringSize), sizeof(int));
	memcpy(ptr->buf + 8, stringBuffer.data(), stringSize);

	ptr->dataSize = 8 + stringSize;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerMakeFriendProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager)
{
	int iBuffer = reinterpret_cast<int&>(ptr->buf[4]);
	bool bBuffer{ false };

	rbTreeNode<string, UserData> *pBuffer =
		InUserData.SearchUserNode(ptr->pUserNode->GetValue().GetFriendStringWithIndex(ptr->pUserNode->GetValue().GetDemandFriendContIndex()), bBuffer);

	// 친구 거절했다 임마. // 내꺼에서만 알면 되지 않나...?
	if (iBuffer == 0)
	{
		if(bBuffer)
		// 친구 정보 알려줌.

		// 잘라냅시다. // 내부에서 친구 인자정리함.
		ptr->pUserNode->SetValue().SetDeleteFriendID();
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// 친구 신청한 유저가 나가서 친구 등록 프로세스를 진행하기 어려움.
	else if (!bBuffer)
	{
		// 친구 거절 정보 알려줄수가 없음 (이미 나갔는 걸? )
		//InUDPManager.Push(UDP_PROTOCOL::DENY_FRIEND, pBuffer->SetValue().GetSocketInfo());

		// 잘라냅시다. // 내부에서 친구 인자정리함.
		ptr->pUserNode->SetValue().SetDeleteFriendID();
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// 친구 신청한 유저가 존재함. 친구 등록 최종 프로세스 시행. (친구하나 만들기도 힘들다;)
	else
	{
		InUDPManager.Push(UDP_PROTOCOL::RESULT_FRIEND, pBuffer->SetValue().GetSocketInfo());

		pBuffer->SetValue().SetInsertFriendID(ptr->pUserNode->GetKey());
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}

	//항상 똑같은 값 전달.
	memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_ANSWER_MAKE_FRIEND), sizeof(int));

	ptr->dataSize = 8;
}