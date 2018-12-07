#include "../_2_MainUIScene/MainUIScene.h"

#include "../../IOCPServer/SocketInfo.h"
#include "../../IOCPServer/UDPManager.h"

SCENE_NETWORK_MANAGER::MainUiScene::MainUiScene(GameRoomManager* pInRoomData, UserDataManager* pInUserData, UDPManager* pInUDPManager) 
	: BaseScene(pInRoomData, pInUserData, pInUDPManager)
	, PERMIT_FRIEND_INFO(Protocol::PERMIT_FRIEND_INFO), NOTIFY_FRIEND_INVITE(Protocol::NOTIFY_FRIEND_INVITE)
	, GUESTCHECK_FRIEND_INVITE(Protocol::GUESTCHECK_FRIEND_INVITE), HOSTCHECK_FRIEND_INVITE(Protocol::HOSTCHECK_FRIEND_INVITE)
	//, ANSWER_FRIEND_INVITE(Protocol::ANSWER_FRIEND_INVITE)
	, CHECK_DEMAND_MAKE_FRIEND(Protocol::CHECK_DEMAND_MAKE_FRIEND)
	, NOTIFY_MAKE_FRIEND_INFO(Protocol::NOTIFY_MAKE_FRIEND_INFO)
	, CHECK_ANSWER_MAKE_FRIEND(Protocol::CHECK_ANSWER_MAKE_FRIEND)
	, CONST_TRUE(1), CONST_FALSE(0), CONST_2(2), CONST_3(3)
{}

void SCENE_NETWORK_MANAGER::MainUiScene::ProcessData(const int& InRecvType, SocketInfo* pClient)
{
	if (InRecvType == DEMAND_FRIEND_INFO)
		_DemandFriendInfoProcess(pClient);
	else if (InRecvType == DEMAND_FRIEND_INVITE)
		_DemandFriendInviteProcess(pClient);
	else if (InRecvType == ANSWER_FRIEND_INVITE)
		_AnswerFriendInviteProcess(pClient);
	else if (InRecvType == DELAY_FRIEND_INVITE)
		_DelayFriendInviteProcess(pClient);
	else if (InRecvType == DEMAND_MAKE_FRIEND)
		_DemandMakeFriendProcess(pClient);
	else if (InRecvType == DEMAND_MAKE_FRIEND_INFO)
		_DemandMakeFriendInfoProcess(pClient);
	else if (InRecvType == ANSWER_MAKE_FRIEND)
		_AnswerMakeFriendProcess(pClient);
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInfoProcess(SocketInfo* pClient)
{
	int friendNum = pClient->pUserNode->GetFriendNicknameContSize();
	
	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_FRIEND_INFO), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<char*>(&friendNum), sizeof(int));

	pClient->dataSize = 8;

	std::cout << " " << pClient->pUserNode->GetKey() << "의 친구 수는 : " << friendNum << "\n";

	// (최적화) 현재는 해당 요청이 들어 올때마다, 
	// 동일하게 Nickname을 통해 ID를 찾고, 해당 ID를 통해 UserNode를 찾는 행위를 반복하고 있는데,
	// 기존 weak_ptr Cont에서 nullptr이 아닐때의, 리소스 만료 여부를 검사해, 문제가 없다면, 해당 노드의 Get을 활용하는 방안이 필요함.
	if (friendNum)
	{
		int stringSize{};
		wstring stringBuffer{};
		bool isOnLogin{};
		shared_ptr<UserData> pBuffer = nullptr;

		for (int i = 0; i < friendNum; ++i)
		{
			// 해당 컨테이너의 i번째 닉네임.
			stringBuffer = pClient->pUserNode->GetFriendNicknameWithIndex(i);

			// 해당 아이디로 현재 상태 판정, True일경우 1, false일 경우 0
			pBuffer = pUserData->SearchUserNodeWithNickname(stringBuffer, isOnLogin);

			// 로그인 햇을 때, 해당 유저 닉네임 사이즈와, 닉네임, 현재 상태를 보냄.
			if (isOnLogin)
			{
				// 현재 클라이언트의, UserNode에, 친구 소켓 인포 저장
				pClient->pUserNode->SetFreindUserDataWithIndex(pBuffer, i); // weak_ptr로 변환.

				memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

				stringBuffer = pBuffer->GetNickName();	// 반복되는 작업. 필요없는 문구.
				stringSize = stringBuffer.size() * 2;	// DEV_66 사이즈 * 2

				memcpy(pClient->buf + pClient->dataSize + 4, reinterpret_cast<char*>(&stringSize), sizeof(int));
				memcpy(pClient->buf + pClient->dataSize + 8, stringBuffer.data(), stringSize);
				
				pClient->dataSize += (12 + stringSize); // 4 Bool isOnLogin, 4 nickNameSize, 4 (바로 뒷줄) 방 들어가 있는지 없는지 여부.

				if (pBuffer->GetSocketInfo()->pRoomIter != nullptr)
				{
					memcpy(pClient->buf + pClient->dataSize - 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
				}
				else
				{
					memcpy(pClient->buf + pClient->dataSize - 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				}
			}
			// 로그인 하지 않았을 때, 해당 데이터 적재.
			else
			{
				memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				pClient->dataSize += sizeof(int);	// 4 Bool isOnLogin
			}
		}
	}
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInviteProcess(SocketInfo* pClient)
{
	int indexBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	shared_ptr<UserData> pBuffer = pClient->pUserNode->GetFriendUserDataWithIndex(indexBuffer).lock();

	// 상대방의 shared_ptr가 지금 nullptr가 아니고, 상대방이 어느 방에도 접속해 있지 않은 경우.  
	if (pBuffer != nullptr && pBuffer->GetSocketInfo()->pRoomIter == nullptr)
	{
		// 방 생성 필요
		pClient->pRoomIter = new GameRoom(pClient->pUserNode, nullptr, nullptr);

		// 친구 방과 친구의 소켓주소구조체도 등록.	// 이 때 nullptr이 아닐경우 문제가 될 요지가 충분함.! 동기화 처리!필요!
		pBuffer->GetSocketInfo()->pRoomIter = pClient->pRoomIter;
		//pClient->pRoomIter->SetFriendUserPtr(pBuffer);

		// 나는야 방장님이시다.
		pClient->isHost = true;

		// UDP Packet 등록.
		pUDPManager->Push(UDP_PROTOCOL::INVITE_FRIEND, pBuffer);

		// TCP True Send 필요.
		memcpy(pClient->buf, reinterpret_cast<const char*>(&NOTIFY_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}
	else
	{
		// TCP False Send 필요.
		memcpy(pClient->buf, reinterpret_cast<const char*>(&NOTIFY_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// pBuffer.reset(); // auto.

	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerFriendInviteProcess(SocketInfo* pClient)
{
	int isTrueBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	if (pClient->pRoomIter->GetDynamicFriendInviteBuffer() == true)
	{
		if (isTrueBuffer == 0)
		{
			// 방 삭제는 호스트가 하도록 변경됨.
			//delete pClient->pRoomIter;

			// DEV_66 False라고 알림. -> 동기화 오바될 수 있음 (pRoomIter가 그 사이 Destory 될 수 있음)
			pClient->pRoomIter->SetDynamicFriendInviteBuffer();

			// 내 방은 내가 정한다.
			pClient->pRoomIter = nullptr;

			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		
			pClient->dataSize = 8;
		}
		else
		{
			// 게스트로 방접속 처리.
			pClient->isHost = false;

			pClient->pRoomIter->JoinRoom(pClient->pUserNode);

			// 방 접속 허용 데이터 준비.
			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

			// 방 데이터 및 상대편 닉네임 정보 얻기 --> 이거도 당연히 그 전에 알아야하는 거 아닌가? 그렇게 하자.
			int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
			//wstring stringBuffer;

			pClient->pRoomIter->GetRoomGameData(pClient->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

			// 방 다이나믹 데이터 전송
			memcpy(pClient->buf + 8, reinterpret_cast<char*>(&retIsHostFirst), sizeof(int));
			memcpy(pClient->buf + 12, reinterpret_cast<char*>(&retPlayerMissionIndex), sizeof(int));
			memcpy(pClient->buf + 16, reinterpret_cast<char*>(&retEnemyMissionIndex), sizeof(int));
			memcpy(pClient->buf + 20, reinterpret_cast<char*>(&retSubMissionIndex), sizeof(int));

			//int sizeBuffer = stringBuffer.size() * 2; // DEV_66;
			//memcpy(pClient->buf + 24, reinterpret_cast<char*>(&sizeBuffer), sizeof(int));
			//memcpy(pClient->buf + 28, stringBuffer.data(), sizeBuffer);
			//pClient->dataSize = 32 + sizeBuffer;

			pClient->dataSize = 24;
		}
	}
	else
	{
		//이미 호스트가 7초가 지나 방을 나갔거나, 이상해짐.
		delete pClient->pRoomIter;
		pClient->pRoomIter = nullptr;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 8;
	}
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DelayFriendInviteProcess(SocketInfo* pClient)
{
	if (pClient->pRoomIter->GetDynamicFriendInviteBuffer() == false)
	{
		// 방 제거해줌.-> 친구(적) 유저 나간 케이스. 여기서 이대로 끝!
		delete pClient->pRoomIter;
		pClient->pRoomIter = nullptr;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 8;
	}
	else if (pClient->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_PLAY)
	{
		// 게임 시작함.

		// 일반적인 친구 입장과 동일하게 처리.
		int _ANSWER_FRIEND_JOIN = Protocol::ANSWER_FRIEND_JOIN;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&_ANSWER_FRIEND_JOIN), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;

		pClient->pRoomIter->GetRoomGameData(pClient->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);
		memcpy(pClient->buf + 8, (char*)&retIsHostFirst, sizeof(int));
		memcpy(pClient->buf + 12, (char*)&retPlayerMissionIndex, sizeof(int));
		memcpy(pClient->buf + 16, (char*)&retEnemyMissionIndex, sizeof(int));
		memcpy(pClient->buf + 20, (char*)&retSubMissionIndex, sizeof(int));

		pClient->dataSize = 24;
	}
	// 상대방이 어떤 반응이 없었음. (동기화 처리 필요함. )
	else
	{
		// 기존에는 해당 방을 삭제했다면, 앞으로는 DynamicData를 false로 바꾸도록 변경.
		pClient->pRoomIter->SetDynamicFriendInviteBuffer();

		memcpy(pClient->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		pClient->dataSize = 8;
	}
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendProcess(SocketInfo* pClient)
{
	int iBuffer = reinterpret_cast<int&>(pClient->buf[4]);
	pClient->buf[8 + iBuffer] = '\0';
	
	string idBuffer(pClient->buf + 8);

	std::cout << "[Debug] DemandMakeFriend "<< pClient->pUserNode->GetKey() <<"가 친구요청할 ID는 : " << idBuffer << "입니다. \n";

	bool isOnLogin{ false };
	shared_ptr<UserData> pBuffer = pUserData->SearchUserNode(idBuffer, isOnLogin);

	// 상대방이 로그인 하지 않은 상황 : ANSWER_MAKE_FRIEND + 0 + 0
	if (!isOnLogin)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 12;

		//std::cout << "상대방이 로그인 하지 않은 상황 : ANSWER_MAKE_FRIEND + 0 + 0 \n";
		return;
	}

	iBuffer = pBuffer->GetFriendNicknameContSize();

	// 상대방이 맥스 프렌드인 상황 : ANSWER_MAKE_FRIEND + 0 + 1 
	if (iBuffer >= 4)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "상대방이 맥스 프렌드인 상황 : ANSWER_MAKE_FRIEND + 0 + 1  \n";

		return;
	}

	// 상대방이 이미 친구 관련 중인 상황 : ANSWER_MAKE_FRIEND + 0 + 2
	if (pBuffer->GetDemandFriendContIndex() != -1)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "상대방이 이미 친구 관련 중인 상황 : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	// 상대방에게 친구 요청을 보냄 : ANSWER_MAKE_FRIEND + 1
	pBuffer->SetDemandFriendContIndex(pBuffer->SetInsertFriendID(pClient->pUserNode->GetNickName()));
	pUDPManager->Push(UDP_PROTOCOL::DEMAND_FRIEND, pBuffer);
	//pClient->pUserNode->SetValue().SetInsertFriendID(pBuffer->GetKey());

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

	//std::cout << "상대방에게 친구 요청을 보냄 : ANSWER_MAKE_FRIEND + 1 \n";
	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendInfoProcess(SocketInfo* pClient)
{
	wstring stringBuffer = pClient->pUserNode->GetFriendNicknameWithIndex(pClient->pUserNode->GetDemandFriendContIndex());
	int stringSize = stringBuffer.size();

	memcpy(pClient->buf, reinterpret_cast<const char*>(&NOTIFY_MAKE_FRIEND_INFO), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&stringSize), sizeof(int));
	memcpy(pClient->buf + 8, stringBuffer.data(), stringSize);

	pClient->dataSize = 8 + stringSize;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerMakeFriendProcess(SocketInfo* pClient)
{
	int iBuffer = reinterpret_cast<int&>(pClient->buf[4]);
	bool bBuffer{ false };

	shared_ptr<UserData> pBuffer =
		pUserData->SearchUserNodeWithNickname(pClient->pUserNode->GetFriendNicknameWithIndex(pClient->pUserNode->GetDemandFriendContIndex()), bBuffer);

	// 친구 거절했다 임마. // 내꺼에서만 알면 되지 않나...?
	if (iBuffer == 0)
	{
		if(bBuffer)
		// 친구 정보 알려줌.

		// 잘라냅시다. // 내부에서 친구 인자정리함.
		pClient->pUserNode->SetDeleteFriendID();
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// 친구 신청한 유저가 나가서 친구 등록 프로세스를 진행하기 어려움.
	else if (!bBuffer)
	{
		// 친구 거절 정보 알려줄수가 없음 (이미 나갔는 걸? )
		//InUDPManager.Push(UDP_PROTOCOL::DENY_FRIEND, pBuffer->SetValue().GetSocketInfo());

		// 잘라냅시다. // 내부에서 친구 인자정리함.
		pClient->pUserNode->SetDeleteFriendID();
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// 친구 신청한 유저가 존재함. 친구 등록 최종 프로세스 시행. (친구하나 만들기도 힘들다;)
	else
	{
		std::cout << " " << pBuffer->GetKey() << "님이 보낸 친구초대를 " << pClient->pUserNode->GetKey() << "님이 받았어요! \n";
		pUDPManager->Push(UDP_PROTOCOL::RESULT_FRIEND, pBuffer /*pClient->pUserNode->SetValue().GetSocketInfo()*/);

		pBuffer->SetInsertFriendID(pClient->pUserNode->GetNickName());
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}

	//항상 똑같은 값 전달.
	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_ANSWER_MAKE_FRIEND), sizeof(int));

	pClient->dataSize = 8;
}