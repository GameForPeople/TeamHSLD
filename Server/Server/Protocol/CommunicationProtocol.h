#pragma once

// 주의해야합니다! 항상 클래스는 4바이트 단위로 제작합니다. SIMD 아니고 이거 뭐더라...
#include "../stdafx.h"

enum Protocol {
	END_SEND			=	-1	,
	START_RECV			=	0	,
	DEMAND_LOGIN		=	100	,
	FAIL_LOGIN			=	101	,
	PERMIT_LOGIN		=	102	,
	DEMAND_MAKEROOM		=	301	,
	PERMIT_MAKEROOM		=	302	,
	DEMAND_JOINROOM		=	303	,
	PERMIT_JOINROOM		=	304	,
	FAIL_JOINROOM		=	305	,
	DEMAND_ROOMHOST		=	400	,
	ROOMSTATE_VOID		=	410	,
	ROOMSTATE_GUESTIN	=	411	,
	DEMAND_GAMESTATE	=	500
};

enum class SCENE_NAME {
	TITLE_SCENE	,	// 로고 노출 및 연결
	LOGIN_SCENE	,	// 계정 로그인
	MainUI_SCENE,
	LOBBY_SCENE	,	// 이거 쫌 그래 사실 별로야
	ROOM_SCENE	,		// 방으로 연결
	INGAME_SCENE	// 얍얍얍 인게임 얍얍얍
};

// 기반 스트럭처
struct BaseStruct {

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