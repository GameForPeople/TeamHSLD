#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserDataManager.h"
#include "../UserData/User_RBTree.h"

class GameRoom;

struct SocketInfo
{
	OVERLAPPED overlapped;	// OVERLAPPED ����ü
	WSABUF wsabuf;
	SOCKET sock;
	char buf[BUF_SIZE + 1];

	int dataSize;
	// �̰� Ʈ��� ���� Ÿ�̹�, flase�� �ִ� Ÿ�̹� (�ߺ�! ���� �踤�Դϴ�.)
	bool isRecvTurn;

	// �̰Ŵ� �� ���Ͽ� ������ �ؾ������� �����س��� �� (enum Protocol)
	//int bufferProtocol; // �Ƚ��Ӹ�

	// ȸ������ + �α��� ��, �ε� ���� �����Ϳ��� �ش� �ε��� �����Ͽ� ���!	
	// ���Ŀ� �ش� �ε����� ���ͷ����� �����ͷ� ���� ������ �����Դϴ�.
	// ���� �����̳� Std::Vector

	// STL��� �Ƹ��ٿ� �츮 ���ͷ����͸� ����մϴ�. 24����Ʈ;;���پ�;;
	rbTreeNode<string,UserData>* pUserNode;
	// �ѹ��� ����ؼ� ������ ����սô�.
	int userDataContIndex;

	// �뾻 �ε���, �� ���� �ó� ���� �� ���� ( �ʱ�ȭ ���ʿ� )
	GameRoom* pRoomIter;
	bool isInRoom;

	// �뾻, �ΰ��Ӿ����� ȣ��Ʈ ���� üũ (�ʱ�ȭ ���ʿ�)
	bool isHost;

	// ĳ������ ������ ��Ƴִ� ����ü�Դϴ�.
	//UserData* pUserData;

public:
	//������ �ʿ信 ���� �߰�.
	__inline SocketInfo() :
		//bufferProtocol(0), 
		buf(),
		pRoomIter(nullptr),
		isInRoom(false),
		isHost(false),
		//dataSize(), 
		isRecvTurn(true),
		pUserNode(nullptr),
		userDataContIndex()//,
		//enemyIter()
	{};

	__inline ~SocketInfo() = default;
};