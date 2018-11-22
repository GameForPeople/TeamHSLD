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

	std::cout << " " << ptr->pUserNode->GetKey() << "�� ģ�� ���� : " << friendNum << "\n";

	if (friendNum)
	{
		int stringSize{};
		string stringBuffer{};
		bool isOnLogin{};
		rbTreeNode<string, UserData> *pBuffer = nullptr;

		for (int i = 0; i < friendNum; ++i)
		{
			// �ش� �����̳��� i��° ���̵� ��Ʈ���� ������.
			stringBuffer = ptr->pUserNode->SetValue().GetFriendStringCont()[i];
				//stringSize = stringBuffer.size();
			std::cout << " " << ptr->pUserNode->GetKey() << "�� ģ�� : " << stringBuffer << "\n";

			// ���̵� ��Ʈ�� �������, ���̵� ����.
				//memcpy(ptr->buf + ptr->dataSize, reinterpret_cast<char*>(&stringSize), sizeof(int));
				//memcpy(ptr->buf + ptr->dataSize + 4, stringBuffer.data(), stringSize);
				//ptr->dataSize += 4 + stringSize;

			// �ش� ���̵�� ���� ���� ����, True�ϰ�� 1, false�� ��� 0
			pBuffer = InUserData.SearchUserNode(stringBuffer, isOnLogin);

			// �α��� ���� ��, �ش� ���� �г��� �������, �г���, ���� ���¸� ����.
			if (isOnLogin)
			{
				// ���� Ŭ���̾�Ʈ��, UserNode��, ģ�� ���� ���� ����
					//ptr->pUserNode->SetValue().SetFreindSocketInfo(pBuffer->SetValue().GetSocketInfo(), i); 				// �̰� �ɱ� ������ ���� ����. �׽�Ʈ �ʿ�.

				memcpy(ptr->buf + ptr->dataSize, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));


				stringBuffer = pBuffer->SetValue().GetNickName();
				stringSize = stringBuffer.size();

				memcpy(ptr->buf + ptr->dataSize + 4, reinterpret_cast<char*>(&stringSize), sizeof(int));
				memcpy(ptr->buf + ptr->dataSize + 8, stringBuffer.data(), stringSize);
				
				ptr->dataSize += (12 + stringSize); // 4 Bool isOnLogin, 4 nickNameSize, 4 (�ٷ� ����) �� �� �ִ��� ������ ����.

				if (pBuffer->GetValue().GetSocketInfo()->pRoomIter != nullptr)
				{
					memcpy(ptr->buf + ptr->dataSize - 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
				}
				else
				{
					memcpy(ptr->buf + ptr->dataSize - 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
				}
			}
			// �α��� ���� �ʾ��� ��, �ش� ������ ����.
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

	// ������ ������ �� ���� ����, ���� Search�ؼ� ���ô�? ���� ������, ������?
	//ptr->pUserNode->SetValue().GetFriendSocketInfo(indexBuffer);
	bool isOnUser{ false };

	rbTreeNode<string, UserData> *pBuffer = InUserData.SearchUserNode(ptr->pUserNode->SetValue().GetFriendStringWithIndex(indexBuffer), isOnUser);

	if (isOnUser && pBuffer->SetValue().GetSocketInfo()->pRoomIter == nullptr)
	{
		// �� ���� �ʿ�
		ptr->pRoomIter = new GameRoom(ptr->pUserNode, nullptr, nullptr);

		// ���¾� ������̽ô�.
		ptr->isHost = true;

		// ģ�� �浵 ���.
		pBuffer->SetValue().GetSocketInfo()->pRoomIter = ptr->pRoomIter;
		ptr->pRoomIter->SetFriendUserPtr(pBuffer);

		// UDP Packet ���.
		InUDPManager.Push(UDP_PROTOCOL::INVITE_FRIEND, pBuffer->SetValue().GetSocketInfo());

		// TCP True Send �ʿ�.
		memcpy(ptr->buf, reinterpret_cast<const char*>(&NOTIFY_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}
	else
	{
		// TCP False Send �ʿ�.
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
			// ����ȭ ����ĥ �� ����.
			//ptr->pRoomIter->SetDataProtocol(false, );
			ptr->pRoomIter->RetEnemyUserIter(ptr->isHost)->SetValue().GetSocketInfo()->pRoomIter = nullptr;

			delete ptr->pRoomIter;
			ptr->pRoomIter = nullptr;

			// ������ ���� �� ����.
			memcpy(ptr->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		}
		else
		{
			ptr->pRoomIter->JoinRoom(ptr->pUserNode);

			// �� ����.
			memcpy(ptr->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
		}
	}
	else
	{
		//�̹� 10�ʰ� ���� ��.
		memcpy(ptr->buf, reinterpret_cast<const char*>(&GUESTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DelayFriendInviteProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData)
{
	int iBuffer = reinterpret_cast<int&>(ptr->buf[4]);

	if (iBuffer)
	{
		if (ptr->pRoomIter->RetEnemyUserIter(ptr->isHost) == nullptr)
		{
			// �� ��������. ���⼭ �̴�� ��! -> guest�� UDP �ս� ���ɼ��Ǵ� ����ȭ ���� ���ɼ�-> �̰Ÿ� �˻��� �� ������ ���� ���� ��?
			delete ptr->pRoomIter;
			ptr->pRoomIter = nullptr;

			memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_2), sizeof(int));
		}
		else if (ptr->pRoomIter->roomState == ROOM_STATE::ROOM_STATE_PLAY)
		{
			memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
		}
		else
		{
			memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		}
	}
	else {
		if (ptr->pRoomIter == nullptr)
		{
			// �̴�� �� OK!
			memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_2), sizeof(int));
		}
		else if (ptr->pRoomIter->RetEnemyUserIter(ptr->isHost) == nullptr)
		{
			// �� ��������. ���⼭ �̴�� ��! -> guest�� UDP �ս� ���ɼ��Ǵ� ����ȭ ���� ���ɼ�-> �̰Ÿ� �˻��� �� ������ ���� ���� ��?
			delete ptr->pRoomIter;
			ptr->pRoomIter = nullptr;

			memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_2), sizeof(int));
		}
		else
		{
			// ���� �� �ƽ�Ÿ�ӿ� ���°Ű���? �ش� �κ� �����Ѵٳ� // ����° False������ ���� ��.
			memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
			memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
		}
	}

	ptr->dataSize = 8;
}

void SCENE_NETWORK_MANAGER::MainUiScene::_DemandMakeFriendProcess(SocketInfo* ptr, GameRoomManager& InRoomData, UserDataManager& InUserData, UDPManager& InUDPManager)
{
	int iBuffer = reinterpret_cast<int&>(ptr->buf[4]);
	ptr->buf[8 + iBuffer] = '\0';
	
	string idBuffer(ptr->buf + 8);

	std::cout << "[Debug] DemandMakeFriend "<< ptr->pUserNode->GetKey() <<"�� ģ����û�� ID�� : " << idBuffer << "�Դϴ�. \n";

	bool isOnLogin{ false };
	rbTreeNode<string, UserData>* pBuffer = InUserData.SearchUserNode(idBuffer, isOnLogin);

	// ������ �α��� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 0
	if (!isOnLogin)
	{
		memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(ptr->buf + 8, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));

		ptr->dataSize = 12;

		//std::cout << "������ �α��� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 0 \n";
		return;
	}

	iBuffer = pBuffer->SetValue().GetFriendStringContSize();

	// ������ �ƽ� �������� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 1 
	if (iBuffer >= 4)
	{
		memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(ptr->buf + 8, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

		ptr->dataSize = 12;
		//std::cout << "������ �ƽ� �������� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 1  \n";

		return;
	}

	// ������ �̹� ģ�� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 2
	if (pBuffer->SetValue().GetDemandFriendContIndex() != -1)
	{
		memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
		memcpy(ptr->buf + 8, reinterpret_cast<const char*>(&CONST_2), sizeof(int));

		ptr->dataSize = 12;
		//std::cout << "������ �̹� ģ�� ���� ���� ��Ȳ : ANSWER_MAKE_FRIEND + 0 + 2 \n";
		return;
	}

	// ���濡�� ģ�� ��û�� ���� : ANSWER_MAKE_FRIEND + 1
	pBuffer->SetValue().SetDemandFriendContIndex(pBuffer->SetValue().SetInsertFriendID(ptr->pUserNode->GetKey()));
	InUDPManager.Push(UDP_PROTOCOL::DEMAND_FRIEND, pBuffer->SetValue().GetSocketInfo());
	//ptr->pUserNode->SetValue().SetInsertFriendID(pBuffer->GetKey());

	memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_DEMAND_MAKE_FRIEND), sizeof(int));
	memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));

	//std::cout << "���濡�� ģ�� ��û�� ���� : ANSWER_MAKE_FRIEND + 1 \n";
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

	// ģ�� �����ߴ� �Ӹ�. // ���������� �˸� ���� �ʳ�...?
	if (iBuffer == 0)
	{
		if(bBuffer)
		// ģ�� ���� ���� �˷���.
			InUDPManager.Push(UDP_PROTOCOL::DENY_FRIEND, pBuffer->SetValue().GetSocketInfo());

		// �߶���ô�. // ���ο��� ģ�� ����������.
		ptr->pUserNode->SetValue().SetDeleteFriendID();
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// ģ�� ��û�� ������ ������ ģ�� ��� ���μ����� �����ϱ� �����.
	else if (!bBuffer)
	{
		// ģ�� ���� ���� �˷��ټ��� ���� (�̹� ������ ��? )
		//InUDPManager.Push(UDP_PROTOCOL::DENY_FRIEND, pBuffer->SetValue().GetSocketInfo());

		// �߶���ô�. // ���ο��� ģ�� ����������.
		ptr->pUserNode->SetValue().SetDeleteFriendID();
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	// ģ�� ��û�� ������ ������. ģ�� ��� ���� ���μ��� ����. (ģ���ϳ� ����⵵ �����;)
	else
	{
		pBuffer->SetValue().SetInsertFriendID(ptr->pUserNode->GetKey());
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}

	//�׻� �Ȱ��� �� ����.
	memcpy(ptr->buf, reinterpret_cast<const char*>(&CHECK_ANSWER_MAKE_FRIEND), sizeof(int));

	ptr->dataSize = 8;
}