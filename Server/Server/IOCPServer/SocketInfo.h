#pragma once

#include "../PCH/stdafx.h"

#include "../GameRoom/GameRoomManager.h"
#include "../UserData/NewUserDataManager.h"

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

	// �뾻 �ε���, �� ���� �ó� ���� �� ���� ( �ʱ�ȭ ���ʿ� )
	int roomIndex;
	bool isInRoom;

	// �뾻, �ΰ��Ӿ����� ȣ��Ʈ ���� üũ (�ʱ�ȭ ���ʿ�)
	bool isHost;

	// ĳ������ ������ ��Ƴִ� ����ü�Դϴ�.
	//NewUserData* pUserData;

	// STL��� �Ƹ��ٿ� �츮 ���ͷ����͸� ����մϴ�. 24����Ʈ;;���پ�;;
	user_iter userIter;
	// �ѹ��� ����ؼ� ������ ����սô�.
	int userDataContIndex;

public:
	//������ �ʿ信 ���� �߰�.
	__inline SocketInfo() :
		//bufferProtocol(0), 
		buf(),
		roomIndex(-1),
		isInRoom(false),
		isHost(false),
		//dataSize(), 
		isRecvTurn(true),
		userIter(),
		userDataContIndex()//,
		//enemyIter()
	{};

	__inline ~SocketInfo() = default;
};