#pragma once

// �����ؾ��մϴ�! �׻� Ŭ������ 4����Ʈ ������ �����մϴ�. SIMD �ƴϰ� �̰� ������...
#include "../stdafx.h"

#define ID_MAX_LEN 20

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

	DISCONNECTED_ENEMY_CLIENT = 498, // ����� Ŭ���̾�Ʈ ��Ʈ��ũ ���� ó�� (roomProtocol �� Ŭ�󿡼� ����)


	// for GameScene
	
	DEMAND_GAME_STATE	=	500 ,	// ���潺 ���� ģ����, �� �������� �����޾�??�� ���庽
	VOID_GAME_STATE		=	501 ,	// �� ����� ������ ������ �ƹ��͵� ���߾�!

	NOTIFY_END_OF_TURN	=	502	,	// �� �� ���ߴ�!
	NOTIFY_CHANGE_TURN =	503	,	// �� �� ���ߴ�!
	
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

public:
	//BaseStruct(const BaseStruct&) = delete; // �޸𸮿��� ������ �����Ϸ��� ��¿������ �ּ�ó��
	BaseStruct& operator=(const BaseStruct&) = delete;
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

#pragma region [LOGIN SCENE STRUCT]

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

#pragma endregion


#pragma region [LOBBY SCENE STRUCT]

#pragma region [OLD LOBBY SCENE]

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

#pragma endregion


#pragma region [NEW LOBBY SCENE]

// �� LobbyScene

// type 311
struct DemandRandomMatchStruct : public BaseStruct {
	// �̰� �Ⱦ��ϴ�.
	DemandRandomMatchStruct() = delete;
};

// type 312
struct PermitMakeRandomStruct : public BaseStruct {
	int roomIndex;
	int isHostFirst;
	int playerMissionIndex;
	int enemyMissionIndex;
	int subMissionIndex;

	PermitMakeRandomStruct(const int InRoomIndex, const int InIsHostFirst, const int InPlayerMissionIndex, const int InEnemyMissionIndex, const int InSubMissionIndex)
		: roomIndex(InRoomIndex), isHostFirst(InIsHostFirst) ,playerMissionIndex(InPlayerMissionIndex), enemyMissionIndex(InEnemyMissionIndex), subMissionIndex(InSubMissionIndex)
	{};

	~PermitMakeRandomStruct() = default;
};

//type 313
struct PermitJoinRandomStruct : public BaseStruct {
	int roomIndex;
	int isHostFirst;
	int playerMissionIndex;
	int enemyMissionIndex;
	int subMissionIndex;
	int idSize;
	//string enemyId;
	char enemyId[ID_MAX_LEN];

	PermitJoinRandomStruct(const int InRoomIndex, const int InIsHostFirst, const int InPlayerMissionIndex, const int InEnemyMissionIndex, const int InSubMissionIndex, const string& InEnemyId )
		: roomIndex(InRoomIndex), isHostFirst(InIsHostFirst), playerMissionIndex(InPlayerMissionIndex), enemyMissionIndex(InEnemyMissionIndex), subMissionIndex(InSubMissionIndex), idSize(InEnemyId.size()) //, enemyId(InEnemyID)
	{
		for (int i = 0; i < idSize; ++i)
		{
			enemyId[i] = InEnemyId[i];
		}
	};

	~PermitJoinRandomStruct() = default;
};

//type 314
struct DemandGuestJoinStruct : public BaseStruct {
	DemandGuestJoinStruct() = delete;
};

//type 315
struct PermitGuestJoinStruct : public BaseStruct {
	int idSize;
	//string enemyId;
	char enemyId[ID_MAX_LEN];

	PermitGuestJoinStruct(const string& InEnemyId) :idSize(InEnemyId.size()) //, enemyId(InEnemyId)
	{
		for (int i = 0; i < idSize; ++i)
		{
			enemyId[i] = InEnemyId[i];
		}
	};

	~PermitGuestJoinStruct() = default;
};

//type 316
struct PermitGuestNotJoinStruct : public BaseStruct {
	PermitGuestNotJoinStruct() = delete;
};

#pragma endregion

#pragma endregion


#pragma region [ROOM SCENE STRUCT]
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


// type 421 
struct DemandEnemyCharacterStruct : public BaseStruct {
	int playerCharacterIndex;
	//�Ƚ��~
	DemandEnemyCharacterStruct() = delete;
};

struct PermitEnemyCharacterStruct : public BaseStruct {
	int playerCharacterIndex;

	PermitEnemyCharacterStruct(const int InPlayerCharacterIndex) : playerCharacterIndex(InPlayerCharacterIndex)
	{};

	~PermitEnemyCharacterStruct() = default;
};


#pragma endregion


#pragma region [INGAME SCENE STRUCT]

// type 502, 503 // �̰� �Ⱦ�
struct ChangeTurnStruct : public BaseStruct
{
private:	// ���� ���Ƴ�.
	__inline ChangeTurnStruct() = default;
	__inline ~ChangeTurnStruct() = default;
};

// type 511, 521 ???????? �̰� �Ⱦ���...?
struct VoidStruct : public BaseStruct
{
private:	// ���� ���Ƴ�.
	__inline VoidStruct() = default;
	__inline ~VoidStruct() = default;
};


// type 512, 522
struct ChangePlanetStruct : public BaseStruct
{
	int					terrainType;
	int					changeTerrainCount;
	int					terrainIndex[12];	//Dice 12
	//std::vector<int>	terrainIndex;

public:
	__inline ChangePlanetStruct(const int InTerrainType, const int InChangeTerrainCount, const int* InTerrainIndex)
		: terrainType(InTerrainType), changeTerrainCount(InChangeTerrainCount)
	{
		// ��ģ���̼���??
		memcpy(terrainIndex, InTerrainIndex, changeTerrainCount * sizeof(int));
	}

	__inline ~ChangePlanetStruct() = default;
};

//type 513, 523
struct ActionEventCardTerrainStruct : public BaseStruct
{
	int					eventCardType;
	int					terrainType;
	int					changeTerrainCount;
	int					terrainIndex[12];	//Dice 12

public:
	__inline ActionEventCardTerrainStruct(const int InEventCardType, const int InTerrainType, const int InChangeTerrainCount, const int* InTerrainIndex)
		: eventCardType(InEventCardType), terrainType(InTerrainType), changeTerrainCount(InChangeTerrainCount)
	{
		// �̰� ��¥ �Ƿ���?
		memcpy(terrainIndex, InTerrainIndex, changeTerrainCount * sizeof(int));
	}

	__inline ~ActionEventCardTerrainStruct() = default;
};

//type 514, 524
struct ActionEventCardDiceBuffStruct : public BaseStruct
{
	int					eventCardType;

public:
	__inline ActionEventCardDiceBuffStruct(const int InEventCardType) : eventCardType(InEventCardType)
	{}

	__inline ~ActionEventCardDiceBuffStruct() = default;
};

//type 515, 525
struct ActionEventCardDefenseStruct : public BaseStruct
{
	//?????????????????????????????????????????????????????????????????? �̰� ������ ���� ��� ó���ؾ����� �𸣰ھ�...

	__inline ActionEventCardDefenseStruct() {}
	__inline ~ActionEventCardDefenseStruct() {}
};

#pragma endregion