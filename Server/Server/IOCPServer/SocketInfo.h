#pragma once

#include "../PCH/stdafx.h"

#include "../GameRoom/GameRoomManager.h"
#include "../UserData/NewUserDataManager.h"

struct SocketInfo
{
	OVERLAPPED overlapped;	// OVERLAPPED 구조체
	WSABUF wsabuf;
	SOCKET sock;
	char buf[BUF_SIZE + 1];

	int dataSize;
	// 이게 트루면 받을 타이밍, flase면 주는 타이밍 (삐빅! 지금 계륵입니다.)
	bool isRecvTurn;

	// 이거는 이 번턴에 무엇을 해야할지를 저장해놓는 것 (enum Protocol)
	//int bufferProtocol; // 안써임마

	// 회원가입 + 로그인 시, 로드 유저 데이터에서 해당 인덱스 저장하여 사용!	
	// 추후에 해당 인덱스는 이터레이터 포인터로 변경 적용할 예정입니다.
	// 현재 컨테이너 Std::Vector

	// 룸씐 인덱스, 방 제작 시나 접속 시 사용됨 ( 초기화 미필요 )
	int roomIndex;

	// 룸씐, 인게임씬에서 호스트 여부 체크 (초기화 미필요)
	bool isHost;

	// 캐릭터의 정보를 담아넣는 구조체입니다.
	//NewUserData* pUserData;

	// STL답게 아름다운 우리 이터레이터를 사용합니다.
	map<string, NewUserData>::iterator userIter;
	//std::conditional_t<

public:
	//생성자 필요에 의해 추가.
	__inline SocketInfo() :
		//bufferProtocol(0), 
		buf(),
		roomIndex(-1),
		isHost(false),
		//dataSize(), 
		isRecvTurn(true),
		userIter()
	{};

	__inline ~SocketInfo() = default;
};