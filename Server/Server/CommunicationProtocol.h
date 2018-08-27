#pragma once

// 주의해야합니다! 항상 클래스는 4바이트 단위로 제작합니다. SIMD 아니고 이거 뭐더라...
#include "stdafx.h"


enum Protocol {
	END_SEND = -1
	, START_RECV = 0
	, DEMAND_LOGIN = 100
	, FAIL_LOGIN = 101
	, PERMIT_LOGIN = 102
	, DEMAND_GAMESTATE = 400
};

enum class SCENE_NAME {
	TITLE_SCENE,	// 로고 노출 및 연결
	LOGIN_SCENE,	// 계정 로그인
	LOBBY_SCENE,	// 이거 쫌 그래 사실 별로야
	ROOM_SCENE,		// 방으로 연결
	INGAME_SCENE	// 얍얍얍 인게임 얍얍얍
};

// 기반 스트럭처
struct BaseStruct {

};

struct BaseSendStruct : public BaseStruct {
	int sendType{};
	BaseStruct* dataBuffer;

public:
	__inline BaseSendStruct(const int InSendType, BaseStruct* InStruct) : sendType(InSendType), dataBuffer(InStruct)
	{};
	
	__inline BaseSendStruct() = default;
	
	__inline ~BaseSendStruct()
	{
		if (dataBuffer != nullptr) delete dataBuffer;
	}
};

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

// type 401 -> One Player Changed // 현재 테스트 기능.
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