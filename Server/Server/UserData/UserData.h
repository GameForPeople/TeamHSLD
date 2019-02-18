#pragma once

#include "../PCH/stdafx.h"
#include "../ContentManager/ItemManager.h"
#include "../ContentManager/CharacterManager.h"

struct SocketInfo;

class UserData {
	//	static!
public:
	static const wstring				VIP_CODE;
	static unique_ptr<ItemManager>		itemManager;		//Init in UserData.cpp
	static unique_ptr<CharacterManager>	characterManager;	//Init in UserData.cpp
private:
	SocketInfo*						pSocketInfo;			// ���� ���� ����ü

	Type_ID							id{};					// ���̵� -> Ű ������ ����Ǹ鼭, ���� �ʿ�.
	Type_Nickname					nickname{};				//�г���

	int								winCount;				//�¸� Ƚ��
	int								loseCount;				//�й� Ƚ��
	int								money;					//�ΰ��� ��ȭ

	int								achievementBit;			//���� ��Ʈ
	int								itemBit;				//������ ��Ʈ
	int								characterBit;			//���� ĳ���� ��Ʈ

	vector<std::string>				friendNicknameCont;		// ģ�� �г��� �����̳�
	vector<weak_ptr<UserData>>		friendUserDataCont;		// ģ�� ���� ���� �����̳�

	int								demandFriendContIndex;	// ģ�� �߰� ��, ���� �ε��� ����.
	BYTE							activeCharacterIndex;	// ���� Ȱ��ȭ�� ĳ������ �ε���.

	u_short							udpPortNumber;
	// int	test[5000000]; // UserData���� �޸𸮰� �� �����ǳ�?
public:
	// For rbTreeNode's nullNode or Friend Nullptr!
	UserData() noexcept
		: pSocketInfo(nullptr)
		, id()
		, nickname()
		, winCount()
		, loseCount()
		, money()
		, achievementBit()
		, itemBit()
		, characterBit()
		, friendNicknameCont()
		, friendUserDataCont()
		, demandFriendContIndex(-1)
		, activeCharacterIndex(CharacterManager::CHARACTER_INDEX::MY_PLANET)
		, udpPortNumber(0)
	{};

	// ģ���� ���� ���. friendSocketInfoCont��, �÷��̾ ģ�� ���� UI�� ��û�� ��쿡�� üũ�մϴ�. (����ȭ ��, DB ���ʿ� ������)
	UserData(SocketInfo* pInSocketInfo, const Type_ID& InID, const Type_Nickname& InNickname,
		const int InWinCount, const int InLoseCount, const int InMoney, 
		const int InAchievementBit, const int InItemBit, const int InCharacterBit, const BYTE InActiveCharacterIndex,
		const vector<Type_Nickname>& InFriendStringCont) //, const std::vector<SOCKETINFO*>& InFriendSocketInfoCont)
		: pSocketInfo(pInSocketInfo)
		, id(InID)
		, nickname(InNickname)
		, winCount(InWinCount)
		, loseCount(InLoseCount)
		, money(InMoney)
		, achievementBit(InAchievementBit)
		, itemBit(InItemBit)
		, characterBit(InCharacterBit)
		, friendNicknameCont(InFriendStringCont)
		, friendUserDataCont()
		, demandFriendContIndex(-1)
		, activeCharacterIndex(InActiveCharacterIndex)
		, udpPortNumber(0)
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
		const int InAchievementBit, const int InItemBit, const int InCharacterBit, const BYTE InActiveCharacterIndex)
		: pSocketInfo(pInSocketInfo)
		, id(InID)
		, nickname(InNickname)
		, winCount(InWinCount)
		, loseCount(InLoseCount)
		, money(InMoney)
		, achievementBit(InAchievementBit)
		, itemBit(InItemBit)
		, characterBit(InCharacterBit)
		, friendNicknameCont()
		, friendUserDataCont()
		, demandFriendContIndex(-1)
		, activeCharacterIndex(InActiveCharacterIndex)
		, udpPortNumber(0)
	{};

	//ȸ������ó��
	UserData(SocketInfo* pInSocketInfo, const Type_ID& InID)
		: pSocketInfo(pInSocketInfo)
		, id(InID)
		, nickname()
		, winCount(0)
		, loseCount(0)
		, money(0)
		, achievementBit(0)
		, itemBit(0)
		, characterBit(0)
		, friendNicknameCont()
		, friendUserDataCont()
		, demandFriendContIndex(-1)
		, activeCharacterIndex(CharacterManager::CHARACTER_INDEX::MY_PLANET)
		, udpPortNumber(0)
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
			<< ", itemBit : " << itemBit
			<< ", characterBit : " << characterBit
			<< "\n";
			// friend�� ����ϱ� �ʽ��ϴ�. -> ��� �������ϴ�.
	}

	_NODISCARD __inline constexpr SocketInfo* GetSocketInfo() const noexcept { return pSocketInfo; }
	_NODISCARD __inline Type_ID&	GetID()  /*const*/ noexcept { return id; }
	_NODISCARD __inline Type_Nickname& GetNickname()   /*const*/ noexcept { return nickname; }
	_NODISCARD __inline int	GetWinCount()  const noexcept { return winCount; }
	_NODISCARD __inline int	GetLoseCount()  const noexcept { return loseCount; }
	_NODISCARD __inline int	GetMoney()  const noexcept { return money; }
	_NODISCARD __inline int	GetAchievementBit()  const noexcept { return achievementBit; }
	_NODISCARD __inline int	GetItemBit()  const noexcept { return itemBit; }
	_NODISCARD __inline int	GetCharacterBit()  const noexcept { return characterBit; }
	_NODISCARD __inline vector<Type_Nickname>& GetFriendNicknameCont() /*const*/ noexcept { return friendNicknameCont; }
	_NODISCARD __inline Type_Nickname& GetFriendNicknameWithIndex(const int InIndex ) /*const*/ noexcept { return friendNicknameCont[InIndex]; }
	_NODISCARD __inline weak_ptr<UserData>/*&*/ GetFriendUserDataWithIndex( const int InIndex ) /*const*/ noexcept { return friendUserDataCont[InIndex]; }
	_NODISCARD __inline int	GetFriendNicknameContSize() const noexcept { return friendNicknameCont.size(); }
	_NODISCARD __inline int	GetDemandFriendContIndex() const noexcept { return demandFriendContIndex; }
	_NODISCARD __inline BYTE GetActiveCharacterIndex() const noexcept { return activeCharacterIndex; }
	_NODISCARD __inline u_short	GetUdpPortNumber() const noexcept { return udpPortNumber; }

	// �ִ� �� �ߺ�...?
	//__inline void	SetWinOrLose(const int& value) {
	//	if (value == 1) { winCount++; }
	//	else if (value == 2) { loseCount++; }
	//}
	// True�� ��� �¸�, False�� ��� �й�.
	__inline void SetGameResult(const bool InWinOrLose)	noexcept { if (InWinOrLose) { ++winCount; money += 1000; } else ++loseCount; }
	__inline void SetNickname(const Type_Nickname& InNickname)	noexcept { nickname = InNickname; }
	__inline void SetMoney(const int InMoney) noexcept { money = InMoney; }
	__inline void SetFreindUserDataWithIndex(const shared_ptr<UserData>& InSocketInfo, const int InIndex) noexcept { friendUserDataCont[InIndex] = InSocketInfo; }
	__inline int SetInsertFriendNickname(const Type_Nickname& InFriendNickname) noexcept { 
		if (friendNicknameCont.size() >= 4) return -1;

		friendNicknameCont.emplace_back(InFriendNickname);
		return friendNicknameCont.size() - 1;
	}
	__inline void SetDemandFriendContIndex(const int InIndex) noexcept { demandFriendContIndex = InIndex; }
	__inline void SetDeleteFriendID()
	{
		//warning C26444: Avoid unnamed objects with custom construction and destruction (es.84).
		friendNicknameCont.erase(friendNicknameCont.begin() + demandFriendContIndex);
		demandFriendContIndex = -1;
	}
	__inline void SetActiveCharacterIndex(const BYTE InIndex) noexcept { activeCharacterIndex = InIndex; }
	__inline void SetUdpPortNumber(const u_short InUdpPortNumber) noexcept { udpPortNumber = InUdpPortNumber; }

public:
	_NODISCARD int BuyItem(const int InItemIndex);
	_NODISCARD int BuyCharacter(const int InCharacterIndex);

	_NODISCARD int VipCodeProcess(const wstring& InInputtedString);
};



