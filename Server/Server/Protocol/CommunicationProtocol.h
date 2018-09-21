#pragma once

// 주의해야합니다! 항상 클래스는 4바이트 단위로 제작합니다. SIMD 아니고 이거 뭐더라...
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
	PERMIT_MAKEROOM		=	302	, // 여기서, 방의 생성자를 호출하는데, 누가 먼저 턴인지, 어느 승리조건인지 결정 ( 방객체 변수로 저장)
	DEMAND_JOINROOM		=	303	,
	PERMIT_JOINROOM		=	304	, // 여기서, 추가된 정보(먼저 턴, 승리조건 등 추가 전송)
	FAIL_JOINROOM		=	305	,
	
	// for RoomScene
	DEMAND_ROOMHOST		=	400	,
	ROOMSTATE_VOID		=	410	,
	ROOMSTATE_GUESTIN	=	411	,
	
	// for GameScene
	
	DEMAND_GAME_STATE	=	500 ,	// 디펜스 턴인 친구가, 야 공격턴이 공격햇어??를 여쭤봄
	VOID_GAME_STATE		=	501 ,	// 야 수비야 공격이 아무것도 안했어!

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

private:
	//BaseStruct(const BaseStruct&); // 메모리에서 강제로 변경하려면 어쩔수없이 주석처리
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

// type 501, 511 ???????? 이거 안쓸꺼야
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
		// 미친놈이세요??
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
		// 이건 무슨 미친짓이더냐
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
	//?????????????????????????????????????????????????????????????????? 이거 솔직히 아직 어떻게 처리해야할지 모르겠어...

	__inline ActionEventCardDefenseStruct() {}
	__inline ~ActionEventCardDefenseStruct() {}
};