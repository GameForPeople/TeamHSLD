#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserDataManager.h"

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
	shared_ptr<UserData>	pUserNode;
	//CUSTOM_SET::rbTreeNode<UserData, string>* pUserNode;

	// �ѹ��� ����ؼ� ������ ����սô�.
	// int userDataContIndex; -> ������ �Ⱦ��̴µ� ����..?

	// �뾻 �ε���, �� ���� �ó� ���� �� ���� ( �ʱ�ȭ ���ʿ� )
	shared_ptr<GameRoom>	pRoomIter;
	//bool isInRoom;

	// �뾻, �ΰ��Ӿ����� ȣ��Ʈ ���� üũ (�ʱ�ȭ ���ʿ�)
	bool isHost;

	// ĳ������ ������ ��Ƴִ� ����ü�Դϴ�.
	//UserData* pUserData;

public:
	//������ �ʿ信 ���� �߰�.
	SocketInfo() noexcept
		//bufferProtocol(0), 
		:	overlapped()
		,	wsabuf()
		,	sock()
		,	buf()
		,	dataSize()
		//,	isInRoom(false)
		,	isHost(false)
		//,	dataSize() 
		,	isRecvTurn(true)
		,	pUserNode(nullptr)
		,	pRoomIter(nullptr)
		//,	userDataContIndex()
		//enemyIter()
	{};

	~SocketInfo() = default;
};