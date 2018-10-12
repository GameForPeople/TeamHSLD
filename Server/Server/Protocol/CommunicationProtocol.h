#pragma once

// 주의해야합니다! 항상 클래스는 4바이트 단위로 제작합니다. SIMD 아니고 이거 뭐더라...
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
	PERMIT_GUEST_JOIN	=	315,	// 방에 게스트가 접속했음.
	PERMIT_GUEST_NOT_JOIN = 316,	// 방에 게스트가 접속했음.


	// for RoomScene 구 룸씬
	DEMAND_ROOMHOST		=	400	,
	ROOMSTATE_VOID		=	410	,
	ROOMSTATE_GUESTIN	=	411	,
	
	// 신 Room Protocol
	DEMAND_ENEMY_CHARACTER = 421,   // 내 캐릭터 변경 정보를 전송하고,
	PERMIT_ENEMY_CHARACTER = 422,   // 상대방의 캐릭터 정보를 받아옵니다.

	DISCONNECTED_ENEMY_CLIENT = 498, // 상대편 클라이언트 네트워크 예외 처리 (roomProtocol 및 클라에서 적용)


	// for GameScene
	
	DEMAND_GAME_STATE	=	500 ,	// 디펜스 턴인 친구가, 야 공격턴이 공격햇어??를 여쭤봄
	VOID_GAME_STATE		=	501 ,	// 야 수비야 아직은 공격이 아무것도 안했어!

	NOTIFY_END_OF_TURN	=	502	,	// 야 나 다했다!
	NOTIFY_CHANGE_TURN =	503	,	// 야 재 다했대!
	
	VOID_CLIENT_TO_SERVER = 511 ,				// 공격턴 클라이언트가 시간초과로 아무것도 보내지 않을 때,
	CHANGE_PLANET_CLIENT_TO_SERVER = 512 ,		// 공격턴 클라이언트가 땅을 바꿧을 때,
	ACTION_EVENTCARD_TERRAIN_CLIENT_TO_SERVER = 513,	// 공격턴 클라이언트의 이벤트 카드(공격, 지형변경) 처리
	ACTION_EVENTCARD_DICEBUFF_CLIENT_TO_SERVER = 514,	// 공격턴 클라이언트의 이벤트 카드(공격, 주사위 관련) 처리
	ACTION_EVENTCARD_DEFENSE_CLIENT_TO_SERVER = 515,	// ??턴 클라이언트의 이벤트 카드 쉴드 처리??


	VOID_SERVER_TO_CLIENT = 521 ,				// 서버가 수비턴 클라이언트에게 VOID전송 다만, 이거 받으면 턴 변경임!
	CHANGE_PLANET_SERVER_TO_CLIENT = 522 ,		// 서버가 수비턴 클라이언트에게 공격 클라이언트가 바꾼 행성 정보를 전송
	ACTION_EVENTCARD_TERRAIN_SERVER_TO_CLIENT = 523,	// 서버가 수비턴 클라이언트에게 공격턴 클라이언트 이벤트 카드(공격, 지형변경) 정보를 전송
	ACTION_EVENTCARD_DICEBUFF_SERVER_TO_CLIENT = 524,	// 서버가 수비턴 클라이언트에게 공격턴 클라이언트 이벤트 카드(공격, 주사위 관련) 처리
	ACTION_EVENTCARD_DEFENSE_SERVER_TO_CLIENT = 525,	// ??턴 클라이언트의 이벤트 카드 쉴드 처리??

	// Not yet used
	DEMAND_DICE_CLIENT_TO_SERVER = 521,		// 주사위 숫자를 요구할 때,
	PERMIT_DICE_SERVER_TO_CLIENT = 522 ,

	DEMAND_EVENTCARD_CLIENT_TO_SERVER = 523,	// 공통덱에서 이벤트 카드를 요구할 때,
	PERMIT_EVENTCARD_SERVER_TO_CLIENT = 524 ,

	DEMAND_EVENTCARD_DICE_CLIENT_TO_SERVER = 525 , // 이벤트 카드의 숫자를 요구할 때
	PERMIT_EVENTCARD_DICE_SERVER_TO_CLIENT = 526 

};

enum class SCENE_NAME {
	TITLE_SCENE	,	// 로고 노출 및 연결
	LOGIN_SCENE	,	// 계정 로그인
	MainUI_SCENE,
	LOBBY_SCENE	,	// 이거 쫌 그래 사실 별로야
	ROOM_SCENE	,		// 방으로 연결
	INGAME_SCENE	// 얍얍얍 인게임 얍얍얍
};

enum TERRAIN_TYPE {

};

// 기반 스트럭처
struct BaseStruct {
public:
	BaseStruct() = default;
	~BaseStruct() = default;

public:
	//BaseStruct(const BaseStruct&) = delete; // 메모리에서 강제로 변경하려면 어쩔수없이 주석처리
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

// type 100일때, 서버에 바로 다음 날려주는 구조체
struct DemandLoginStruct : public BaseStruct {
	int type{};	// 1일때는 로그인, 2일때는 회원가입
	int PW{};
	int IDSize{};
	std::string ID;
};

// type 100일때, 서버에 바로 다음 날려주는 구조체
struct DemandLoginCharStruct : public BaseStruct {
	int type{};	// 1일때는 로그인, 2일때는 회원가입
	int PW{};
	int IDSize{};
	char ID[20]{};
	
	DemandLoginCharStruct() { ID[0] = { 0, }; };
	~DemandLoginCharStruct() = default;
};

// type 101 Server -> Client 로그인 실패, 회원가입 실패
struct FailLoginStruct : public BaseStruct {
	int type{}; // 1일때 로그인 없는 아이디, 2일때 로그인 잘못된 비밀번호, 3일때 이미 로그인한 아이디, 4일때 회원가입 중복된 아이디!
	// 아이디, 비밀번호 정합성은, 클라단에서 체크하세요!!

	__inline FailLoginStruct(int InType) : type(InType)
	{};

	__inline FailLoginStruct() = default;
	__inline ~FailLoginStruct() = default;
};

// type 102 Server -> Client 로그인 성공, Lobby정보, 계정정보 전달
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
	//안씁니다. -> ? 함수화하면서 쓰는 걸로 바꼇는데? 히히
	// ? 띠용? 이거 안쓰네 이친구는 리시브구나..헤헿 리시브는 고냥 메모리로 갖고 놀랳
};

// type 302
struct PermitMakeRoomStruct : public BaseStruct {
	// 사실 이것도안쓰지만. -> ? 애는 진짜씀 ㅎ
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
	//안써요~
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

// type 502, 503 // 이거 안씀
struct ChangeTurnStruct : public BaseStruct
{
private:	// 생성 막아놈.
	__inline ChangeTurnStruct() = default;
	__inline ~ChangeTurnStruct() = default;
};

// type 511, 521 ???????? 이거 안쓸껄...?
struct VoidStruct : public BaseStruct
{
private:	// 생성 막아놈.
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
		// 미친놈이세요??
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
		// 이거 진짜 되려나?
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
	//?????????????????????????????????????????????????????????????????? 이거 솔직히 아직 어떻게 처리해야할지 모르겠어...

	__inline ActionEventCardDefenseStruct() {}
	__inline ~ActionEventCardDefenseStruct() {}
};

#pragma endregion