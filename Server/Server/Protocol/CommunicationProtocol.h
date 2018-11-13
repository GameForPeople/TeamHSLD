#pragma once

/*
CommunicationProtocol Header

 1. 멤버변수는, std::string, std::vector<int> 대신 char[], int[]를 사용함. (이는 NETWORK_UTIL::SendProcess과의 호환성 때문)
	- 희망사항 : 추후 SendProcess에서 BaseStruct의 가상함수를 호출하도록 함. 해당 함수 인자는 char*(Byte*) // &(ptr->buffer[0]) 를 인자로 받으며, 
				각 상속된 객체 내에서 오버라이드 된 함수에서 각각 처리. 이 때는, String과 Vector를 사용해도 좋음 ( 동일하게 String_view 사용가능)

 2. 현재 BaseStruct가 50% NonCopyable Struct임. 
	- 대입 할당 연산자는 막았으나, 복사생성자 막지 못함. (이동생성자를 쓰던가, 아에 저 상황을 뿌셔야함)

 3. 현재 객체의 String -> char*로의 변환을, for문에서, strncpy(or memcpy) 로 변경함.
	- 솔직히 적합하지 않음. 이미 선언된 문자열을 활용하지 못한다는 점은 큰 손해. 이를 어찌 할 수 있을지 고민해야함.

 4. 마찬가지로 현재 인자가 Called by Const Value 인데, 이거 손해. Called by Const Ref로 변환이 가능한지를 확인할 필요가 있음 

 5. 3번안이 안될 때, 인자를 줄이기 위해... char 배열의 포인터를 넘기는 건 어떨까? -> 고나마 복사 한번하니 성능적인 이득이 있을 듯 보임.

 */


// 주의해야합니다! 항상 클래스는 4바이트 단위로 제작합니다. SIMD 아니고 이거 뭐더라...
#include "../PCH/stdafx.h"

enum Protocol {

	// for Server
	END_SEND			=	-1	,
	START_RECV			=	0	,

	// for LoginScene
	DEMAND_LOGIN		=	100	,
	FAIL_LOGIN			=	101	,
	PERMIT_LOGIN		=	102	,

	CHANGE_NICKNAME		=	103 ,	// Client To Server
	PERMIT_NICKNAME		=	104 ,	// Server to Client - buffer
	
	// for MainUiScene

	// for LobbyScene 구 로비 씬
	DEMAND_MAKEROOM		=	301	,
	PERMIT_MAKEROOM		=	302	, // 여기서, 방의 생성자를 호출하는데, 누가 먼저 턴인지, 어느 승리조건인지 결정 ( 방객체 변수로 저장)
	DEMAND_JOINROOM		=	303	,
	PERMIT_JOINROOM		=	304	, // 여기서, 추가된 정보(먼저 턴, 승리조건 등 추가 전송)
	FAIL_JOINROOM		=	305	,
	
	// 신 Lobby Protocol
	DEMAND_RANDOM_MATCH =	311	,   // 해당 프로토콜을 받을 경우, 먼저 방 접속 가능 여부를 확인하고, 불가능 시 방을 생성합니다.
	PERMIT_MAKE_RANDOM	=	312	,   // 방을 만들었다고 알립니다.
	PERMIT_JOIN_RANDOM	=	313	,   // 방에 접속했다고 알립니다.

	DEMAND_GUEST_JOIN	=	314	,   // 방에 게스트가 접속했는지의 여부를 확인합니다.
	PERMIT_GUEST_JOIN	=	315 ,	// 방에 게스트가 접속했음.
	PERMIT_GUEST_NOT_JOIN = 316 ,	// 방에 게스트가 접속했음.

	DEMAND_EXIT_RANDOM	=	317	,	// 랜덤큐를 취소함을 알림.
	ANSWER_EXIT_RANDOM	=	318	,	// 랜덤큐 취소 여부에 대해 알림 (성공시 보내고, 실패시는, Guest_Join 처리함)

	// for RoomScene 구 룸씬
	DEMAND_ROOMHOST		=	400	,
	ROOMSTATE_VOID		=	410	,
	ROOMSTATE_GUESTIN	=	411	,
	
	// 신 Room Protocol
	DEMAND_ENEMY_CHARACTER = 421,   // 내 캐릭터 변경 정보를 전송하고,
	PERMIT_ENEMY_CHARACTER = 422,   // 상대방의 캐릭터 정보를 받아옵니다.

	DISCONNECTED_ENEMY_CLIENT = 506, // 상대편 클라이언트 네트워크 예외에 대한 처리 요청 (roomProtocol 및 클라에서 적용) - (Server to client)
	DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT = 506, // 상대편 클라이언트가 나갔음을 인지하고, 해당 처리를 요청함. (Client to Server)

	// for GameScene // 단방향으로 수정 적용 완료(2018/10/19ver)
	VOID_GAME_STATE =500 ,	// 디펜스 턴인 친구가, 야 공격턴이 공격햇어??를 여쭤봄

	NOTIFY_CHANGE_TURN = 501,	// 야 나 다했다!

	NOTIFY_DICE_VALUE = 502,	// 서버야 주사위 값 받아라

	NOTIFY_TERRAIN_TYPE = 503,	// A클라이언트가 선택한 지형 카드 서버에 알림

	NOTIFY_TERRAIN_INDEXS = 504,	// A클라이언트가 변경한 지형 카드 인덱스를 서버에 전달

	NOTIFY_EVENTCARD_INDEX = 505, // A클라이언트가 사용한 이벤트 카드 인덱스 
};

enum class SCENE_NAME {
	TITLE_SCENE	,	// 로고 노출 및 연결
	LOGIN_SCENE	,	// 계정 로그인
	MainUI_SCENE,
	LOBBY_SCENE	,	// 이거 쫌 그래 사실 별로야
	ROOM_SCENE	,		// 방으로 연결
	INGAME_SCENE	// 얍얍얍 인게임 얍얍얍
};


