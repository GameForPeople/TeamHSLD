#pragma once

// �����ؾ��մϴ�! �׻� Ŭ������ 4����Ʈ ������ �����մϴ�. SIMD �ƴϰ� �̰� ������...
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
	TITLE_SCENE,	// �ΰ� ���� �� ����
	LOGIN_SCENE,	// ���� �α���
	LOBBY_SCENE,	// �̰� �� �׷� ��� ���ξ�
	ROOM_SCENE,		// ������ ����
	INGAME_SCENE	// ���� �ΰ��� ����
};

// ��� ��Ʈ��ó
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

// type 401 -> One Player Changed // ���� �׽�Ʈ ���.
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