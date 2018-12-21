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
	
	if (pClient->pUserNode->GetDemandFriendContIndex() != -1)
	{
		friendNum -= 1;
	}

	memcpy(pClient->buf, reinterpret_cast<const char*>(&PERMIT_FRIEND_INFO), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<char*>(&friendNum), sizeof(int));

	pClient->dataSize = 8;

	//std::cout << " " << pClient->pUserNode->GetKey() << "�� ģ�� ���� : " << friendNum << "\n";

	// (����ȭ) ����� �ش� ��û�� ��� �ö�����, 
	// �����ϰ� Nickname�� ���� ID�� ã��, �ش� ID�� ���� UserNode�� ã�� ������ �ݺ��ϰ� �ִµ�,
	// ���� weak_ptr Cont���� nullptr�� �ƴҶ���, ���ҽ� ���� ���θ� �˻���, ������ ���ٸ�, �ش� ����� Get�� Ȱ���ϴ� ����� �ʿ���.
	// �����ϰ� ���� �г����� ��� ��û�� �ʿ䵵 ���� ���¸� ��û�ϸ� ��. �ش翡 ���� ó���� ����ȭ�Ҷ� �� �ʿ��غ���.

	if (friendNum > 0)
	{
		int stringSize{};
		Type_Nickname stringBuffer{};
		bool isOnLogin{};
		bool isOnMatch{};
		shared_ptr<UserData> pBuffer = nullptr;

		for (int i = 0; i < friendNum; ++i)
		{
			// �ش� �����̳��� i��° �г���.
			stringBuffer = pClient->pUserNode->GetFriendNicknameWithIndex(i);

			// �ش� ���̵�� ���� ���� ����, True�ϰ�� 1, false�� ��� 0
			pBuffer = pUserData->SearchUserNodeWithNickname(stringBuffer, isOnLogin, isOnMatch);

			// 0�� ���, ���� �г���. 
			// 1�� ���, �г����� �ִµ� ��α���.
			// 2�� ���, �κ�.
			// 3�� ���, ������.
			pClient->dataSize += sizeof(int);	// 4 Bool Type

			// 0�� ���, (���� ������ �� ������ Ʈ���� �� ����)
			if (!isOnMatch)
			{
				std::cout << "[Error] �̹� ģ���߰��� �� ������ �г����� ���� �г������� ���ɴϴ�. \n";
				memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				continue;
			}

			if (!isOnLogin)	// �α��� ������ ��,
				memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
			else if (pBuffer->GetSocketInfo()->pRoomIter != nullptr)	// ���� ��, �κ�
				memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<const char*>(&CONST_2), sizeof(int));
			else	// ���� ��, ������.
				memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<const char*>(&CONST_3), sizeof(int));

			pClient->pUserNode->SetFreindUserDataWithIndex(pBuffer, i); // weak_ptr�� ��ȯ.

			stringSize = MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), NULL, NULL);

			wstring uniStrBuffer(stringSize, 0);
			MultiByteToWideChar(CP_ACP, 0, &stringBuffer[0], stringBuffer.size(), &uniStrBuffer[0], stringSize);
			
			stringSize = uniStrBuffer.size() * 2;

			memcpy(pClient->buf + pClient->dataSize + 4, reinterpret_cast<char*>(&stringSize), sizeof(int));
			memcpy(pClient->buf + pClient->dataSize + 8, uniStrBuffer.data(), stringSize);

			pClient->dataSize += (4 + stringSize); 
		}
	}
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandFriendInviteProcess(SocketInfo* pClient)
{
	int indexBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	shared_ptr<UserData> pBuffer = pClient->pUserNode->GetFriendUserDataWithIndex(indexBuffer).lock();

	// ������ shared_ptr�� ���� nullptr�� �ƴϰ�, ������ ��� �濡�� ������ ���� ���� ���.  
	if (pBuffer != nullptr && pBuffer->GetSocketInfo()->pRoomIter == nullptr)
	{
		// �� ���� �ʿ�
		pClient->pRoomIter = make_shared<GameRoom>(pClient->pUserNode, true);

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
			int retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex;
			//Type_Nickname stringBuffer;

			pClient->pRoomIter->GetRoomGameData(pClient->isHost, retIsHostFirst, retPlayerMissionIndex, retEnemyMissionIndex, retSubMissionIndex);

			// �� ���̳��� ������ ����
			memcpy(pClient->buf + 8, reinterpret_cast<char*>(&retIsHostFirst), sizeof(int));
			memcpy(pClient->buf + 12, reinterpret_cast<char*>(&retPlayerMissionIndex), sizeof(int));
			memcpy(pClient->buf + 16, reinterpret_cast<char*>(&retEnemyMissionIndex), sizeof(int));
			memcpy(pClient->buf + 20, reinterpret_cast<char*>(&retSubMissionIndex), sizeof(int));

			//int sizeBuffer = stringBuffer.size(); // DEV_66;
			//memcpy(pClient->buf + 24, reinterpret_cast<char*>(&sizeBuffer), sizeof(int));
			//memcpy(pClient->buf + 28, stringBuffer.data(), sizeBuffer);
			//pClient->dataSize = 32 + sizeBuffer;

			pClient->dataSize = 24;
		}
	}
	else
	{
		//�̹� ȣ��Ʈ�� 7�ʰ� ���� ���� �����ų�, �̻�����.
		pClient->pRoomIter.reset();
		//pClient->pRoomIter = nullptr;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 8;
	}
}

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

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendProcess(SocketInfo* pClient)
{
	int iBuffer = reinterpret_cast<int&>(pClient->buf[4]);
	pClient->buf[8 + iBuffer] = '\0';
	
	Type_Nickname NicknameBuffer((pClient->buf + 8));

	bool isOnLogin{ false };
	bool isOnMatch{ false };
	shared_ptr<UserData> pBuffer = pUserData->SearchUserNodeWithNickname(NicknameBuffer, isOnLogin, isOnMatch);

	// �ش� �г����̰� �ƿ� ���� ��� : ANSWER_MAKE_FRIEND + 0 + 3
	if (!isOnMatch)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_3), sizeof(int));

		pClient->dataSize = 12;
		return;
	}

	// ������ �α��� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 0 .
	if (!isOnLogin)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 12;
		return;
	}

	iBuffer = pBuffer->GetFriendNicknameContSize();

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
	if (pBuffer->GetDemandFriendContIndex() != -1)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "������ �̹� ģ�� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	// ���� ���� ������ ģ�� ���� ���� ��Ȳ �� ���. : ANSWER_MAKE_FRIEND + 0 + 2 ���� ������ ��츦 ��.
	if (pClient->pUserNode->GetDemandFriendContIndex() != -1)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "������ �̹� ģ�� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	// ���濡�� ģ�� ��û�� ������, ���� ģ���� �ӽ÷� ��� ������. : ANSWER_MAKE_FRIEND + 1
	pBuffer->SetDemandFriendContIndex(pBuffer->SetInsertFriendNickname(pClient->pUserNode->GetNickname()));
	pClient->pUserNode->SetDemandFriendContIndex(pClient->pUserNode->SetInsertFriendNickname(pBuffer->GetNickname()));
	
	pUDPManager->Push(UDP_PROTOCOL::DEMAND_FRIEND, pBuffer);
	//pClient->pUserNode->SetValue().SetInsertFriendID(pBuffer->GetKey());

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

	//std::cout << "���濡�� ģ�� ��û�� ���� : ANSWER_MAKE_FRIEND + 1 \n";
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
		pUserData->SearchUserNodeWithNickname(pClient->pUserNode->GetFriendNicknameWithIndex(pClient->pUserNode->GetDemandFriendContIndex()), bBuffer, bIsOnMatch);

	// bIsOnMatch <- �̰� �׳� ��¿�� ���� �־��ֱ��ϴµ� ���⼭��, ���ǹ��ϴ�.

	// ģ�� �����ߴ� �Ӹ�. // ���������� �˸� ���� �ʳ�...?
	if (iBuffer == 0)
	{
		if(bBuffer)
		// ģ�� ���� �˷���.

		// �߶���ô�. // ���ο��� ģ�� ����������.
		pClient->pUserNode->SetDeleteFriendID();
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// ģ�� ��û�� ������ ������ ģ�� ��� ���μ����� �����ϱ� �����.
	else if (!bBuffer)
	{
		// ģ�� ���� ���� �˷��ټ��� ���� (�̹� ������ ��? )
		//InUDPManager.Push(UDP_PROTOCOL::DENY_FRIEND, pBuffer->SetValue().GetSocketInfo());

		// �߶���ô�. // ���ο��� ģ�� ����������.
		pClient->pUserNode->SetDeleteFriendID();
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// ģ�� ��û�� ������ ������. ģ�� ��� ���� ���μ��� ����. (ģ���ϳ� ����⵵ �����;)
	else
	{
		std::cout << " " << pBuffer->GetKey() << "���� ���� ģ���ʴ븦 " << pClient->pUserNode->GetKey() << "���� �޾Ҿ��! \n";
		pUDPManager->Push(UDP_PROTOCOL::RESULT_FRIEND, pBuffer /*pClient->pUserNode->SetValue().GetSocketInfo()*/);

		pBuffer->SetDemandFriendContIndex(-1);
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}

	// ���� 4����Ʈ�� �׻� �Ȱ��� �� ����.
	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_ANSWER_MAKE_FRIEND), sizeof(int));

	pClient->dataSize = 8;
}