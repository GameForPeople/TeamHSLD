#pragma once

#include "../PCH/stdafx.h"

struct SocketInfo;

class UserData {
	SocketInfo*						pSocketInfo;			// ���� ���� ����ü

	Type_ID							id{};					// ���̵� -> Ű ������ ����Ǹ鼭, ���� �ʿ�.
	Type_Nickname					nickname{};				//�г���

	int								winCount;				//�¸� Ƚ��
	int								loseCount;				//�й� Ƚ��
	int								money;					//�ΰ��� ��ȭ

	int								achievementBit;			//���� ��Ʈ
	int								titleBit;				//Īȣ ��Ʈ
	int								characterBit;			//���� ĳ���� ��Ʈ

	vector<std::string>				friendNicknameCont;		// ģ�� �г��� �����̳�
	vector<weak_ptr<UserData>>		friendUserDataCont;		// ģ�� ���� ���� �����̳�

	int								demandFriendContIndex;	// ģ�� �߰� ��, ���� �ε��� ����.

public:
	// For rbTreeNode's nullNode or Friend Nullptr!
	UserData() noexcept
		:	pSocketInfo(nullptr)
		,	id()
		,	nickname()
		,	winCount()
		,	loseCount()
		,	money()
		,	achievementBit()
		,	titleBit()
		,	characterBit()
		,	friendNicknameCont()
		,	friendUserDataCont()
		,	demandFriendContIndex(-1)
	{};

	// ģ���� ���� ���. friendSocketInfoCont��, �÷��̾ ģ�� ���� UI�� ��û�� ��쿡�� üũ�մϴ�. (����ȭ ��, DB ���ʿ� ������)
	UserData(SocketInfo* pInSocketInfo, const Type_ID& InID, const Type_Nickname& InNickname,
		const int InWinCount, const int InLoseCount, const int InMoney, 
		const int InAchievementBit, const int InTitleBit, const int InCharacterBit, 
		const vector<Type_Nickname>& InFriendStringCont) //, const std::vector<SOCKETINFO*>& InFriendSocketInfoCont)
		: 	pSocketInfo(pInSocketInfo)
		,	id(InID)
		,	nickname(InNickname) 
		,	winCount(InWinCount) 
		,	loseCount(InLoseCount) 
		,	money(InMoney)
		,	achievementBit(InAchievementBit)
		,	titleBit(InTitleBit) 
		,	characterBit(InCharacterBit)
		,	friendNicknameCont(InFriendStringCont)
		,	friendUserDataCont()
		,	demandFriendContIndex(-1)
	{
		//friendUserDataCont.reserve(InFriendStringCont.size()); 
		// reserver�� ��������, �޸� �Ҵ� �ع�������.
		// �� ������, Ŭ���̾�Ʈ���� ģ�������� ��û�� ��, 
		// emplace�� �����ʰ�(�Ҵ����� �ʰ�), �ش� �ε����� set�ϱ� ������ �����÷ο� ������ �߻���.

		friendUserDataCont.reserve(InFriendStringCont.size());
		
		for (int i = 0; i < InFriendStringCont.size(); ++i)
			friendUserDataCont.emplace_back();	//dummy -> nullptr������ ������ ������( ���� �Ұ�), ��¥�� ���� ������ �翵�� �޸���....�̸����ٰ� ��������..������...
	};

	//ģ���� ���� ���. default�� init��.
	UserData(SocketInfo* pInSocketInfo, const Type_ID& InID, const Type_Nickname& InNickname,
		const int InWinCount, const int InLoseCount, const int InMoney,
		const int InAchievementBit, const int InTitleBit, const int InCharacterBit)
		:	pSocketInfo(pInSocketInfo)
		,	id(InID)
		,	nickname(InNickname)
		,	winCount(InWinCount)
		,	loseCount(InLoseCount)
		,	money(InMoney)
		,	achievementBit(InAchievementBit)
		,	titleBit(InTitleBit)
		,	characterBit(InCharacterBit)
		,	friendNicknameCont()
		,	friendUserDataCont()
		,	demandFriendContIndex(-1)
	{};

	//ȸ������ó��
	UserData(SocketInfo* pInSocketInfo, const Type_ID& InID)
		:	pSocketInfo(pInSocketInfo)
		,	id(InID)
		,	nickname()
		,	winCount(0)
		,	loseCount(0)
		,	money(0)
		,	achievementBit(0)
		,	titleBit(0)
		,	characterBit(0)
		,	friendNicknameCont()
		,	friendUserDataCont()
		,	demandFriendContIndex(-1)
	{};

	//������!
	~UserData() 
	{
		pSocketInfo = nullptr;

		for (auto iter = friendUserDataCont.begin(); iter != friendUserDataCont.end(); ++iter)
		{
			iter->reset();
		}
	};

public:
	//need TEMPLATE InterFace
	_NODISCARD __inline Type_ID& GetKey() noexcept { return id; }

public:
	//!1 : �ش� �Լ��� ����� �뵵�Դϴ�.
	_DEPRECATED __inline void	PrintUserData() const noexcept
	{
		std::cout
			<< "id : " << id
			<< ", Nickname : " << nickname
			<< ", winCount : " << winCount
			<< ", loseCount : " << loseCount
			<< ", money : " << money
			<< ", achievementBit : " << achievementBit
			<< ", titleBit : " << titleBit
			<< ", characterBit : " << characterBit
			<< "\n";
			// friend�� ����ϱ� �ʽ��ϴ�. -> ��� �������ϴ�.
	}

	_NODISCARD __inline SocketInfo* GetSocketInfo() const noexcept { return pSocketInfo; }
	_NODISCARD __inline Type_ID&	GetID()  /*const*/ noexcept { return id; }
	_NODISCARD __inline Type_Nickname& GetNickname()   /*const*/ noexcept { return nickname; }
	_NODISCARD __inline int	GetWinCount()  const noexcept { return winCount; }
	_NODISCARD __inline int	GetLoseCount()  const noexcept { return loseCount; }
	_NODISCARD __inline int	GetMoney()  const noexcept { return money; }
	_NODISCARD __inline int	GetAchievementBit()  const noexcept { return achievementBit; }
	_NODISCARD __inline int	GetTitleBit()  const noexcept { return titleBit; }
	_NODISCARD __inline int	GetCharacterBit()  const noexcept { return characterBit; }
	_NODISCARD __inline vector<Type_Nickname>& GetFriendNicknameCont() /*const*/ noexcept { return friendNicknameCont; }
	_NODISCARD __inline Type_Nickname& GetFriendNicknameWithIndex(const int& InIndex ) /*const*/ noexcept { return friendNicknameCont[InIndex]; }
	_NODISCARD __inline weak_ptr<UserData>/*&*/ GetFriendUserDataWithIndex( const int& InIndex ) /*const*/ noexcept { return friendUserDataCont[InIndex]; }
	_NODISCARD __inline int	GetFriendNicknameContSize() const noexcept { return friendNicknameCont.size(); }
	_NODISCARD __inline int	GetDemandFriendContIndex() const noexcept { return demandFriendContIndex; }
	
	// �ִ� �� �ߺ�...?
	//__inline void	SetWinOrLose(const int& value) {
	//	if (value == 1) { winCount++; }
	//	else if (value == 2) { loseCount++; }
	//}

	// True�� ��� �¸�, False�� ��� �й�.
	__inline void	SetGameResult(const bool InWinOrLose)	noexcept { if (InWinOrLose) ++winCount; 	else ++loseCount; }
	__inline void   SetNickname(const Type_Nickname& InNickname)	noexcept { nickname = InNickname; }
	__inline void	SetMoney(const int InMoney) noexcept { money = InMoney; }
	__inline void   SetFreindUserDataWithIndex(const shared_ptr<UserData>& InSocketInfo, const int InIndex) noexcept { friendUserDataCont[InIndex] = InSocketInfo; }
	
	__inline int	SetInsertFriendNickname(const Type_Nickname& InFriendNickname) noexcept { 
		if (friendNicknameCont.size() >= 4) return -1;

		friendNicknameCont.emplace_back(InFriendNickname);
		return friendNicknameCont.size() - 1;
	}
	
	__inline void	SetDemandFriendContIndex(const int InIndex) noexcept { demandFriendContIndex = InIndex; }

	__inline void	SetDeleteFriendID()
	{
		//warning C26444: Avoid unnamed objects with custom construction and destruction (es.84).
		friendNicknameCont.erase(friendNicknameCont.begin() + demandFriendContIndex);
		demandFriendContIndex = -1;
	}
};



