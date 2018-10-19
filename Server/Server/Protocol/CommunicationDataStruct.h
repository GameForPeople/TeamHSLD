#pragma once

#include "../stdafx.h"

#include "../Protocol/CommunicationProtocol.h"

#define		MAX_ID_LEN			20
#define		MAX_TERRAIN_LEN		12

// 기반 스트럭처
struct BaseStruct {
public:
	BaseStruct() = default;
	virtual ~BaseStruct() = 0 {};
public:
	//BaseStruct(const BaseStruct&) = delete; // 로그인을 없앴더니, SetData 에서 문제네;;
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

// type 100일때, 서버에 바로 다음 날려주는 구조체
struct DemandLoginStruct : public BaseStruct {
	int type{};	// 1일때는 로그인, 2일때는 회원가입
	int PW{};
	int IDSize{};
	std::string ID;

	//안쓰네.
	DemandLoginStruct() = delete;
	virtual ~DemandLoginStruct() override = default;
};

// type 100일때, 서버에 바로 다음 날려주는 구조체
struct DemandLoginCharStruct : public BaseStruct {
	int type{};	// 1일때는 로그인, 2일때는 회원가입
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

// type 101 Server -> Client 로그인 실패, 회원가입 실패
struct FailLoginStruct : public BaseStruct {
	int type{}; // 1일때 로그인 없는 아이디, 2일때 로그인 잘못된 비밀번호, 3일때 이미 로그인한 아이디, 4일때 회원가입 중복된 아이디!
				// 아이디, 비밀번호 정합성은, 클라단에서 체크하세요!!

	FailLoginStruct(const int InType) : type(InType)
	{};

	//__inline FailLoginStruct() = delete;
	virtual ~FailLoginStruct() override = default;
};

// type 102 Server -> Client 로그인 성공, Lobby정보, 계정정보 전달
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
	//안씁니다. -> ? 함수화하면서 쓰는 걸로 바꼇는데? 히히
	// ? 띠용? 이거 안쓰네 이친구는 리시브구나..헤헿 리시브는 고냥 메모리로 갖고 놀랳
	DemandMakeRoomStruct() = delete;
};

// type 302
struct PermitMakeRoomStruct : public BaseStruct {
	// 사실 이것도안쓰지만. -> ? 애는 진짜씀 ㅎ
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

// 신 LobbyScene

// type 311
struct DemandRandomMatchStruct : public BaseStruct {
	// 이거 안씁니다.
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
	//응~ 안써요~
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


// type 501 // 이거 안씀
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
		// 정확히 작동하는지의 여부 확인 필요.
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