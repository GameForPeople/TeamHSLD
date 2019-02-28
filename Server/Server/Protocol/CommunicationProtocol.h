#pragma once

/*
CommunicationProtocol Header

 1. ���������, std::string, std::vector<int> ��� char[], int[]�� �����. (�̴� NETWORK_UTIL::SendProcess���� ȣȯ�� ����)
	- ������� : ���� SendProcess���� BaseStruct�� �����Լ��� ȣ���ϵ��� ��. �ش� �Լ� ���ڴ� char*(Byte*) // &(ptr->buffer[0]) �� ���ڷ� ������, 
				�� ��ӵ� ��ü ������ �������̵� �� �Լ����� ���� ó��. �� ����, String�� Vector�� ����ص� ���� ( �����ϰ� String_view ��밡��)

 2. ���� BaseStruct�� 50% NonCopyable Struct��. 
	- ���� �Ҵ� �����ڴ� ��������, ��������� ���� ����. (�̵������ڸ� ������, �ƿ� �� ��Ȳ�� �Ѽž���)

 3. ���� ��ü�� String -> char*���� ��ȯ��, for������, strncpy(or memcpy) �� ������.
	- ������ �������� ����. �̹� ����� ���ڿ��� Ȱ������ ���Ѵٴ� ���� ū ����. �̸� ���� �� �� ������ ����ؾ���.

 4. ���������� ���� ���ڰ� Called by Const Value �ε�, �̰� ����. Called by Const Ref�� ��ȯ�� ���������� Ȯ���� �ʿ䰡 ���� 

 5. 3������ �ȵ� ��, ���ڸ� ���̱� ����... char �迭�� �����͸� �ѱ�� �� ���? -> ���� ���� �ѹ��ϴ� �������� �̵��� ���� �� ����.

 */


// �����ؾ��մϴ�! �׻� Ŭ������ 4����Ʈ ������ �����մϴ�. SIMD �ƴϰ� �̰� ������...
#include "../PCH/stdafx.h"

enum Protocol {

	// for Server
	END_SEND			=	-1	,
	START_RECV			=	0	,

	// for OtherSceneFunction(TitleScene Process)
	DEMAND_ANNOUNCEMENT =	10	,	// UDP�� ANNOUNCEMENT�� �޾��� ��, Ŭ���̾�Ʈ�� ������.
	ANSWER_ANNOUNCEMENT =	11	,   // ANNOUNCEMENT�� ���� �����͸� Ŭ���̾�Ʈ���� ������ ��,

	DEMAND_ADD_CHAT		=	12	,	// CHAT ������ �߰� ��, �Ҹ���� �Լ�.
	ANSWER_ADD_CHAT		=	13	,	// �ش� ���뿡 �亯. �� �������� ����.

	// for LoginScene
	DEMAND_LOGIN		=	100	,
	FAIL_LOGIN			=	101	,
	PERMIT_LOGIN		=	102	,

	CHANGE_Nickname		=	103 ,	// Client To Server
	PERMIT_Nickname		=	104 ,	// Server to Client - buffer
	
	// for MainUiScene
	DEMAND_FRIEND_INFO	=	201	,	// Client to Server - ģ�� ���� �����ּ���.
	PERMIT_FRIEND_INFO	=	202 ,	// Server to Client

	DEMAND_FRIEND_INVITE =	203 ,   // Client to Server - with Index , AŬ���̾�Ʈ��, index(B) Ŭ���̾�Ʈ���� ���� �÷����ϰ� �ʹٰ� ��û.
	NOTIFY_FRIEND_INVITE =  204 ,	// Server to Client - with bool , Search�� ����, nullptr�� �ƴ� ���, true�� UDP�� Push

	ANSWER_FRIEND_INVITE =	205	,	// Client to Server  - index(B) Ŭ���̾�Ʈ���Լ� ����� ��, �ٵ� �� �� ���� ����� true�� ���, 
	GUESTCHECK_FRIEND_INVITE =	206 ,	// Server to Client  - ���� AŬ���̾�Ʈ�� roomPtr�� ������ ��ȿ�� �˻� ��, �����ϸ� true.

 	DELAY_FRIEND_INVITE  =  207 ,	// Client to Server  - �� ��, �� 7�ʳ� ��޷ȴµ� �� ������? // ���� ������, DEMAND_ROOMHOST ȣ��.
	HOSTCHECK_FRIEND_INVITE = 208,  // Server to Client - �� GuestUser nullptr �´�. ���Ӿ��Ϸ��� ��.

		// for MainUiScene -- friend
	DEMAND_MAKE_FRIEND	=	209	,	// ģ���ϰ� �;��. (���� ��� �ȸ���. -> ���� 4�� �̻��̸� ���ʿ� �ȵǰ�, ������ 4�� ���ϸ� OK)
	CHECK_DEMAND_MAKE_FRIEND	=	210 ,	// UDP Message ���� ����.

	DEMAND_MAKE_FRIEND_INFO = 211,	// �г��� ���� �� ���� ���� ���� �䱸
	NOTIFY_MAKE_FRIEND_INFO = 212,  // �� �г��� �̷� �ְ� ������ ģ������

	ANSWER_MAKE_FRIEND = 213,		// Ŭ��κ��� �亯�� ��. ģ������ ������ ����.
	CHECK_ANSWER_MAKE_FRIEND = 214, // �ش� �亯 ó���� �������ο� ���� Ȯ�ν�����.

		// for MainUiScene -- shop
	DEMAND_BUY_ITEM	=	215,		// ������ �ε��� �����.
	ANSWER_BUY_ITEM =	216,		// ������ Ʈ��, �������� False�����.

		// for MainUiScene -- VipCode
	DEMAND_VIP_CODE =	217,		// �� �̰� VIP Code �¾ƿ�?
	ANSWER_VIP_CODE =	218,		// �³� Ʋ����~

		// for MainUiScene -- Character Buy
	DEMAND_BUY_CHARACTER = 219,		// �� ĳ���� ��Կ�
	ANSWER_BUY_CHARACTER = 220,		// �� ��� �ֳ� ���� ��������� �Ⱦ��ٰ�

	CHANGE_ACTIVE_CHARACTER = 221,	// �� ĳ���� �̰ɷ� �ٲܰԿ�.
	PERMIT_ACTIVE_CHARACTER = 222,  // OK! �˾Ѿ�. (��� ����)

	// for LobbyScene �� �κ� ��
	DEMAND_MAKEROOM		=	301	,
	PERMIT_MAKEROOM		=	302	, // ���⼭, ���� �����ڸ� ȣ���ϴµ�, ���� ���� ������, ��� �¸��������� ���� ( �水ü ������ ����)
	DEMAND_JOINROOM		=	303	,
	PERMIT_JOINROOM		=	304	, // ���⼭, �߰��� ����(���� ��, �¸����� �� �߰� ����)
	FAIL_JOINROOM		=	305	,
	
	// �� Lobby Protocol
	DEMAND_RANDOM_MATCH =	311	,   // �ش� ���������� ���� ���, ���� �� ���� ���� ���θ� Ȯ���ϰ�, �Ұ��� �� ���� �����մϴ�.
	PERMIT_MAKE_RANDOM	=	312	,   // ���� ������ٰ� �˸��ϴ�.
	PERMIT_JOIN_RANDOM	=	313	,   // �濡 �����ߴٰ� �˸��ϴ�.

	DEMAND_GUEST_JOIN	=	314	,   // �濡 �Խ�Ʈ�� �����ߴ����� ���θ� Ȯ���մϴ�.
	PERMIT_GUEST_JOIN	=	315 ,	// �濡 �Խ�Ʈ�� ��������.
	PERMIT_GUEST_NOT_JOIN = 316 ,	// �濡 �Խ�Ʈ�� ��������.

	DEMAND_EXIT_RANDOM	=	317	,	// ����ť�� ������� �˸�.
	ANSWER_EXIT_RANDOM	=	318	,	// ����ť ��� ���ο� ���� �˸� (������ ������, ���нô�, Guest_Join ó����)

	DEMAND_FRIEND_JOIN  =	319 ,	// [FRIEND] ģ���� �ʴ뿡 ���ϴ� �� ���� �Ǵ� (Friend ����)
	ANSWER_FRIEND_JOIN	=	320	,	// [FRIEND] �̿� ���� ��� ����.

	// for RoomScene �� ���
	DEMAND_ROOMHOST		=	400	,
	ROOMSTATE_VOID		=	410	,
	ROOMSTATE_GUESTIN	=	411	,
	
	// �� Room Protocol
	DEMAND_ENEMY_CHARACTER = 421,   // �� ĳ���� ���� ������ �����ϰ�,
	PERMIT_ENEMY_CHARACTER = 422,   // ������ ĳ���� ������ �޾ƿɴϴ�.


	// for GameScene // �ܹ������� ���� ���� �Ϸ�(2018/10/19ver)
	VOID_GAME_STATE =500 ,	// ���潺 ���� ģ����, �� �������� �����޾�??�� ���庽

	NOTIFY_CHANGE_TURN = 501,	// �� �� ���ߴ�!

	NOTIFY_DICE_VALUE = 502,	// ������ �ֻ��� �� �޾ƶ�

	NOTIFY_TERRAIN_TYPE = 503,	// AŬ���̾�Ʈ�� ������ ���� ī�� ������ �˸�

	NOTIFY_TERRAIN_INDEXS = 504,	// AŬ���̾�Ʈ�� ������ ���� ī�� �ε����� ������ ����

	NOTIFY_EVENTCARD_INDEX = 505, // AŬ���̾�Ʈ�� ����� �̺�Ʈ ī�� �ε��� 

	DISCONNECTED_ENEMY_CLIENT = 506, // ����� Ŭ���̾�Ʈ ��Ʈ��ũ ���ܿ� ���� ó�� ��û (roomProtocol �� Ŭ�󿡼� ����) - (Server to client)
	DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT = 506, // ����� Ŭ���̾�Ʈ�� �������� �����ϰ�, �ش� ó���� ��û��. (Client to Server)
	
	NOTIFY_GAME_READY = 507,	// �̰� �ְ� ������ ����յ�. ? �ٵ� �� ���� �̰� ����� �ȳ���;;

	NOTIFY_EMOJI = 508,	// �̸�Ƽ�� // �̸��� �����Դϴ�.

	NOTIFY_GAME_END = 509,	// ���� �������
	NOTIFY_GAME_BUFFER = 510,	// �ش� ������ ���� �����Դϴ�.

	NOTIFY_EVENT_CARD = 511
};

enum class SCENE_NAME {
	TITLE_SCENE	,	// �ΰ� ���� �� ����
	LOGIN_SCENE	,	// ���� �α���
	MainUI_SCENE,
	LOBBY_SCENE	,	// �̰� �� �׷� ��� ���ξ�
	ROOM_SCENE	,		// ������ ����
	INGAME_SCENE	// ���� �ΰ��� ����
};


