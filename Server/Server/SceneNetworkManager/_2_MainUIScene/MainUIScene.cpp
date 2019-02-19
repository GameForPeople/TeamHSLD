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
		- Ŭ���̾�Ʈ���� ģ�� â�� ų ��� ������ ���������� �޾��� ��� ���� �Ǹ�, ģ�� ������ �����մϴ�.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInfoProcess(SocketInfo* pClient)
{
	std::cout << "[DEMAND_FRIEND] DEBUG - 1 \n";

	int friendNum = pClient->pUserNode->GetFriendNicknameContSize();

	//if (pClient->pUserNode->GetTemporaryFriendUserData().expired() == false)
	//{
	//	friendNum -= 1;
	//}

	std::cout << "[DEMAND_FRIEND] DEBUG - 2 \n";

	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_FRIEND_INFO), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<char*>(&friendNum), sizeof(int));

	std::cout << "[DEMAND_FRIEND] DEBUG - 3 \n";

	pClient->dataSize = 8;

	//std::cout << " " << pClient->pUserNode->GetKey() << "�� ģ�� ���� : " << friendNum << "\n";

	// (����ȭ) ����� �ش� ��û�� ��� �ö�����, 
	// �����ϰ� Nickname�� ���� ID�� ã��, �ش� ID�� ���� UserNode�� ã�� ������ �ݺ��ϰ� �ִµ�,
	// ���� weak_ptr Cont���� nullptr�� �ƴҶ���, ���ҽ� ���� ���θ� �˻���, ������ ���ٸ�, �ش� ����� Get�� Ȱ���ϴ� ����� �ʿ���.
	// �����ϰ� ���� �г����� ��� ��û�� �ʿ䵵 ���� ���¸� ��û�ϸ� ��. �ش翡 ���� ó���� ����ȭ�Ҷ� �� �ʿ��غ���.

	if (friendNum > 0)
	{
		std::cout << "[DEMAND_FRIEND] DEBUG - 4 \n";

		int stringSize{};
		Type_Nickname stringBuffer{};
		bool isOnLogin{};
		bool isOnMatch{};
		BYTE characterIndex{};
		shared_ptr<UserData> pBuffer = nullptr;

		for (int i = 0; i < friendNum; ++i)
		{
			std::cout << "[DEMAND_FRIEND] DEBUG - 5 \n";

			// �ش� �����̳��� i��° �г���.
			stringBuffer = pClient->pUserNode->GetFriendNicknameWithIndex(i);

			std::cout << "[DEMAND_FRIEND] DEBUG - 6 \n";

			// �ش� ���̵�� ���� ���� ����, True�ϰ�� 1, false�� ��� 0
			pBuffer = pUserData->SearchUserNodeByNicknameWithActiveCharacterIndex(stringBuffer, isOnLogin, isOnMatch, characterIndex);

			std::cout << "[DEMAND_FRIEND] DEBUG - 7 \n";

			// 0�� ���, ���� �г���. 
			// 1�� ���, �г����� �ִµ� ��α���.
			// 2�� ���, �κ�.
			// 3�� ���, ������.

			//std::cout << "pClient->dataSize : " << pClient->dataSize << std::endl;

			pClient->dataSize += 5;	// 4 Bool Type + 4 characterIndex;

			std::cout << "[DEMAND_FRIEND] DEBUG - 8 \n";

			// 0�� ���, (���� ������ �� ������ Ʈ���� �� ����)
			if (!isOnMatch)
			{
				std::cout << "[Error] �̹� ģ���߰��� �� ������ �г����� ���� �г������� ���ɴϴ�. \n";
				memcpy(pClient->buf + pClient->dataSize - 5, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				//memcpy(pClient->buf + pClient->dataSize - 1, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				pClient->buf[pClient->dataSize - 1] = characterIndex;
				continue;
			}

			std::cout << "[DEMAND_FRIEND] DEBUG - 9 \n";

			if (!isOnLogin)	// �α��� ������ ��,
				memcpy(pClient->buf + pClient->dataSize - 5, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
			else if (pBuffer->GetSocketInfo()->pRoomIter == nullptr)	// ���� ��, �κ�
				memcpy(pClient->buf + pClient->dataSize - 5, reinterpret_cast<const char*>(&CONST_2), sizeof(int));
			else	// ���� ��, ������.
				memcpy(pClient->buf + pClient->dataSize - 5, reinterpret_cast<const char*>(&CONST_3), sizeof(int));

			std::cout << "[DEMAND_FRIEND] DEBUG - 10 \n";

			pClient->buf[pClient->dataSize - 1] = characterIndex;

			std::cout << "[DEMAND_FRIEND] DEBUG - 11 \n";

			pClient->pUserNode->SetFreindUserDataWithIndex(pBuffer, i); // weak_ptr�� ��ȯ.

			std::cout << "[DEMAND_FRIEND] DEBUG - 12 \n";

			stringSize = MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), NULL, NULL);

			std::cout << "[DEMAND_FRIEND] DEBUG - 13 \n";

			wstring uniStrBuffer(stringSize, 0);
			MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), &uniStrBuffer[0], stringSize);
			
			std::cout << "[DEMAND_FRIEND] DEBUG - 14 \n";

			stringSize = uniStrBuffer.size() * 2;

			memcpy(pClient->buf + pClient->dataSize , reinterpret_cast<char*>(&stringSize), sizeof(int));
			memcpy(pClient->buf + pClient->dataSize + 4, uniStrBuffer.data(), stringSize);

			std::cout << "[DEMAND_FRIEND] DEBUG - 15 \n";

			pClient->dataSize += (4 + stringSize); 
		}
	}
}

/*
	_DemandFriendInviteProcess
		- Ŭ���̾�Ʈ���� ģ���� �����ϱ� ��ɰ�, ���� �ε����� �޾��� �� ����Ǵ� �Լ��Դϴ�.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInviteProcess(SocketInfo* pClient)
{
	int indexBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	shared_ptr<UserData> pBuffer = pClient->pUserNode->GetFriendUserDataWithIndex(indexBuffer).lock();

	//pBuffer != nullptr -> �̰Ŵ� ��� ������� �����, ģ�� â UI Ű��, Invite�ϱ� ������, �ð��� ��� ������ �� �� �ֱ� ������ ����ȭ
	// 

	// ������ shared_ptr�� ���� nullptr�� �ƴϰ�, ������ ��� �濡�� ������ ���� ���� ���.  
	if (pBuffer != nullptr)
	{
		// pBuffer�� nullptr�� ���, �������� ���� �־ ���ǹ� �и�.
		if (pBuffer->GetSocketInfo()->pRoomIter == nullptr)
		{
			// �� ���� �ʿ�
			pClient->pRoomIter = make_shared<GameRoom>(pClient->pUserNode, true, true);

			// ģ�� ��� ģ���� �����ּұ���ü�� ���.	// �� �� nullptr�� �ƴҰ�� ������ �� ������ �����.! ����ȭ ó��!�ʿ�!
			pBuffer->GetSocketInfo()->pRoomIter = pClient->pRoomIter;
			//pClient->pRoomIter->SetFriendUserPtr(pBuffer);

			// ���¾� ������̽ô�.
			pClient->isHost = true;

			// UDP Packet ���.
			pUDPManager->Push(UDP_PROTOCOL::INVITE_FRIEND, pBuffer);

			// TCP True Send �ʿ�.
			memcpy(pClient->buf, reinterpret_cast<const char*>(&NOTIFY_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
		}
		else
		{
			// TCP False Send �ʿ�.
			memcpy(pClient->buf, reinterpret_cast<const char*>(&NOTIFY_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		}
	}
	else
	{
		// TCP False Send �ʿ�.
		memcpy(pClient->buf, reinterpret_cast<const char*>(&NOTIFY_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	pBuffer.reset(); // auto.

	pClient->dataSize = 8;
}

/*
	_AnswerFriendInviteProcess
		- �Խ�Ʈ����, �ʴ뿡 ���� ����� �޾��� �� ����Ǵ� �Լ��Դϴ�. ( 1 -> ok , 0 -> no)
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerFriendInviteProcess(SocketInfo* pClient)
{
	int isTrueBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	if (pClient->pRoomIter->GetDynamicFriendInviteBuffer() == true)
	{
		if (isTrueBuffer == 0)
		{
			// �޷� �� ���� ���Ҳ���?
			pClient->pRoomIter->SetDynamicFriendInviteBuffer();

			// �� ������ ȣ��Ʈ�� �ϵ��� �����. -> shared_ptr���� �����. ���������.
			pClient->pRoomIter.reset();

			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		
			pClient->dataSize = 8;
		}
		else
		{
			// �Խ�Ʈ�� ������ ó��.
			pClient->isHost = false;

			pClient->pRoomIter->JoinRoom(pClient->pUserNode);

			// �� ���� ��� ������ �غ�.
			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

			// �� ������ �� ����� �г��� ���� ��� --> �̰ŵ� �翬�� �� ���� �˾ƾ��ϴ� �� �ƴѰ�? �׷��� ����.
			int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex, retCharacterIndex;
			Type_Nickname retNickname;

			pClient->pRoomIter->GetRoomGameDataWithNickname(pClient->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex, retCharacterIndex, retNickname);

			// �� ���̳��� ������ ����
			memcpy(pClient->buf + 8, reinterpret_cast<char*>(&retIsHostFirst), sizeof(int));
			memcpy(pClient->buf + 12, reinterpret_cast<char*>(&retPlayerMissionIndex), sizeof(int));
			memcpy(pClient->buf + 16, reinterpret_cast<char*>(&retEnemyMissionIndex), sizeof(int));
			memcpy(pClient->buf + 20, reinterpret_cast<char*>(&retSubMissionIndex), sizeof(int));
			memcpy(pClient->buf + 24, reinterpret_cast<char*>(&retCharacterIndex), sizeof(int));

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
				std::cout << "    [Friend] : ģ����Ͽ� ���� ģ������ �ʴ븦 ����\n";
				nicknameIndexBuffer = 0;
			}

			memcpy(pClient->buf + 28, reinterpret_cast<char*>(&nicknameIndexBuffer), sizeof(int));

			pClient->dataSize = 32;
		}
	}
	else
	{
		//�̹� ȣ��Ʈ�� 7�ʰ� ���� ���� �����ų�, �̻�����.
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
			// �޷� �� ���� ���Ҳ���?
			pClient->pRoomIter->SetDynamicFriendInviteBuffer();

			// �� ������ ȣ��Ʈ�� �ϵ��� �����. -> shared_ptr���� �����. ���������.
			pClient->pRoomIter.reset();

			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

			pClient->dataSize = 8;
		}
	}
}

/*
	_DelayFriendInviteProcess
		- ȣ��Ʈ��, 7�ʰ� ���� ��, ������ Delay ��Ŷ�� ������ ȣ��Ǵ� �Լ�.

		#0. ����� ���� 3����
		0. ģ���� ������ �� ���� Ȯ���� ����� ��, ���� �ѽð�, ���� �ѽŰ��� �˸�.
		1. ?? �׻��� ���͹��ǳ�... ���ӽ���!
		2. ������ �ƹ� ���� ����.. �� �ܿ�...UDP�ҽ� ���� ��....��....  ���� �ѽð�, ���� �ѽŰ��� �˸�.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_DelayFriendInviteProcess(SocketInfo* pClient)
{
	if (pClient->pRoomIter->GetDynamicFriendInviteBuffer() == false)
	{
		// �� ��������.-> ģ��(��) ���� ���� ���� Ȯ���� ���̽�. ���⼭ �̴�� ��!
		pClient->pRoomIter.reset();
		//pClient->pRoomIter = nullptr;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 8;
	}
	else if (pClient->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_PLAY)
	{
		// ���� ������ ���� Ȯ���� ���̽�.

		// �Ϲ����� ģ�� ����� �����ϰ� ó��.
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
	
	// ������ � ������ ������. ( UDP �ҽ��̰ų�, ��ư �ٸ� �ָ��� ���̽�. )
	else
	{
		// �������� �ش� ���� �����ߴٸ�, �����δ� DynamicData�� false�� �ٲٵ��� ����.
		pClient->pRoomIter->SetDynamicFriendInviteBuffer();

		// shared_ptr�� �ٲ�.. ���⼭�� �����������..
		pClient->pRoomIter.reset();

		memcpy(pClient->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		pClient->dataSize = 8;
	}
}

/*
	_DemandMakeFriendProcess
		- Ŭ���̾�Ʈ���� ���� �г����� ���Ͽ�, �ش� �г����� �������� ģ�� ��û�� �������� ���θ� Ȯ���մϴ�.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendProcess(SocketInfo* pClient)
{
	std::cout << pClient->pUserNode->GetNickname() << "�� ���� DemandMakeFriendProcess�� ȣ��Ǿ����ϴ�. \n";

	int iBuffer = reinterpret_cast<int&>(pClient->buf[4]);	// ���� ������
	//pClient->buf[8 + iBuffer] = '\0';// wchar�� \0�� �ΰ�...
	//pClient->buf[9 + iBuffer] = '\0';// wchar�� \0�� �ΰ�...
	
	wstring wideStrBuffer(iBuffer / 2, 0);	// ���� �÷ο� ���ɼ�!! +1 �� �ʿ��� ��?
	memcpy(&wideStrBuffer[0], pClient->buf + 8, iBuffer);

	Type_Nickname NicknameBuffer = CONVERT_UTIL::WStringToString(wideStrBuffer);

	//std::cout << "��û�� ģ���ʴ�� ~ �Դϴ�. : " << NicknameBuffer << " ������� : " << NicknameBuffer.size()<< std::endl;

	bool isOnLogin{ false };
	bool isOnMatch{ false };
	shared_ptr<UserData> pBuffer = pUserData->SearchUserNodeByNickname(NicknameBuffer, isOnLogin, isOnMatch);

	// �ش� �г����̰� �ƿ� ���� ��� : ANSWER_MAKE_FRIEND + 0 + 3
	if (!isOnMatch)
	{
		//std::cout << "[FRIEND] DEBUG 2 " << std::endl;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_3), sizeof(int));

		pClient->dataSize = 12;
		return;
	}

	// ������ �α��� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 0 .
	if (!isOnLogin)
	{
		//std::cout << "[FRIEND] DEBUG 3 " << std::endl;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 12;
		return;
	}

	iBuffer = pBuffer->GetFriendNicknameContSize();
	//std::cout << "[FRIEND] DEBUG 4 " << std::endl;

	// ������ �ƽ� �������� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 1 
	if (iBuffer >= 4)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "������ �ƽ� �������� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 1  \n";

		return;
	}

	// ������ �̹� ģ�� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 2
	if (pBuffer->GetTemporaryFriendUserData().use_count() != 0)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "������ �̹� ģ�� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	// ���� ���� ������ ģ�� ���� ���� ��Ȳ �� ���. : ANSWER_MAKE_FRIEND + 0 + 2 ���� ������ ��츦 ��.
	if (pClient->pUserNode->GetTemporaryFriendUserData().use_count() != 0)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "������ �̹� ģ�� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	//std::cout << "[FRIEND] DEBUG 5 " << std::endl;

	// ���濡�� ģ�� ��û�� ������, ���� ģ���� �ӽ÷� ��� ������. : ANSWER_MAKE_FRIEND + 1
	//pBuffer->SetDemandFriendContIndex(pBuffer->SetInsertFriendNickname(pClient->pUserNode->GetNickname()));
	//pClient->pUserNode->SetDemandFriendContIndex(pClient->pUserNode->SetInsertFriendNickname(pBuffer->GetNickname()));
	pBuffer->SetTemporaryFriendUserData(pClient->pUserNode);
	pClient->pUserNode->SetTemporaryFriendUserData(pBuffer);

	pUDPManager->Push(UDP_PROTOCOL::DEMAND_FRIEND, pBuffer);
	//pClient->pUserNode->SetValue().SetInsertFriendID(pBuffer->GetKey());

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

	//std::cout << "���濡�� ģ�� ��û�� ���� : ANSWER_MAKE_FRIEND + 1 \n";
	pClient->dataSize = 8;

	std::cout << pClient->pUserNode->GetNickname() << "�� ���� DemandMakeFriendProcess�� ���ϵǾ����ϴ�. \n";
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendInfoProcess(SocketInfo* pClient)
{
	std::cout << pClient->pUserNode->GetNickname() << "�� ���� _DemandMakeFriendInfoProcess�� ȣ��Ǿ����ϴ�. \n";

	if (auto pRetNode = pClient->pUserNode->GetTemporaryFriendUserData().lock()
		; pRetNode != nullptr) 
	{
		std::wstring stringBuffer = CONVERT_UTIL::StringToWString(pRetNode->GetNickname());
		int stringSize = stringBuffer.size() * 2; 

		memcpy(pClient->buf, reinterpret_cast<const char*>(&NOTIFY_MAKE_FRIEND_INFO), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&stringSize), sizeof(int));
		memcpy(pClient->buf + 8, stringBuffer.data(), stringSize);

		pClient->dataSize = 8 + stringSize;
	}

	std::cout << pClient->pUserNode->GetNickname() << "�� ���� _DemandMakeFriendInfoProcess�� ���ϵǾ����ϴ�. \n";
}

void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerMakeFriendProcess(SocketInfo* pClient)
{
	std::cout << pClient->pUserNode->GetNickname() << "�� ���� _AnswerMakeFriendProcess�� ȣ��Ǿ����ϴ�. \n";

	int iBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	std::cout << "[ANSWER_FRIEND] DEBUG - 1 \n";
	//shared_ptr<UserData> pBuffer =
	//	pUserData->SearchUserNodeByNickname(pClient->pUserNode->GetFriendNicknameWithIndex(pClient->pUserNode->GetDemandFriendContIndex()), bBuffer, bIsOnMatch);
	auto pBuffer = pClient->pUserNode->GetTemporaryFriendUserData().lock();

	std::cout << "[ANSWER_FRIEND] DEBUG - 2 \n";

	// ģ�� �����ߴ� �Ӹ�. // ���������� �˸� ���� �ʳ�...?
	if (iBuffer == 0)
	{
		if (pBuffer != nullptr)
		{
			std::cout << "[ANSWER_FRIEND] DEBUG - 3 \n";

			pClient->pUserNode->SetDeleteFriendID();
			pBuffer->SetDeleteFriendID();
		}

		std::cout << "[ANSWER_FRIEND] DEBUG - 4 \n";

		pUDPManager->Push(UDP_PROTOCOL::RESULT_FAIL_FRIEND, pBuffer /*pClient->pUserNode->SetValue().GetSocketInfo()*/);

		std::cout << "[ANSWER_FRIEND] DEBUG - 5 \n";

		// �߶���ô�. // ���ο��� ģ�� ����������.
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}
	
	// ģ�� ��û�� ������ ������ ģ�� ��� ���μ����� �����ϱ� �����.
	else if (pBuffer == nullptr)
	{
		// ģ�� ���� ���� �˷��ټ��� ���� (�̹� ������ ��? )
		//InUDPManager.Push(UDP_PROTOCOL::DENY_FRIEND, pBuffer->SetValue().GetSocketInfo());

		std::cout << "[ANSWER_FRIEND] DEBUG - 6 \n";

		// �߶���ô�. // ���ο��� ģ�� ����������.
		pClient->pUserNode->SetDeleteFriendID();
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// ģ�� ��û�� ������ ������. ģ�� ��� ���� ���μ��� ����. (ģ���ϳ� ����⵵ �����;)
	else
	{
#ifdef _DEBUG_MODE_
		std::cout << "   [Friend]" << pBuffer->GetKey() << "���� ���� ģ���ʴ븦 " << pClient->pUserNode->GetKey() << "���� �޾Ҿ��! \n";
#endif // _DEBUG_MODE_

		std::cout << "[ANSWER_FRIEND] DEBUG - 7 \n";

		pUDPManager->Push(UDP_PROTOCOL::RESULT_TRUE_FRIEND, pBuffer /*pClient->pUserNode->SetValue().GetSocketInfo()*/);

		std::cout << "[ANSWER_FRIEND] DEBUG - 8 \n";

		pBuffer->AddFriendNickname(pClient->pUserNode->GetNickname());

		std::cout << "[ANSWER_FRIEND] DEBUG - 9 \n";

		pClient->pUserNode->AddFriendNickname(pBuffer->GetNickname());

		std::cout << "[ANSWER_FRIEND] DEBUG - 10 \n";

		pBuffer->SetDeleteFriendID();

		std::cout << "[ANSWER_FRIEND] DEBUG - 11 \n";

		pClient->pUserNode->SetDeleteFriendID();

		std::cout << "[ANSWER_FRIEND] DEBUG - 12 \n";

		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}

	std::cout << "[ANSWER_FRIEND] DEBUG - 13 \n";

	// ���� 4����Ʈ�� �׻� �Ȱ��� �� ����.
	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_ANSWER_MAKE_FRIEND), sizeof(int));

	std::cout << "[ANSWER_FRIEND] DEBUG - 14 \n";

	pClient->dataSize = 8;

	std::cout << pClient->pUserNode->GetNickname() << "�� ���� _AnswerMakeFriendProcess�� ���ϵǾ����ϴ�. \n";

	std::cout << "[ANSWER_FRIEND] DEBUG - 15 \n";
}

/*
	_BuyItemProcess
		- Ŭ���̾�Ʈ���� ���� ������ �ε����� ������ ���� ��ȭ�������� Ȯ���Ͽ� �������� �����մϴ�.
		- UserData���� Shop�����ϴ� ������ ��.
*/
void SCENE_NETWORK_MANAGER::MainUiScene::_BuyItemProcess(SocketInfo* pClient)
{
	// BuyItem�� ��ȯ��
	int iBuffer = pClient->pUserNode->BuyItem(reinterpret_cast<int&>(pClient->buf[4]));

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CONST_ANSWER_BUY_ITEM), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&iBuffer), sizeof(int));

	// ������ ���� ��.
	iBuffer = pClient->pUserNode->GetMoney();
	memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&iBuffer), sizeof(int));

	pClient->dataSize = 12;
}

/*
	_VipCodeProcess
		- Ŭ���̾�Ʈ���� ���� vipCode�� Ȯ���Ͽ�, vipCode ó���� �ϰ�, �ʿ��, �������� �����մϴ�.
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
		- Ŭ���̾�Ʈ���� ���� ���Ÿ� ����ϴ� ĳ���� �ε����� Ȯ���Ͽ�, ó���մϴ�.
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
		- ���� Ŭ���̾�Ʈ��, Ȱ��ȭ ĳ���� �ε����� �����մϴ�.

	#0. �ش� ĳ���� ������, ��ȿ���� ���δ� �˻����� �ʰ�, �ش� ���������� �׻� �ŷ��մϴ�.
	#1. NicknameTree���� �ڽ��� ��带 ã�� Selected Index�� �����մϴ�. (for Friend)
	#2. �翬�� �ڽ��� UserNode�� activeCharacterIndex ���� �����մϴ�.
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
