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

	// for OtherSceneFunction(TitleScene Process)
	DEMAND_ANNOUNCEMENT =	10	,	// UDP로 ANNOUNCEMENT를 받았을 때, 클라이언트가 전송함.
	ANSWER_ANNOUNCEMENT =	11	,   // ANNOUNCEMENT에 대한 데이터를 클라이언트에게 전달할 때,

	DEMAND_ADD_CHAT		=	12	,	// CHAT 내용을 추가 시, 불리우는 함수.
	ANSWER_ADD_CHAT		=	13	,	// 해당 내용에 답변. 빈 프로토콜 전송.

	// for LoginScene
	DEMAND_LOGIN		=	100	,
	FAIL_LOGIN			=	101	,
	PERMIT_LOGIN		=	102	,

	CHANGE_Nickname		=	103 ,	// Client To Server
	PERMIT_Nickname		=	104 ,	// Server to Client - buffer
	
	// for MainUiScene
	DEMAND_FRIEND_INFO	=	201	,	// Client to Server - 친구 정보 보내주세요.
	PERMIT_FRIEND_INFO	=	202 ,	// Server to Client

	DEMAND_FRIEND_INVITE =	203 ,   // Client to Server - with Index , A클라이언트가, index(B) 클라이언트에게 같이 플레이하고 싶다고 요청.
	NOTIFY_FRIEND_INVITE =  204 ,	// Server to Client - with bool , Search를 통해, nullptr이 아닐 경우, true및 UDP에 Push

	ANSWER_FRIEND_INVITE =	205	,	// Client to Server  - index(B) 클라이언트에게서 대답이 옴, 근데 이 때 만약 대답이 true일 경우, 
	GUESTCHECK_FRIEND_INVITE =	206 ,	// Server to Client  - 최초 A클라이언트와 roomPtr의 포인터 유효성 검사 후, 가능하면 true.

 	DELAY_FRIEND_INVITE  =  207 ,	// Client to Server  - 야 씨, 나 7초나 기달렸는데 왜 대답없냐? // 이전 까지는, DEMAND_ROOMHOST 호출.
	HOSTCHECK_FRIEND_INVITE = 208,  // Server to Client - 야 GuestUser nullptr 맞다. 게임안하려나 봐.

		// for MainUiScene -- friend
	DEMAND_MAKE_FRIEND	=	209	,	// 친구하고 싶어요. (상대방 허락 안맡음. -> 내가 4명 이상이면 애초에 안되고, 상대방이 4명 이하면 OK)
	CHECK_DEMAND_MAKE_FRIEND	=	210 ,	// UDP Message 전송 여부.

	DEMAND_MAKE_FRIEND_INFO = 211,	// 닉네임 정보 및 관련 정보 전달 요구
	NOTIFY_MAKE_FRIEND_INFO = 212,  // 야 닉네임 이런 애가 너한테 친구하재

	ANSWER_MAKE_FRIEND = 213,		// 클라로부터 답변이 옴. 친구할지 안할지 여부.
	CHECK_ANSWER_MAKE_FRIEND = 214, // 해당 답변 처리중 에러여부에 대해 확인시켜줌.

		// for MainUiScene -- shop
	DEMAND_BUY_ITEM	=	215,		// 아이템 인덱스 날라옴.
	ANSWER_BUY_ITEM =	216,		// 샀으면 트루, 못샀으면 False날라옴.

		// for MainUiScene -- VipCode
	DEMAND_VIP_CODE =	217,		// 님 이거 VIP Code 맞아요?
	ANSWER_VIP_CODE =	218,		// 맞네 틀리네~

		// for MainUiScene -- Character Buy
	DEMAND_BUY_CHARACTER = 219,		// 이 캐릭터 살게요
	ANSWER_BUY_CHARACTER = 220,		// 응 살수 있나 보고 살수있으면 팔아줄게

	CHANGE_ACTIVE_CHARACTER = 221,	// 나 캐릭터 이걸로 바꿀게요.
	PERMIT_ACTIVE_CHARACTER = 222,  // OK! 알앗어. (고냥 보냄)

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

	DEMAND_FRIEND_JOIN  =	319 ,	// [FRIEND] 친구가 초대에 응하는 지 여부 판단 (Friend 전용)
	ANSWER_FRIEND_JOIN	=	320	,	// [FRIEND] 이에 따른 대답 해줌.

	// for RoomScene 구 룸씬
	DEMAND_ROOMHOST		=	400	,
	ROOMSTATE_VOID		=	410	,
	ROOMSTATE_GUESTIN	=	411	,
	
	// 신 Room Protocol
	DEMAND_ENEMY_CHARACTER = 421,   // 내 캐릭터 변경 정보를 전송하고,
	PERMIT_ENEMY_CHARACTER = 422,   // 상대방의 캐릭터 정보를 받아옵니다.


	// for GameScene // 단방향으로 수정 적용 완료(2018/10/19ver)
	VOID_GAME_STATE =500 ,	// 디펜스 턴인 친구가, 야 공격턴이 공격햇어??를 여쭤봄

	NOTIFY_CHANGE_TURN = 501,	// 야 나 다했다!

	NOTIFY_DICE_VALUE = 502,	// 서버야 주사위 값 받아라

	NOTIFY_TERRAIN_TYPE = 503,	// A클라이언트가 선택한 지형 카드 서버에 알림

	NOTIFY_TERRAIN_INDEXS = 504,	// A클라이언트가 변경한 지형 카드 인덱스를 서버에 전달

	NOTIFY_EVENTCARD_INDEX = 505, // A클라이언트가 사용한 이벤트 카드 인덱스 

	DISCONNECTED_ENEMY_CLIENT = 506, // 상대편 클라이언트 네트워크 예외에 대한 처리 요청 (roomProtocol 및 클라에서 적용) - (Server to client)
	DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT = 506, // 상대편 클라이언트가 나갔음을 인지하고, 해당 처리를 요청함. (Client to Server)
	
	NOTIFY_GAME_READY = 507,	// 이거 주고 받으면 꼴토롱됨. ? 근데 왜 있지 이거 기억이 안나네;;

	NOTIFY_EMOJI = 508,	// 이모티콘 // 이모지 관련입니다.

	NOTIFY_GAME_END = 509,	// 게임 끝났어요
	NOTIFY_GAME_BUFFER = 510,	// 해당 게임은 끝난 게임입니다.

	NOTIFY_EVENT_CARD = 511
};

enum class SCENE_NAME {
	TITLE_SCENE	,	// 로고 노출 및 연결
	LOGIN_SCENE	,	// 계정 로그인
	MainUI_SCENE,
	LOBBY_SCENE	,	// 이거 쫌 그래 사실 별로야
	ROOM_SCENE	,		// 방으로 연결
	INGAME_SCENE	// 얍얍얍 인게임 얍얍얍
};


