#pragma once

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
	
	// for LobbyScene
	DEMAND_MAKEROOM		=	301	,
	PERMIT_MAKEROOM		=	302	, // ���⼭, ���� �����ڸ� ȣ���ϴµ�, ���� ���� ������, ��� �¸��������� ���� ( �水ü ������ ����)
	DEMAND_JOINROOM		=	303	,
	PERMIT_JOINROOM		=	304	, // ���⼭, �߰��� ����(���� ��, �¸����� �� �߰� ����)
	FAIL_JOINROOM		=	305	,
	
	// for RoomScene
	DEMAND_ROOMHOST		=	400	,
	ROOMSTATE_VOID		=	410	,
	ROOMSTATE_GUESTIN	=	411	,
	
	// for GameScene
	
	DEMAND_GAME_STATE	=	500 ,	// ���潺 ���� ģ����, �� �������� �����޾�??�� ���庽
	VOID_GAME_STATE		=	501 ,	// �� ����� ������ �ƹ��͵� ���߾�!

	VOID_CLIENT_TO_SERVER = 511 ,				// ������ Ŭ���̾�Ʈ�� �ð��ʰ��� �ƹ��͵� ������ ���� ��,
	CHANGE_PLANET_CLIENT_TO_SERVER = 512 ,		// ������ Ŭ���̾�Ʈ�� ���� �مf�� ��,
	ACTION_EVENTCARD_TERRAIN_CLIENT_TO_SERVER = 513,	// ������ Ŭ���̾�Ʈ�� �̺�Ʈ ī��(����, ��������) ó��
	ACTION_EVENTCARD_DICEBUFF_CLIENT_TO_SERVER = 514,	// ������ Ŭ���̾�Ʈ�� �̺�Ʈ ī��(����, �ֻ��� ����) ó��
	ACTION_EVENTCARD_DEFENSE_CLIENT_TO_SERVER = 515,	// ??�� Ŭ���̾�Ʈ�� �̺�Ʈ ī�� ���� ó��??


	VOID_SERVER_TO_CLIENT = 521 ,				// ������ ������ Ŭ���̾�Ʈ���� VOID���� �ٸ�, �̰� ������ �� ������!
	CHANGE_PLANET_SERVER_TO_CLIENT = 522 ,		// ������ ������ Ŭ���̾�Ʈ���� ���� Ŭ���̾�Ʈ�� �ٲ� �༺ ������ ����
	ACTION_EVENTCARD_TERRAIN_SERVER_TO_CLIENT = 523,	// ������ ������ Ŭ���̾�Ʈ���� ������ Ŭ���̾�Ʈ �̺�Ʈ ī��(����, ��������) ������ ����
	ACTION_EVENTCARD_DICEBUFF_SERVER_TO_CLIENT = 524,	// ������ ������ Ŭ���̾�Ʈ���� ������ Ŭ���̾�Ʈ �̺�Ʈ ī��(����, �ֻ��� ����) ó��
	ACTION_EVENTCARD_DEFENSE_SERVER_TO_CLIENT = 525,	// ??�� Ŭ���̾�Ʈ�� �̺�Ʈ ī�� ���� ó��??

	// Not yet used
	DEMAND_DICE_CLIENT_TO_SERVER = 521,		// �ֻ��� ���ڸ� �䱸�� ��,
	PERMIT_DICE_SERVER_TO_CLIENT = 522 ,

	DEMAND_EVENTCARD_CLIENT_TO_SERVER = 523,	// ���뵦���� �̺�Ʈ ī�带 �䱸�� ��,
	PERMIT_EVENTCARD_SERVER_TO_CLIENT = 524 ,

	DEMAND_EVENTCARD_DICE_CLIENT_TO_SERVER = 525 , // �̺�Ʈ ī���� ���ڸ� �䱸�� ��
	PERMIT_EVENTCARD_DICE_SERVER_TO_CLIENT = 526 

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

// ��� ��Ʈ��ó
struct BaseStruct {
public:
	BaseStruct() = default;
	~BaseStruct() = default;

private:
	//BaseStruct(const BaseStruct&); // �޸𸮿��� ������ �����Ϸ��� ��¿������ �ּ�ó��
	BaseStruct& operator=(const BaseStruct&);
};

//struct BaseSendStruct : public BaseStruct {
//	int sendType{};
//	BaseStruct* dataBuffer;
//
//public:
//	__inline BaseSendStruct(const int InSendType, BaseStruct* InStruct) : sendType(InSendType), dataBuffer(InStruct)
//	{};
//	
//	__inline BaseSendStruct() = default;
//	
//	__inline ~BaseSendStruct()
//	{
//		if (dataBuffer != nullptr) delete dataBuffer;
//	}
//};

// type 100�϶�, ������ �ٷ� ���� �����ִ� ����ü
struct DemandLoginStruct : public BaseStruct {
	int type{};	// 1�϶��� �α���, 2�϶��� ȸ������
	int PW{};
	int IDSize{};
	std::string ID;
};

// type 100�϶�, ������ �ٷ� ���� �����ִ� ����ü
struct DemandLoginCharStruct : public BaseStruct {
	int type{};	// 1�϶��� �α���, 2�϶��� ȸ������
	int PW{};
	int IDSize{};
	char ID[20]{};
	
	DemandLoginCharStruct() { ID[0] = { 0, }; };
	~DemandLoginCharStruct() = default;
};

// type 101 Server -> Client �α��� ����, ȸ������ ����
struct FailLoginStruct : public BaseStruct {
	int type{}; // 1�϶� �α��� ���� ���̵�, 2�϶� �α��� �߸��� ��й�ȣ, 3�϶� �̹� �α����� ���̵�, 4�϶� ȸ������ �ߺ��� ���̵�!
	// ���̵�, ��й�ȣ ���ռ���, Ŭ��ܿ��� üũ�ϼ���!!

	__inline FailLoginStruct(int InType) : type(InType)
	{};

	__inline FailLoginStruct() = default;
	__inline ~FailLoginStruct() = default;
};

// type 102 Server -> Client �α��� ����, Lobby����, �������� ����
struct PermitLoginStruct : public BaseStruct {
	int winCount{};
	int loseCount{}; 
	int money{};

	__inline PermitLoginStruct(const int InWin,const int InLose,const int InMoney) : winCount(InWin) , loseCount(InLose) , money(InMoney)
	{};

	__inline PermitLoginStruct() = default;
	__inline ~PermitLoginStruct() = default;
};

// type 301
struct DemandMakeRoomStruct : public BaseStruct {
	//�Ⱦ��ϴ�. -> ? �Լ�ȭ�ϸ鼭 ���� �ɷ� �ٲ��µ�? ����
	// ? ���? �̰� �Ⱦ��� ��ģ���� ���ú걸��..���m ���ú�� ��� �޸𸮷� ���� ��O
};

// type 302
struct PermitMakeRoomStruct : public BaseStruct {
	// ��� �̰͵��Ⱦ�����. -> ? �ִ� ��¥�� ��
	int roomIndex;

	__inline PermitMakeRoomStruct(int InRoomIndex) : roomIndex(InRoomIndex)
	{};

	__inline PermitMakeRoomStruct() = default;
	__inline ~PermitMakeRoomStruct() = default;
};

//type 303
struct DemandJoinRoomStruct : public BaseStruct {
	int roomIndex;

	__inline DemandJoinRoomStruct(int InRoomIndex) : roomIndex(InRoomIndex)
	{};

	__inline DemandJoinRoomStruct() = default;
	__inline ~DemandJoinRoomStruct() = default;
};

//type 304
struct PermitJoinRoomStruct : public BaseStruct {
	int roomIndex;
	int idSize;
	string enemyId;

	__inline PermitJoinRoomStruct(int InRoomIndex, string InEnemyID)
		: roomIndex(InRoomIndex), idSize(InEnemyID.size()), enemyId(InEnemyID)
	{};

	__inline PermitJoinRoomStruct() = default;
	__inline ~PermitJoinRoomStruct() = default;
};

//type 305
struct FailJoinRoomStruct : public BaseStruct {
	int failReason;

	__inline FailJoinRoomStruct(int InFailReason) : failReason(InFailReason)
	{};

	__inline FailJoinRoomStruct() = default;
	__inline ~FailJoinRoomStruct() = default;
};

//type 411
struct RoomStateGuestInStruct : public BaseStruct
{
	int idSize;
	string enemyId;

	__inline RoomStateGuestInStruct(string InEnemyID)
		: idSize(InEnemyID.size()), enemyId(InEnemyID)
	{};

	__inline RoomStateGuestInStruct() = default;
	__inline ~RoomStateGuestInStruct() = default;
};

// type 401 -> One Player Changed //
struct OnePlayerChanged : public BaseStruct
{
	int index{};
	int mixedData{};

	__inline OnePlayerChanged() = default;
	__inline OnePlayerChanged(int InIndex, int InLeftOrRight, int isJumping ) :index(InIndex)
	{
		mixedData = InLeftOrRight * 10 + isJumping;
	};
};


//
//

// type 501, 511 ???????? �̰� �Ⱦ�����
struct VoidStruct : public BaseStruct
{
	__inline ~VoidStruct() = default;

private:
	__inline VoidStruct() = default;
};

// type 502, 512
struct ChangePlanetStruct : public BaseStruct
{
	int					terrainType;
	int					changeTerrainCount;
	int					terrainIndex[12];	//Dice 12
	//std::vector<int>	terrainIndex;

	__inline ChangePlanetStruct(const int InTerrainType, const int InChangeTerrainCount, const int* InTerrainIndex)
		: terrainType(InTerrainType), changeTerrainCount(InChangeTerrainCount)
	{
		// ��ģ���̼���??
		memcpy(terrainIndex, InTerrainIndex, changeTerrainCount * sizeof(int));
	}

	__inline ~ChangePlanetStruct() = default;
};

//type 503, 513
struct ActionEventCardTerrainStruct : public BaseStruct
{
	int					eventCardType;
	int					terrainType;
	int					changeTerrainCount;
	int					terrainIndex[12];	//Dice 12

	__inline ActionEventCardTerrainStruct(const int InEventCardType, const int InTerrainType, const int InChangeTerrainCount, const int* InTerrainIndex)
		: eventCardType(InEventCardType), terrainType(InTerrainType), changeTerrainCount(InChangeTerrainCount)
	{
		// �̰� ���� ��ģ���̴���
		memcpy(terrainIndex, InTerrainIndex, changeTerrainCount * sizeof(int));
	}

	__inline ~ActionEventCardTerrainStruct() = default;
};

//type 504, 514
struct ActionEventCardDiceBuffStruct : public BaseStruct
{
	int					eventCardType;

	__inline ActionEventCardDiceBuffStruct(const int InEventCardType) : eventCardType(InEventCardType)
	{}

	__inline ~ActionEventCardDiceBuffStruct() 
	{}
};

//type 505, 515
struct ActionEventCardDefenseStruct : public BaseStruct
{
	//?????????????????????????????????????????????????????????????????? �̰� ������ ���� ��� ó���ؾ����� �𸣰ھ�...

	__inline ActionEventCardDefenseStruct() {}
	__inline ~ActionEventCardDefenseStruct() {}
};