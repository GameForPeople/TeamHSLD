#pragma once

// �����ؾ��մϴ�! �׻� Ŭ������ 4����Ʈ ������ �����մϴ�. SIMD �ƴϰ� �̰� ������...
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
	TITLE_SCENE	,	// �ΰ� ���� �� ����
	LOGIN_SCENE	,	// ���� �α���
	MainUI_SCENE,
	LOBBY_SCENE	,	// �̰� �� �׷� ��� ���ξ�
	ROOM_SCENE	,		// ������ ����
	INGAME_SCENE	// ���� �ΰ��� ����
};

// ��� ��Ʈ��ó
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