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
#include "../stdafx.h"

enum Protocol {

	// for Server
	END_SEND			=	-1	,
	START_RECV			=	0	,

	// for LoginScene
	DEMAND_LOGIN		=	100	,
	FAIL_LOGIN			=	101	,
	PERMIT_LOGIN		=	102	,
	
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
	PERMIT_GUEST_JOIN	=	315,	// �濡 �Խ�Ʈ�� ��������.
	PERMIT_GUEST_NOT_JOIN = 316,	// �濡 �Խ�Ʈ�� ��������.


	// for RoomScene �� ���
	DEMAND_ROOMHOST		=	400	,
	ROOMSTATE_VOID		=	410	,
	ROOMSTATE_GUESTIN	=	411	,
	
	// �� Room Protocol
	DEMAND_ENEMY_CHARACTER = 421,   // �� ĳ���� ���� ������ �����ϰ�,
	PERMIT_ENEMY_CHARACTER = 422,   // ������ ĳ���� ������ �޾ƿɴϴ�.

	DISCONNECTED_ENEMY_CLIENT = 498, // ����� Ŭ���̾�Ʈ ��Ʈ��ũ ���ܿ� ���� ó�� ��û (roomProtocol �� Ŭ�󿡼� ����) - (Server to client)
	DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT = 499, // ����� Ŭ���̾�Ʈ�� �������� �����ϰ�, �ش� ó���� ��û��. (Client to Server)

	// for GameScene
	DEMAND_GAME_STATE	=	500 ,	// ���潺 ���� ģ����, �� �������� �����޾�??�� ���庽
	
	VOID_GAME_STATE		=	501 ,	// �� ����� ������ ������ �ƹ��͵� ���߾�!

	CHANGE_TURN_TO_SERVER =	502	,	// �� �� ���ߴ�!
	CHANGE_TURN_TO_CLIENT =	503	,	// �� �� ���ߴ�!

	DICE_VALUE_TO_SERVER = 504,	// ������ �ֻ��� �� �޾ƶ�
	DICE_VALUE_TO_CLIENT = 505,	// Ŭ��� �� �ֻ��� �� �̰ž�

	TERRAIN_TYPE_TO_SERVER = 506,	// AŬ���̾�Ʈ�� ������ ���� ī�� ������ �˸�
	TERRAIN_TYPE_TO_CLIENT = 507,  // ����� Ŭ���̾�Ʈ���� ���� ī�� (�ε���) ����.

	TERRAIN_INDEX_TO_SERVER = 508,	// AŬ���̾�Ʈ�� ������ ���� ī�� �ε����� ������ ����
	TERRAIN_INDEX_TO_CLIENT = 509,  // ����� Ŭ���̾�Ʈ���� ���� ī�� (�ε���) ����.

	//? �̰� �� ����غ��� ��.? �ε����� �ѱ�� �ٸ��Ŵ� �� ���� ������ �� ��? --> ������? �翬�Ѱ� 
	// �ʿ���°� �߰��� �Ⱥ�����, (�� ����, ���� ���� ��,)
	EVENTCARD_INDEX_TO_SERVER = 510, // AŬ���̾�Ʈ�� ����� �̺�Ʈ ī�� �ε��� 
	EVENTCARD_INDEX_TO_CLIENT = 511, // ����� Ŭ�󿡰� �ش� �̺�Ʈ ī�� �ε��� ����
};

enum class SCENE_NAME {
	TITLE_SCENE	,	// �ΰ� ���� �� ����
	LOGIN_SCENE	,	// ���� �α���
	MainUI_SCENE,
	LOBBY_SCENE	,	// �̰� �� �׷� ��� ���ξ�
	ROOM_SCENE	,		// ������ ����
	INGAME_SCENE	// ���� �ΰ��� ����
};

enum TERRAIN_TYPE {

};

