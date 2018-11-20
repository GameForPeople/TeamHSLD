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

		// UDP Packet ���.
		InUDPManager.Push(1, pBuffer->SetValue().GetSocketInfo());

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
	if (ptr->pRoomIter == nullptr)
	{
		// �̴�� �� OK!
		memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}
	else if (ptr->pRoomIter->RetEnemyUserIter(ptr->isHost) == nullptr)
	{
		// �� ��������. ���⼭ �̴�� ��! -> guest�� UDP �ս� ���ɼ��Ǵ� ����ȭ ���� ���ɼ�-> �̰Ÿ� �˻��� �� ������ ���� ���� ��?
		delete ptr->pRoomIter;
		ptr->pRoomIter = nullptr;

		memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_TRUE), sizeof(int));
	}
	else
	{
		// ���� �� �ƽ�Ÿ�ӿ� ���°Ű���? �ش� �κ� �����Ѵٳ� // ����° False������ ���� ��.
		memcpy(ptr->buf, reinterpret_cast<const char*>(&HOSTCHECK_FRIEND_INVITE), sizeof(int));
		memcpy(ptr->buf + 4, reinterpret_cast<const char*>(&CONST_FALSE), sizeof(int));
	}

	ptr->dataSize = 8;
}