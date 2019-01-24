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
	, CONST_ANSWER_BUY_ITEM(Protocol::ANSWER_BUY_ITEM)
	, CONST_ANSWER_VIP_CODE(Protocol::ANSWER_VIP_CODE)
	, CONST_ANSWER_BUY_CHARACTER(Protocol::ANSWER_BUY_CHARACTER)
	, CONST_PERMIT_ACTIVE_CHARACTER(Protocol::PERMIT_ACTIVE_CHARACTER)
	, CONST_TRUE(1), CONST_FALSE(0), CONST_2(2), CONST_3(3)
{}

void SCENE_NETWORK_MANAGER::MainUiScene::ProcessData(const int InRecvType, SocketInfo* pClient)
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
	else if (InRecvType == DEMAND_BUY_ITEM)
		_BuyItemProcess(pClient);
	else if (InRecvType == DEMAND_VIP_CODE)
		_VipCodeProcess(pClient);
	else if (InRecvType == DEMAND_BUY_CHARACTER)
		_BuyCharacterProcess(pClient);
	else if (InRecvType == CHANGE_ACTIVE_CHARACTER)
		_ChangeActiveCharacter(pClient);
}

/*
	_DemandFriendInfoProcess
		- 클라이언트에서 친구 창을 킬 경우 보내는 프로토콜을 받았을 경우 실행 되며, 친구 정보를 전송합니다.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInfoProcess(SocketInfo* pClient)
{
	int friendNum = pClient->pUserNode->GetFriendNicknameContSize();

	if (pClient->pUserNode->GetDemandFriendContIndex() != -1)
	{
		friendNum -= 1;
	}

	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_FRIEND_INFO), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<char*>(&friendNum), sizeof(int));

	pClient->dataSize = 8;

	//std::cout << " " << pClient->pUserNode->GetKey() << "의 친구 수는 : " << friendNum << "\n";

	// (최적화) 현재는 해당 요청이 들어 올때마다, 
	// 동일하게 Nickname을 통해 ID를 찾고, 해당 ID를 통해 UserNode를 찾는 행위를 반복하고 있는데,
	// 기존 weak_ptr Cont에서 nullptr이 아닐때의, 리소스 만료 여부를 검사해, 문제가 없다면, 해당 노드의 Get을 활용하는 방안이 필요함.
	// 동일하게 관련 닉네임을 계속 요청할 필요도 없고 상태만 요청하면 됨. 해당에 대한 처리가 최적화할때 꼭 필요해보임.

	if (friendNum > 0)
	{
		int stringSize{};
		Type_Nickname stringBuffer{};
		bool isOnLogin{};
		bool isOnMatch{};
		BYTE characterIndex{};
		shared_ptr<UserData> pBuffer = nullptr;

		for (int i = 0; i < friendNum; ++i)
		{
			// 해당 컨테이너의 i번째 닉네임.
			stringBuffer = pClient->pUserNode->GetFriendNicknameWithIndex(i);

			// 해당 아이디로 현재 상태 판정, True일경우 1, false일 경우 0
			pBuffer = pUserData->SearchUserNodeByNicknameWithActiveCharacterIndex(stringBuffer, isOnLogin, isOnMatch, characterIndex);

			// 0일 경우, 없는 닉네임. 
			// 1일 경우, 닉네임은 있는데 비로그인.
			// 2일 경우, 로비.
			// 3일 경우, 게임중.

			std::cout << "pClient->dataSize : " << pClient->dataSize << std::endl;

			pClient->dataSize += 5;	// 4 Bool Type + 4 characterIndex;

			// 0인 경우, (현재 로직상 이 조건이 트루일 수 없음)
			if (!isOnMatch)
			{
				std::cout << "[Error] 이미 친구추가가 된 유저의 닉네임이 없는 닉네임으로 나옵니다. \n";
				memcpy(pClient->buf + pClient->dataSize - 5, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				//memcpy(pClient->buf + pClient->dataSize - 1, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				pClient->buf[pClient->dataSize - 1] = characterIndex;
				continue;
			}

			if (!isOnLogin)	// 로그인 안했을 때,
				memcpy(pClient->buf + pClient->dataSize - 5, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
			else if (pBuffer->GetSocketInfo()->pRoomIter == nullptr)	// 접속 후, 로비
				memcpy(pClient->buf + pClient->dataSize - 5, reinterpret_cast<const char*>(&CONST_2), sizeof(int));
			else	// 접속 후, 게임중.
				memcpy(pClient->buf + pClient->dataSize - 5, reinterpret_cast<const char*>(&CONST_3), sizeof(int));

			pClient->buf[pClient->dataSize - 1] = characterIndex;

			pClient->pUserNode->SetFreindUserDataWithIndex(pBuffer, i); // weak_ptr로 변환.

			stringSize = MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), NULL, NULL);

			wstring uniStrBuffer(stringSize, 0);
			MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), &uniStrBuffer[0], stringSize);
			
			stringSize = uniStrBuffer.size() * 2;

			memcpy(pClient->buf + pClient->dataSize , reinterpret_cast<char*>(&stringSize), sizeof(int));
			memcpy(pClient->buf + pClient->dataSize + 4, uniStrBuffer.data(), stringSize);

			pClient->dataSize += (4 + stringSize); 
		}
	}
}

/*
	_DemandFriendInviteProcess
		- 클라이언트에서 친구와 같이하기 기능과, 관련 인덱스를 받았을 때 실행되는 함수입니다.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInviteProcess(SocketInfo* pClient)
{
	int indexBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	shared_ptr<UserData> pBuffer = pClient->pUserNode->GetFriendUserDataWithIndex(indexBuffer).lock();

	//pBuffer != nullptr -> 이거는 사실 어느정도 보장됨, 친구 창 UI 키고, Invite하기 전까지, 시간이 길면 문제가 될 수 있기 때문에 조건화
	// 

	// 상대방의 shared_ptr가 지금 nullptr가 아니고, 상대방이 어느 방에도 접속해 있지 않은 경우.  
	if (pBuffer != nullptr)
	{
		// pBuffer가 nullptr일 경우, 문제있을 수도 있어서 조건문 분리.
		if (pBuffer->GetSocketInfo()->pRoomIter == nullptr)
		{
			// 방 생성 필요
			pClient->pRoomIter = make_shared<GameRoom>(pClient->pUserNode, true);

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
	}
	else
	{
		// TCP False Send 필요.
		memcpy(pClient->buf, reinterpret_cast<const char*>(&NOTIFY_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	pBuffer.reset(); // auto.

	pClient->dataSize = 8;
}

/*
	_AnswerFriendInviteProcess
		- 게스트에서, 초대에 대한 대답을 받았을 때 실행되는 함수입니다. ( 1 -> ok , 0 -> no)
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerFriendInviteProcess(SocketInfo* pClient)
{
	int isTrueBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	if (pClient->pRoomIter->GetDynamicFriendInviteBuffer() == true)
	{
		if (isTrueBuffer == 0)
		{
			// 메롱 나 게임 안할껀데?
			pClient->pRoomIter->SetDynamicFriendInviteBuffer();

			// 방 삭제는 호스트가 하도록 변경됨. -> shared_ptr으로 변경됨. 마음대로해.
			pClient->pRoomIter.reset();

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
			Type_Nickname retNickname;

			pClient->pRoomIter->GetRoomGameDataWithNickname(pClient->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex, retNickname);

			// 방 다이나믹 데이터 전송
			memcpy(pClient->buf + 8, reinterpret_cast<char*>(&retIsHostFirst), sizeof(int));
			memcpy(pClient->buf + 12, reinterpret_cast<char*>(&retPlayerMissionIndex), sizeof(int));
			memcpy(pClient->buf + 16, reinterpret_cast<char*>(&retEnemyMissionIndex), sizeof(int));
			memcpy(pClient->buf + 20, reinterpret_cast<char*>(&retSubMissionIndex), sizeof(int));

			auto friendCont = pClient->pUserNode->GetFriendNicknameCont();
			
			int nicknameIndexBuffer = -1;
			for (int i = 0; i < friendCont.size(); ++i)
			{
				if (retNickname.compare(friendCont[i]) == 0)
				{
					nicknameIndexBuffer = i;
					break;
				}
			}

			if (nicknameIndexBuffer == -1) {
				std::cout << "[DEBUG] : 친구목록에 없는 친구에게 초대를 받음\n";
				nicknameIndexBuffer = 0;
			}

			memcpy(pClient->buf + 24, reinterpret_cast<char*>(&nicknameIndexBuffer), sizeof(int));

			pClient->dataSize = 28;
		}
	}
	else
	{
		//이미 호스트가 7초가 지나 방을 나갔거나, 이상해짐.
		if (isTrueBuffer)
		{
			pClient->pRoomIter.reset();
			//pClient->pRoomIter = nullptr;

			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

			pClient->dataSize = 8;
		}
		else
		{
			// 메롱 나 게임 안할껀데?
			pClient->pRoomIter->SetDynamicFriendInviteBuffer();

			// 방 삭제는 호스트가 하도록 변경됨. -> shared_ptr으로 변경됨. 마음대로해.
			pClient->pRoomIter.reset();

			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

			pClient->dataSize = 8;
		}
	}
}

/*
	_DelayFriendInviteProcess
		- 호스트가, 7초가 지난 후, 마지막 Delay 패킷을 보내면 호출되는 함수.

		#0. 경우의 수는 3가지
		0. 친구가 거절을 한 것이 확실한 경우일 때, 방을 뿌시고, 방을 뿌신것을 알림.
		1. ?? 그사이 들어와버렷네... 게임시작!
		2. 아직도 아무 짓도 안함.. 그 외에...UDP소실 같은 거....하....  방을 뿌시고, 방을 뿌신것을 알림.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_DelayFriendInviteProcess(SocketInfo* pClient)
{
	if (pClient->pRoomIter->GetDynamicFriendInviteBuffer() == false)
	{
		// 방 제거해줌.-> 친구(적) 유저 나간 것이 확실한 케이스. 여기서 이대로 끝!
		pClient->pRoomIter.reset();
		//pClient->pRoomIter = nullptr;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 8;
	}
	else if (pClient->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_PLAY)
	{
		// 게임 시작한 것이 확실한 케이스.

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
	
	// 상대방이 어떤 반응이 없었음. ( UDP 소실이거나, 할튼 다른 애매한 케이스. )
	else
	{
		// 기존에는 해당 방을 삭제했다면, 앞으로는 DynamicData를 false로 바꾸도록 변경.
		pClient->pRoomIter->SetDynamicFriendInviteBuffer();

		// shared_ptr로 바뀜.. 여기서도 삭제해줘야함..
		pClient->pRoomIter.reset();

		memcpy(pClient->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		pClient->dataSize = 8;
	}
}

/*
	_DemandMakeFriendProcess
		- 클라이언트에서 받은 닉네임을 통하여, 해당 닉네임의 유저에게 친구 신청이 가능한지 여부를 확인합니다.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendProcess(SocketInfo* pClient)
{
	int iBuffer = reinterpret_cast<int&>(pClient->buf[4]);	// 버퍼 사이즈
	//pClient->buf[8 + iBuffer] = '\0';// wchar는 \0도 두개...
	//pClient->buf[9 + iBuffer] = '\0';// wchar는 \0도 두개...
	
	wstring wideStrBuffer(iBuffer / 2, 0);	// 오버 플로우 가능성!! +1 이 필요할 까?
	memcpy(&wideStrBuffer[0], pClient->buf + 8, iBuffer);

	std::wcout << L"요청한 친구초대는 ~ 입니다. : " << wideStrBuffer << std::endl;

	iBuffer = WideCharToMultiByte(CP_ACP, 0, &wideStrBuffer[0], -1, NULL, 0, NULL, NULL);
	Type_Nickname NicknameBuffer(iBuffer, 0);
	WideCharToMultiByte(CP_ACP, 0, &wideStrBuffer[0], -1, &NicknameBuffer[0], iBuffer, NULL, NULL);

	std::cout << "요청한 친구초대는 ~ 입니다. : " << NicknameBuffer << std::endl;

	bool isOnLogin{ false };
	bool isOnMatch{ false };
	shared_ptr<UserData> pBuffer = pUserData->SearchUserNodeByNickname(NicknameBuffer, isOnLogin, isOnMatch);

	std::cout << "[FRIEND] DEBUG 1 " << std::endl;
	// 해당 닉네임이가 아에 없는 경우 : ANSWER_MAKE_FRIEND + 0 + 3
	if (!isOnMatch)
	{
		std::cout << "[FRIEND] DEBUG 2 " << std::endl;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_3), sizeof(int));

		pClient->dataSize = 12;
		return;
	}

	// 상대방이 로그인 하지 않은 상황 : ANSWER_MAKE_FRIEND + 0 + 0 .
	if (!isOnLogin)
	{
		std::cout << "[FRIEND] DEBUG 3 " << std::endl;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 12;
		return;
	}

	iBuffer = pBuffer->GetFriendNicknameContSize();
	std::cout << "[FRIEND] DEBUG 4 " << std::endl;


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

	// 지금 내가 누구의 친구 관련 중인 상황 일 경우. : ANSWER_MAKE_FRIEND + 0 + 2 위와 동일한 경우를 줌.
	if (pClient->pUserNode->GetDemandFriendContIndex() != -1)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "상대방이 이미 친구 관련 중인 상황 : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	std::cout << "[FRIEND] DEBUG 5 " << std::endl;

	// 상대방에게 친구 요청을 보내고, 나와 친구에 임시로 모두 저장함. : ANSWER_MAKE_FRIEND + 1
	pBuffer->SetDemandFriendContIndex(pBuffer->SetInsertFriendNickname(pClient->pUserNode->GetNickname()));
	pClient->pUserNode->SetDemandFriendContIndex(pClient->pUserNode->SetInsertFriendNickname(pBuffer->GetNickname()));
	
	pUDPManager->Push(UDP_PROTOCOL::DEMAND_FRIEND, pBuffer);
	//pClient->pUserNode->SetValue().SetInsertFriendID(pBuffer->GetKey());

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

	//std::cout << "상대방에게 친구 요청을 보냄 : ANSWER_MAKE_FRIEND + 1 \n";
	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendInfoProcess(SocketInfo* pClient)
{
	Type_Nickname stringBuffer = pClient->pUserNode->GetFriendNicknameWithIndex(pClient->pUserNode->GetDemandFriendContIndex());
	int stringSize = stringBuffer.size(); //DEV_66

	memcpy(pClient->buf, reinterpret_cast<const char*>(&NOTIFY_MAKE_FRIEND_INFO), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&stringSize), sizeof(int));
	memcpy(pClient->buf + 8, stringBuffer.data(), stringSize);

	pClient->dataSize = 8 + stringSize;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerMakeFriendProcess(SocketInfo* pClient)
{
	int iBuffer = reinterpret_cast<int&>(pClient->buf[4]);
	bool bBuffer{ false };
	bool bIsOnMatch{ false };

	shared_ptr<UserData> pBuffer =
		pUserData->SearchUserNodeByNickname(pClient->pUserNode->GetFriendNicknameWithIndex(pClient->pUserNode->GetDemandFriendContIndex()), bBuffer, bIsOnMatch);

	// bIsOnMatch <- 이거 그냥 어쩔수 없이 넣어주긴하는데 여기서는, 무의미하다.

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

		pBuffer->SetDemandFriendContIndex(-1);
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}

	// 앞의 4바이트는 항상 똑같은 값 전달.
	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_ANSWER_MAKE_FRIEND), sizeof(int));

	pClient->dataSize = 8;
}

/*
	_BuyItemProcess
		- 클라이언트에서 받은 아이템 인덱스와 유저의 현재 재화보유량을 확인하여 아이템을 구매합니다.
		- UserData에서 Shop관리하는 것으로 함.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_BuyItemProcess(SocketInfo* pClient)
{
	// BuyItem의 반환값
	int iBuffer = pClient->pUserNode->BuyItem(reinterpret_cast<int&>(pClient->buf[4]));

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CONST_ANSWER_BUY_ITEM), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&iBuffer), sizeof(int));

	// 유저의 현재 돈.
	iBuffer = pClient->pUserNode->GetMoney();
	memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&iBuffer), sizeof(int));

	pClient->dataSize = 12;
}

/*
	_VipCodeProcess
		- 클라이언트에서 받은 vipCode를 확인하여, vipCode 처리를 하고, 필요시, 아이템을 지급합니다.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_VipCodeProcess(SocketInfo* pClient)
{
	const int stringMemoryLength = reinterpret_cast<int&>(pClient->buf[4]);	

	wstring wideStrBuffer(stringMemoryLength / 2, 0);
	memcpy(&wideStrBuffer[0], pClient->buf + 8, stringMemoryLength);

	const int retBuffer = pClient->pUserNode->VipCodeProcess(wideStrBuffer);

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CONST_ANSWER_VIP_CODE), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&retBuffer), sizeof(int));

	pClient->dataSize = 8;
}

/*
	_BuyCharacterProcess
		- 클라이언트에서 받은 구매를 희망하는 캐릭터 인덱스를 확인하여, 처리합니다.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_BuyCharacterProcess(SocketInfo* pClient)
{
	const int retBuffer = pClient->pUserNode->BuyCharacter(reinterpret_cast<int&>(pClient->buf[4]));

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CONST_ANSWER_BUY_CHARACTER), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&retBuffer), sizeof(int));

	pClient->dataSize = 8;
}

/*
	_ChangeActiveCharacter
		- 현재 클라이언트의, 활성화 캐릭터 인덱스를 변경합니다.

	#0. 해당 캐릭터 변경이, 유효한지 여부는 검사하지 않고, 해당 프로토콜을 항상 신뢰합니다.
	#1. NicknameTree에서 자신의 노드를 찾아 Selected Index를 변경합니다. (for Friend)
	#2. 당연히 자신의 UserNode의 activeCharacterIndex 또한 변경합니다.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_ChangeActiveCharacter(SocketInfo* pClient)
{
	pClient->pUserNode->SetActiveCharacterIndex(reinterpret_cast<int&>(pClient->buf[4]));

	//--
	pUserData->SetActiveCharacterIndexWithNickname(pClient->pUserNode->GetNickname(), static_cast<char>(pClient->pUserNode->GetActiveCharacterIndex()));
	//--

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CONST_PERMIT_ACTIVE_CHARACTER), sizeof(int));
	pClient->dataSize = 4;
}
