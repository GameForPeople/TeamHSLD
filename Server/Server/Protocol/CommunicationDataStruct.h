#pragma once

#include "../stdafx.h"

#include "../Protocol/CommunicationProtocol.h"

#define		MAX_ID_LEN			20
#define		MAX_TERRAIN_LEN		12

// ��� ��Ʈ��ó
struct BaseStruct {
public:
	BaseStruct() = default;
	virtual ~BaseStruct() = 0 {};
public:
	//BaseStruct(const BaseStruct&) = delete; // �α����� ���ݴ���, SetData ���� ������;;
	BaseStruct & operator=(const BaseStruct&) = delete;
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

	//�Ⱦ���.
	DemandLoginStruct() = delete;
	virtual ~DemandLoginStruct() override = default;
};

// type 100�϶�, ������ �ٷ� ���� �����ִ� ����ü
struct DemandLoginCharStruct : public BaseStruct {
	int type{};	// 1�϶��� �α���, 2�϶��� ȸ������
	int PW{};
	int IDSize{};
	char ID[MAX_ID_LEN];

	//DemandLoginCharStruct() { ID[0] = { 0, }; };
	//DemandLoginCharStruct(const int InType, const int InPw, const string& InID) 
	//	: type(InType), PW(InPw), IDSize(InID.size())
	//{
	//	memcpy(ID, InID.c_str(), IDSize);
	//};
	DemandLoginCharStruct() = delete;
	virtual ~DemandLoginCharStruct() override  = default;
};

// type 101 Server -> Client �α��� ����, ȸ������ ����
struct FailLoginStruct : public BaseStruct {
	int type{}; // 1�϶� �α��� ���� ���̵�, 2�϶� �α��� �߸��� ��й�ȣ, 3�϶� �̹� �α����� ���̵�, 4�϶� ȸ������ �ߺ��� ���̵�!
				// ���̵�, ��й�ȣ ���ռ���, Ŭ��ܿ��� üũ�ϼ���!!

	FailLoginStruct(const int InType) : type(InType)
	{};

	//__inline FailLoginStruct() = delete;
	virtual ~FailLoginStruct() override = default;
};

// type 102 Server -> Client �α��� ����, Lobby����, �������� ����
struct PermitLoginStruct : public BaseStruct {
	int winCount{};
	int loseCount{};
	int money{};

	PermitLoginStruct(const int InWin, const int InLose, const int InMoney)
		: winCount(InWin), loseCount(InLose), money(InMoney)
	{};

	//__inline PermitLoginStruct() = delete;
	virtual ~PermitLoginStruct() override = default;
};

#pragma endregion


#pragma region [LOBBY SCENE STRUCT]

#pragma region [OLD LOBBY SCENE]

// type 301
struct DemandMakeRoomStruct : public BaseStruct {
	//�Ⱦ��ϴ�. -> ? �Լ�ȭ�ϸ鼭 ���� �ɷ� �ٲ��µ�? ����
	// ? ���? �̰� �Ⱦ��� ��ģ���� ���ú걸��..���m ���ú�� ��� �޸𸮷� ���� ��O
	DemandMakeRoomStruct() = delete;
};

// type 302
struct PermitMakeRoomStruct : public BaseStruct {
	// ��� �̰͵��Ⱦ�����. -> ? �ִ� ��¥�� ��
	int roomIndex;

	PermitMakeRoomStruct(const int InRoomIndex) : roomIndex(InRoomIndex)
	{};

	//__inline PermitMakeRoomStruct() = delete;
	virtual ~PermitMakeRoomStruct() = default;
};

//type 303
struct DemandJoinRoomStruct : public BaseStruct {
	int roomIndex;

	DemandJoinRoomStruct(const int InRoomIndex) : roomIndex(InRoomIndex)
	{};

	//__inline DemandJoinRoomStruct() = delete;
	virtual ~DemandJoinRoomStruct() override = default;
};

//type 304
struct PermitJoinRoomStruct : public BaseStruct {
	int roomIndex;
	int idSize;
	string enemyId;

	PermitJoinRoomStruct(int InRoomIndex, const string_view& InEnemyID)
		: roomIndex(InRoomIndex), idSize(InEnemyID.size()), enemyId(InEnemyID)
	{};

	//__inline PermitJoinRoomStruct() = delete;
	virtual ~PermitJoinRoomStruct() override = default;
};

//type 305
struct FailJoinRoomStruct : public BaseStruct {
	int failReason;

	FailJoinRoomStruct(int InFailReason)
		: failReason(InFailReason)
	{};

	//__inline FailJoinRoomStruct() = delete;
	virtual ~FailJoinRoomStruct() override = default;
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
		: roomIndex(InRoomIndex), isHostFirst(InIsHostFirst), playerMissionIndex(InPlayerMissionIndex), enemyMissionIndex(InEnemyMissionIndex), subMissionIndex(InSubMissionIndex)
	{};

	virtual ~PermitMakeRandomStruct() override = default;
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
	char enemyId[MAX_ID_LEN];

	PermitJoinRandomStruct(const int InRoomIndex, const int InIsHostFirst, const int InPlayerMissionIndex, const int InEnemyMissionIndex, const int InSubMissionIndex, const string& InEnemyId)
		: roomIndex(InRoomIndex), isHostFirst(InIsHostFirst), playerMissionIndex(InPlayerMissionIndex), enemyMissionIndex(InEnemyMissionIndex), subMissionIndex(InSubMissionIndex), idSize(InEnemyId.size()) //, enemyId(InEnemyID)
	{
		//for (int i = 0; i < idSize; ++i)
		//{
		//	enemyId[i] = InEnemyId[i];
		//}
		memcpy(enemyId, InEnemyId.c_str(), idSize);
	};

	virtual ~PermitJoinRandomStruct() override = default;
};

//type 314
struct DemandGuestJoinStruct : public BaseStruct {
	DemandGuestJoinStruct() = delete;
};

//type 315
struct PermitGuestJoinStruct : public BaseStruct {
	int idSize;
	//string enemyId;
	char enemyId[MAX_ID_LEN];

	PermitGuestJoinStruct(const string& InEnemyId) :idSize(InEnemyId.size()) //, enemyId(InEnemyId)
	{
		//for (int i = 0; i < idSize; ++i)
		//{
		//	enemyId[i] = InEnemyId[i];
		//}
		memcpy(enemyId, InEnemyId.c_str(), idSize);
	};

	virtual ~PermitGuestJoinStruct() override = default;
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

	RoomStateGuestInStruct(const string_view& InEnemyID)
		: idSize(InEnemyID.size()), enemyId(InEnemyID)
	{};

	//__inline RoomStateGuestInStruct() = delete;
	virtual ~RoomStateGuestInStruct() override = default;
};

// type 401 -> One Player Changed //
struct OnePlayerChanged : public BaseStruct
{
	int index{};
	int mixedData{};

	OnePlayerChanged(const int InIndex, const int InLeftOrRight, const int isJumping) :index(InIndex)
	{
		mixedData = InLeftOrRight * 10 + isJumping;
	};
	//__inline OnePlayerChanged() = delete;
	OnePlayerChanged() = default;
	virtual ~OnePlayerChanged() override = default;
};


// type 421 
struct DemandEnemyCharacterStruct : public BaseStruct {
	int playerCharacterIndex;
	//��~ �Ƚ��~
	DemandEnemyCharacterStruct() = delete;
};

struct PermitEnemyCharacterStruct : public BaseStruct {
	int playerCharacterIndex;

	PermitEnemyCharacterStruct(const int InPlayerCharacterIndex)
		: playerCharacterIndex(InPlayerCharacterIndex)
	{};

	virtual ~PermitEnemyCharacterStruct() override = default;
};


#pragma endregion


#pragma region [INGAME SCENE STRUCT]


// type 501 // �̰� �Ⱦ�
struct ChangeTurnStruct : public BaseStruct
{
	ChangeTurnStruct() = delete;
	//__inline ~ChangeTurnStruct() = delete;
};

// 502
struct DiceValueStruct : public BaseStruct
{
	int diceValue;

public:
	DiceValueStruct(const int& InDiceValue) : diceValue(InDiceValue) {};
	virtual ~DiceValueStruct() override = default;
};

// 503
struct TerrainTypeStruct : public BaseStruct
{
	int terrainType;

public:
	TerrainTypeStruct(const int& InTerrainType) : terrainType(InTerrainType) {};
	virtual ~TerrainTypeStruct() override = default;
};

// 504
struct TerrainIndexStruct : public BaseStruct
{
	int changedTerrainCount;
	int	terrainIndex[MAX_TERRAIN_LEN];	//Dice 12
	//std::vector<int>	terrainIndex;
public:
	TerrainIndexStruct(const int& InChangedTerrainCount, int* InTerrainIndex) 
		: changedTerrainCount(InChangedTerrainCount), terrainIndex()
	{
		// ��Ȯ�� �۵��ϴ����� ���� Ȯ�� �ʿ�.
		memcpy(terrainIndex, InTerrainIndex, changedTerrainCount * sizeof(int));
	};
	virtual ~TerrainIndexStruct() override = default;
};

// 505
struct EventCardIndexStruct : public BaseStruct
{
	int eventCardIndex;

public:
	EventCardIndexStruct(const int& InEventCardIndex) : eventCardIndex(InEventCardIndex) {};
	virtual ~EventCardIndexStruct() override = default;
};

#pragma endregion