#pragma once

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "wininet.lib")

#include <WinSock2.h>
#include <iostream>
#include <cstdlib>

#include <fstream>

#include <vector>
#include <string>

//thread
#include <thread>
#include <atomic>
#include <mutex>

// For ExternalIP
#include "wininet.h"
#include <tchar.h>

#include <string_view>

using namespace std;

// For Login
#define		MAX_ID_LEN		12
#define		BUF_SIZE		100

struct SOCKETINFO
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
	int userIndex;

	// 룸씐 인덱스, 방 제작 시나 접속 시 사용됨 ( 초기화 미필요 )
	int roomIndex;

	// 룸씐, 인게임씬에서 호스트 여부 체크 (초기화 미필요)
	bool isHost;

	// 대망의 이 친구..는 사실 바이트단위로 전송하면서..쓰레기가 되어버렸는데..
	//는 다시 부활!!!! 리팩토링 과정에서 이거 사용하기로 변경!!! 주석 해제!! 축하해 ㅠㅠ
	// 는 memcpy 과정에서의 오류때문에... 안쓰는걸로 결정..
	//BaseStruct* dataBuffer;

public:
	//생성자 필요에 의해 추가.
	__inline SOCKETINFO() :
		//bufferProtocol(0), 
		buf(),
		userIndex(-1), 
		roomIndex(-1), 
		isHost(false),
		//dataSize(), 
		isRecvTurn(true)
	{};

	__inline ~SOCKETINFO() = default;
};
