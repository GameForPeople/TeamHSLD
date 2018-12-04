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
	, CONST_TRUE(1) , CONST_FALSE (0), CONST_2(2), CONST_3(3)
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
	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_FRIEND_INFO), sizeof(int));

	//UserData buffer = 
	int friendNum = pClient->pUserNode->SetValue().GetFriendStringCont().size();
	memcpy(pClient->buf + 4, reinterpret_cast<char*>(&friendNum), sizeof(int));

	pClient->dataSize = 8;

	std::cout << " " << pClient->pUserNode->GetKey() << "의 친구 수는 : " << friendNum << "\n";

	if (friendNum)
	{
		int stringSize{};
		string stringBuffer{};
		bool isOnLogin{};
		rbTreeNode<string, UserData> *pBuffer = nullptr;

		for (int i = 0; i < friendNum; ++i)
		{
			// 해당 컨테이너의 i번째 아이디 스트링의 사이즈.
			stringBuffer = pClient->pUserNode->SetValue().GetFriendStringCont()[i];
				//stringSize = stringBuffer.size();
			std::cout << " " << pClient->pUserNode->GetKey() << "의 친구 : " << stringBuffer << "\n";

			// 아이디 스트링 사이즈와, 아이디 적재.
				//memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<char*>(&stringSize), sizeof(int));
				//memcpy(pClient->buf + pClient->dataSize + 4, stringBuffer.data(), stringSize);
				//pClient->dataSize += 4 + stringSize;

			// 해당 아이디로 현재 상태 판정, True일경우 1, false일 경우 0
			pBuffer = pUserData->SearchUserNode(stringBuffer, isOnLogin);

			// 로그인 햇을 때, 해당 유저 닉네임 사이즈와, 닉네임, 현재 상태를 보냄.
			if (isOnLogin)
			{
				// 현재 클라이언트의, UserNode에, 친구 소켓 인포 저장
					//pClient->pUserNode->SetValue().SetFreindSocketInfo(pBuffer->SetValue().GetSocketInfo(), i); 				// 이거 쪼금 위험할 수도 있음. 테스트 필요.

				memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));


				stringBuffer = pBuffer->SetValue().GetNickName();
				stringSize = stringBuffer.size();

				memcpy(pClient->buf + pClient->dataSize + 4, reinterpret_cast<char*>(&stringSize), sizeof(int));
				memcpy(pClient->buf + pClient->dataSize + 8, stringBuffer.data(), stringSize);
				
				pClient->dataSize += (12 + stringSize); // 4 Bool isOnLogin, 4 nickNameSize, 4 (바로 뒷줄) 방 들어가 있는지 없는지 여부.

				if (pBuffer->GetValue().GetSocketInfo()->pRoomIter != nullptr)
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

	// 기존에 저장한 거 쓰지 말고, 새로 Search해서 씁시다? 성능 버리고, 안정성?
	//pClient->pUserNode->SetValue().GetFriendSocketInfo(indexBuffer);
	bool isOnUser{ false };

	rbTreeNode<string, UserData> *pBuffer = pUserData->SearchUserNode(pClient->pUserNode->SetValue().GetFriendStringWithIndex(indexBuffer), isOnUser);

	if (isOnUser && pBuffer->SetValue().GetSocketInfo()->pRoomIter == nullptr)
	{
		// 방 생성 필요
		pClient->pRoomIter = new GameRoom(pClient->pUserNode, nullptr, nullptr);

		// 나는야 방장님이시다.
		pClient->isHost = true;

		// 친구 방과 친구의 소켓주소구조체도 등록.
		pBuffer->SetValue().GetSocketInfo()->pRoomIter = pClient->pRoomIter;
		pClient->pRoomIter->SetFriendUserPtr(pBuffer);

		// UDP Packet 등록.
		pUDPManager->Push(UDP_PROTOCOL::INVITE_FRIEND, pBuffer->SetValue().GetSocketInfo());

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

	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerFriendInviteProcess(SocketInfo* pClient)
{
	int isTrueBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	if (pClient->pRoomIter != nullptr)
	{
		if (isTrueBuffer == 0)
		{
			// 동기화 오바칠 수 있음.
			pClient->pRoomIter->SetUserPtr(true, nullptr);

			// 방 삭제 호스트가 하도록 변경됨.
			//delete pClient->pRoomIter;
			//pClient->pRoomIter = nullptr;

			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		}
		else
		{
			pClient->isHost = false;
			pClient->pRoomIter->JoinRoom(pClient->pUserNode);

			// 방 접속.
			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

			int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
			pClient->pRoomIter->GetRoomGameData(pClient->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

			memcpy(pClient->buf + 8, reinterpret_cast<char*>(&retIsHostFirst), sizeof(int));
			memcpy(pClient->buf + 12, reinterpret_cast<char*>(&retPlayerMissionIndex), sizeof(int));
			memcpy(pClient->buf + 16, reinterpret_cast<char*>(&retEnemyMissionIndex), sizeof(int));
			memcpy(pClient->buf + 20, reinterpret_cast<char*>(&retSubMissionIndex), sizeof(int));

			rbTreeNode<string, UserData>* EnemypClientBuffer = pClient->pRoomIter->RetEnemyUserIter(pClient->isHost);
			string stringBuffer(EnemypClientBuffer->GetValue().GetNickName());
			int sizeBuffer = stringBuffer.size();
			
			std::cout << "보내는 닉네임은 : " << stringBuffer << "입니다. 사이즈는 "<< sizeBuffer << " \n";

			memcpy(pClient->buf + 24, reinterpret_cast<char*>(&sizeBuffer), sizeof(int));
			memcpy(pClient->buf + 28, stringBuffer.data(), sizeBuffer);

			pClient->dataSize = 32 + sizeBuffer;
		}
	}
	else
	{
		//이미 호스트가 이상하거나 시간이 늦은 방.
		memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DelayFriendInviteProcess(SocketInfo* pClient)
{
	if (rbTreeNode<string, UserData> *pBuffer = pClient->pRoomIter->RetEnemyUserIter(pClient->isHost);  
	pBuffer == nullptr)
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
	// 정상적으로 방삭제 (UDP 손실 가능성에 의한 경우의 수 존재) - 혹은 상대방 타임아웃
	else
	{
		// 야 미안 내가 너방 잘못 만들었어. 
		if (pClient->pRoomIter == pBuffer->SetValue().GetSocketInfo()->pRoomIter) 
		{
			pBuffer->SetValue().GetSocketInfo()->pRoomIter = nullptr;
		}

		delete pClient->pRoomIter;
		pClient->pRoomIter = nullptr;

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
	rbTreeNode<string, UserData>* pBuffer = pUserData->SearchUserNode(idBuffer, isOnLogin);

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

	iBuffer = pBuffer->SetValue().GetFriendStringContSize();

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
	if (pBuffer->SetValue().GetDemandFriendContIndex() != -1)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "상대방이 이미 친구 관련 중인 상황 : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	// 상대방에게 친구 요청을 보냄 : ANSWER_MAKE_FRIEND + 1
	pBuffer->SetValue().SetDemandFriendContIndex(pBuffer->SetValue().SetInsertFriendID(pClient->pUserNode->GetKey()));
	pUDPManager->Push(UDP_PROTOCOL::DEMAND_FRIEND, pBuffer->SetValue().GetSocketInfo());
	//pClient->pUserNode->SetValue().SetInsertFriendID(pBuffer->GetKey());

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

	//std::cout << "상대방에게 친구 요청을 보냄 : ANSWER_MAKE_FRIEND + 1 \n";
	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendInfoProcess(SocketInfo* pClient)
{
	string stringBuffer = pClient->pUserNode->SetValue().GetFriendStringWithIndex(pClient->pUserNode->SetValue().GetDemandFriendContIndex());
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

	rbTreeNode<string, UserData> *pBuffer =
		pUserData->SearchUserNode(pClient->pUserNode->GetValue().GetFriendStringWithIndex(pClient->pUserNode->GetValue().GetDemandFriendContIndex()), bBuffer);

	// 친구 거절했다 임마. // 내꺼에서만 알면 되지 않나...?
	if (iBuffer == 0)
	{
		if(bBuffer)
		// 친구 정보 알려줌.

		// 잘라냅시다. // 내부에서 친구 인자정리함.
		pClient->pUserNode->SetValue().SetDeleteFriendID();
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// 친구 신청한 유저가 나가서 친구 등록 프로세스를 진행하기 어려움.
	else if (!bBuffer)
	{
		// 친구 거절 정보 알려줄수가 없음 (이미 나갔는 걸? )
		//InUDPManager.Push(UDP_PROTOCOL::DENY_FRIEND, pBuffer->SetValue().GetSocketInfo());

		// 잘라냅시다. // 내부에서 친구 인자정리함.
		pClient->pUserNode->SetValue().SetDeleteFriendID();
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// 친구 신청한 유저가 존재함. 친구 등록 최종 프로세스 시행. (친구하나 만들기도 힘들다;)
	else
	{
		std::cout << " " << pBuffer->GetKey() << "님이 보낸 친구초대를 " << pClient->pUserNode->GetKey() << "님이 받았어요! \n";
		pUDPManager->Push(UDP_PROTOCOL::RESULT_FRIEND, pBuffer->SetValue().GetSocketInfo() /*pClient->pUserNode->SetValue().GetSocketInfo()*/);

		pBuffer->SetValue().SetInsertFriendID(pClient->pUserNode->GetKey());
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}

	//항상 똑같은 값 전달.
	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_ANSWER_MAKE_FRIEND), sizeof(int));

	pClient->dataSize = 8;
}