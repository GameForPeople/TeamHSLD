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

	std::cout << " " << pClient->pUserNode->GetKey() << "�� ģ�� ���� : " << friendNum << "\n";

	if (friendNum)
	{
		int stringSize{};
		string stringBuffer{};
		bool isOnLogin{};
		rbTreeNode<string, UserData> *pBuffer = nullptr;

		for (int i = 0; i < friendNum; ++i)
		{
			// �ش� �����̳��� i��° ���̵� ��Ʈ���� ������.
			stringBuffer = pClient->pUserNode->SetValue().GetFriendStringCont()[i];
				//stringSize = stringBuffer.size();
			std::cout << " " << pClient->pUserNode->GetKey() << "�� ģ�� : " << stringBuffer << "\n";

			// ���̵� ��Ʈ�� �������, ���̵� ����.
				//memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<char*>(&stringSize), sizeof(int));
				//memcpy(pClient->buf + pClient->dataSize + 4, stringBuffer.data(), stringSize);
				//pClient->dataSize += 4 + stringSize;

			// �ش� ���̵�� ���� ���� ����, True�ϰ�� 1, false�� ��� 0
			pBuffer = pUserData->SearchUserNode(stringBuffer, isOnLogin);

			// �α��� ���� ��, �ش� ���� �г��� �������, �г���, ���� ���¸� ����.
			if (isOnLogin)
			{
				// ���� Ŭ���̾�Ʈ��, UserNode��, ģ�� ���� ���� ����
					//pClient->pUserNode->SetValue().SetFreindSocketInfo(pBuffer->SetValue().GetSocketInfo(), i); 				// �̰� �ɱ� ������ ���� ����. �׽�Ʈ �ʿ�.

				memcpy(pClient->buf + pClient->dataSize, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));


				stringBuffer = pBuffer->SetValue().GetNickName();
				stringSize = stringBuffer.size();

				memcpy(pClient->buf + pClient->dataSize + 4, reinterpret_cast<char*>(&stringSize), sizeof(int));
				memcpy(pClient->buf + pClient->dataSize + 8, stringBuffer.data(), stringSize);
				
				pClient->dataSize += (12 + stringSize); // 4 Bool isOnLogin, 4 nickNameSize, 4 (�ٷ� ����) �� �� �ִ��� ������ ����.

				if (pBuffer->GetValue().GetSocketInfo()->pRoomIter != nullptr)
				{
					memcpy(pClient->buf + pClient->dataSize - 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
				}
				else
				{
					memcpy(pClient->buf + pClient->dataSize - 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				}
			}
			// �α��� ���� �ʾ��� ��, �ش� ������ ����.
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

	// ������ ������ �� ���� ����, ���� Search�ؼ� ���ô�? ���� ������, ������?
	//pClient->pUserNode->SetValue().GetFriendSocketInfo(indexBuffer);
	bool isOnUser{ false };

	rbTreeNode<string, UserData> *pBuffer = pUserData->SearchUserNode(pClient->pUserNode->SetValue().GetFriendStringWithIndex(indexBuffer), isOnUser);

	if (isOnUser && pBuffer->SetValue().GetSocketInfo()->pRoomIter == nullptr)
	{
		// �� ���� �ʿ�
		pClient->pRoomIter = new GameRoom(pClient->pUserNode, nullptr, nullptr);

		// ���¾� ������̽ô�.
		pClient->isHost = true;

		// ģ�� ��� ģ���� �����ּұ���ü�� ���.
		pBuffer->SetValue().GetSocketInfo()->pRoomIter = pClient->pRoomIter;
		pClient->pRoomIter->SetFriendUserPtr(pBuffer);

		// UDP Packet ���.
		pUDPManager->Push(UDP_PROTOCOL::INVITE_FRIEND, pBuffer->SetValue().GetSocketInfo());

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

	pClient->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_AnswerFriendInviteProcess(SocketInfo* pClient)
{
	int isTrueBuffer = reinterpret_cast<int&>(pClient->buf[4]);

	if (pClient->pRoomIter != nullptr)
	{
		if (isTrueBuffer == 0)
		{
			// ����ȭ ����ĥ �� ����.
			pClient->pRoomIter->SetUserPtr(true, nullptr);

			// �� ���� ȣ��Ʈ�� �ϵ��� �����.
			//delete pClient->pRoomIter;
			//pClient->pRoomIter = nullptr;

			memcpy(pClient->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		}
		else
		{
			pClient->isHost = false;
			pClient->pRoomIter->JoinRoom(pClient->pUserNode);

			// �� ����.
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
			
			std::cout << "������ �г����� : " << stringBuffer << "�Դϴ�. ������� "<< sizeBuffer << " \n";

			memcpy(pClient->buf + 24, reinterpret_cast<char*>(&sizeBuffer), sizeof(int));
			memcpy(pClient->buf + 28, stringBuffer.data(), sizeBuffer);

			pClient->dataSize = 32 + sizeBuffer;
		}
	}
	else
	{
		//�̹� ȣ��Ʈ�� �̻��ϰų� �ð��� ���� ��.
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
		// �� ��������.-> ģ��(��) ���� ���� ���̽�. ���⼭ �̴�� ��!
		delete pClient->pRoomIter;
		pClient->pRoomIter = nullptr;

		memcpy(pClient->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 8;
	}
	else if (pClient->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_PLAY)
	{
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
	// ���������� ����� (UDP �ս� ���ɼ��� ���� ����� �� ����) - Ȥ�� ���� Ÿ�Ӿƿ�
	else
	{
		// �� �̾� ���� �ʹ� �߸� �������. 
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

	std::cout << "[Debug] DemandMakeFriend "<< pClient->pUserNode->GetKey() <<"�� ģ����û�� ID�� : " << idBuffer << "�Դϴ�. \n";

	bool isOnLogin{ false };
	rbTreeNode<string, UserData>* pBuffer = pUserData->SearchUserNode(idBuffer, isOnLogin);

	// ������ �α��� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 0
	if (!isOnLogin)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		pClient->dataSize = 12;

		//std::cout << "������ �α��� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 0 \n";
		return;
	}

	iBuffer = pBuffer->SetValue().GetFriendStringContSize();

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
	if (pBuffer->SetValue().GetDemandFriendContIndex() != -1)
	{
		memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(pClient->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		pClient->dataSize = 12;
		//std::cout << "������ �̹� ģ�� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	// ���濡�� ģ�� ��û�� ���� : ANSWER_MAKE_FRIEND + 1
	pBuffer->SetValue().SetDemandFriendContIndex(pBuffer->SetValue().SetInsertFriendID(pClient->pUserNode->GetKey()));
	pUDPManager->Push(UDP_PROTOCOL::DEMAND_FRIEND, pBuffer->SetValue().GetSocketInfo());
	//pClient->pUserNode->SetValue().SetInsertFriendID(pBuffer->GetKey());

	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
	memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

	//std::cout << "���濡�� ģ�� ��û�� ���� : ANSWER_MAKE_FRIEND + 1 \n";
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

	// ģ�� �����ߴ� �Ӹ�. // ���������� �˸� ���� �ʳ�...?
	if (iBuffer == 0)
	{
		if(bBuffer)
		// ģ�� ���� �˷���.

		// �߶���ô�. // ���ο��� ģ�� ����������.
		pClient->pUserNode->SetValue().SetDeleteFriendID();
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// ģ�� ��û�� ������ ������ ģ�� ��� ���μ����� �����ϱ� �����.
	else if (!bBuffer)
	{
		// ģ�� ���� ���� �˷��ټ��� ���� (�̹� ������ ��? )
		//InUDPManager.Push(UDP_PROTOCOL::DENY_FRIEND, pBuffer->SetValue().GetSocketInfo());

		// �߶���ô�. // ���ο��� ģ�� ����������.
		pClient->pUserNode->SetValue().SetDeleteFriendID();
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// ģ�� ��û�� ������ ������. ģ�� ��� ���� ���μ��� ����. (ģ���ϳ� ����⵵ �����;)
	else
	{
		std::cout << " " << pBuffer->GetKey() << "���� ���� ģ���ʴ븦 " << pClient->pUserNode->GetKey() << "���� �޾Ҿ��! \n";
		pUDPManager->Push(UDP_PROTOCOL::RESULT_FRIEND, pBuffer->SetValue().GetSocketInfo() /*pClient->pUserNode->SetValue().GetSocketInfo()*/);

		pBuffer->SetValue().SetInsertFriendID(pClient->pUserNode->GetKey());
		memcpy(pClient->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}

	//�׻� �Ȱ��� �� ����.
	memcpy(pClient->buf, reinterpret_cast<const char*>(&CHECK_ANSWER_MAKE_FRIEND), sizeof(int));

	pClient->dataSize = 8;
}