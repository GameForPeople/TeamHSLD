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
#include "../stdafx.h"

#define		MAX_ID_LEN			20
#define		MAX_TERRAIN_LEN		12


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

	DISCONNECTED_ENEMY_CLIENT = 498, // 상대편 클라이언트 네트워크 예외에 대한 처리 요청 (roomProtocol 및 클라에서 적용) - (Server to client)
	DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT = 499, // 상대편 클라이언트가 나갔음을 인지하고, 해당 처리를 요청함. (Client to Server)

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
	//BaseStruct(const BaseStruct&) = delete; // 로그인을 없앴더니, SetData 에서 문제네;;
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

	//안쓰네.
	DemandLoginStruct() = delete;
	~DemandLoginStruct() = default;
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
	~DemandLoginCharStruct() = default;
};

// type 101 Server -> Client 로그인 실패, 회원가입 실패
struct FailLoginStruct : public BaseStruct {
	int type{}; // 1일때 로그인 없는 아이디, 2일때 로그인 잘못된 비밀번호, 3일때 이미 로그인한 아이디, 4일때 회원가입 중복된 아이디!
	// 아이디, 비밀번호 정합성은, 클라단에서 체크하세요!!

	__inline FailLoginStruct(const int InType) : type(InType)
	{};

	//__inline FailLoginStruct() = delete;
	__inline ~FailLoginStruct() = default;
};

// type 102 Server -> Client 로그인 성공, Lobby정보, 계정정보 전달
struct PermitLoginStruct : public BaseStruct {
	int winCount{};
	int loseCount{}; 
	int money{};

	__inline PermitLoginStruct(const int InWin,const int InLose, const int InMoney) 
		: winCount(InWin) , loseCount(InLose) , money(InMoney)
	{};

	//__inline PermitLoginStruct() = delete;
	__inline ~PermitLoginStruct() = default;
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

	__inline PermitMakeRoomStruct(const int InRoomIndex) : roomIndex(InRoomIndex)
	{};

	//__inline PermitMakeRoomStruct() = delete;
	__inline ~PermitMakeRoomStruct() = default;
};

//type 303
struct DemandJoinRoomStruct : public BaseStruct {
	int roomIndex;

	__inline DemandJoinRoomStruct(const int InRoomIndex) : roomIndex(InRoomIndex)
	{};

	//__inline DemandJoinRoomStruct() = delete;
	__inline ~DemandJoinRoomStruct() = default;
};

//type 304
struct PermitJoinRoomStruct : public BaseStruct {
	int roomIndex;
	int idSize;
	string enemyId;

	__inline PermitJoinRoomStruct(int InRoomIndex, const string_view& InEnemyID)
		: roomIndex(InRoomIndex), idSize(InEnemyID.size()), enemyId(InEnemyID)
	{};

	//__inline PermitJoinRoomStruct() = delete;
	__inline ~PermitJoinRoomStruct() = default;
};

//type 305
struct FailJoinRoomStruct : public BaseStruct {
	int failReason;

	__inline FailJoinRoomStruct(int InFailReason) 
		: failReason(InFailReason)
	{};

	//__inline FailJoinRoomStruct() = delete;
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
	char enemyId[MAX_ID_LEN];

	PermitJoinRandomStruct(const int InRoomIndex, const int InIsHostFirst, const int InPlayerMissionIndex, const int InEnemyMissionIndex, const int InSubMissionIndex, const string& InEnemyId )
		: roomIndex(InRoomIndex), isHostFirst(InIsHostFirst), playerMissionIndex(InPlayerMissionIndex), enemyMissionIndex(InEnemyMissionIndex), subMissionIndex(InSubMissionIndex), idSize(InEnemyId.size()) //, enemyId(InEnemyID)
	{
		//for (int i = 0; i < idSize; ++i)
		//{
		//	enemyId[i] = InEnemyId[i];
		//}
		memcpy(enemyId, InEnemyId.c_str(), idSize);
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
	char enemyId[MAX_ID_LEN];

	PermitGuestJoinStruct(const string& InEnemyId) :idSize(InEnemyId.size()) //, enemyId(InEnemyId)
	{
		//for (int i = 0; i < idSize; ++i)
		//{
		//	enemyId[i] = InEnemyId[i];
		//}
		memcpy(enemyId, InEnemyId.c_str(), idSize);
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

	__inline RoomStateGuestInStruct(const string_view& InEnemyID)
		: idSize(InEnemyID.size()), enemyId(InEnemyID)
	{};

	//__inline RoomStateGuestInStruct() = delete;
	__inline ~RoomStateGuestInStruct() = default;
};

// type 401 -> One Player Changed //
struct OnePlayerChanged : public BaseStruct
{
	int index{};
	int mixedData{};

	__inline OnePlayerChanged(const int InIndex, const int InLeftOrRight, const int isJumping ) :index(InIndex)
	{
		mixedData = InLeftOrRight * 10 + isJumping;
	};
	//__inline OnePlayerChanged() = delete;
	__inline OnePlayerChanged() = default;
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

	~PermitEnemyCharacterStruct() = default;
};


#pragma endregion


#pragma region [INGAME SCENE STRUCT]

// type 502, 503 // 이거 안씀
struct ChangeTurnStruct : public BaseStruct
{
	__inline ChangeTurnStruct() = delete;
	__inline ~ChangeTurnStruct() = delete;
};

// type 511, 521 ???????? 이거 안쓸껄...?
struct VoidStruct : public BaseStruct
{
	__inline VoidStruct() = delete;
	__inline ~VoidStruct() = delete;
};


// type 512, 522
struct ChangePlanetStruct : public BaseStruct
{
	int					terrainType;
	int					changeTerrainCount;
	int					terrainIndex[MAX_TERRAIN_LEN];	//Dice 12
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
	int					terrainIndex[MAX_TERRAIN_LEN];	//Dice 12

public:
	__inline ActionEventCardTerrainStruct(const int InEventCardType, const int InTerrainType, const int InChangeTerrainCount, const int* InTerrainIndex)
		: eventCardType(InEventCardType), terrainType(InTerrainType), changeTerrainCount(InChangeTerrainCount)
	{
		// 되나?
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