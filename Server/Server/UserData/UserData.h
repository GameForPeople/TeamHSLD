#pragma once

#include "../PCH/stdafx.h"

struct SocketInfo;

class UserData {
	SocketInfo*						pSocketInfo;			// ���� ���� ����ü

	std::string						id{};					// ���̵� -> Ű ������ ����Ǹ鼭, ���� �ʿ�.
	std::wstring					nickName{};				//�г���

	int								winCount;				//�¸� Ƚ��
	int								loseCount;				//�й� Ƚ��
	int								money;					//�ΰ��� ��ȭ

	int								achievementBit;			//���� ��Ʈ
	int								titleBit;				//Īȣ ��Ʈ
	int								characterBit;			//���� ĳ���� ��Ʈ

	vector<std::wstring>			friendNicknameCont;		// ģ�� �г��� �����̳�
	vector<weak_ptr<UserData>>		friendUserDataCont;		// ģ�� ���� ���� �����̳�
	int								demandFriendContIndex;	// ģ�� �߰� ��, �߰��ϴ� ģ���� �޴� ģ������

public:
	// For rbTreeNode's nullNode
	UserData() 
		:	pSocketInfo(nullptr)
		,	id()
		,	nickName()
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
	UserData(SocketInfo* pInSocketInfo, const std::string& InID, const std::wstring& InNickName,
		const int& InWinCount, const int& InLoseCount, const int& InMoney, 
		const int& InAchievementBit, const int& InTitleBit, const int& InCharacterBit, 
		const vector<wstring>& InFriendStringCont) //, const std::vector<SOCKETINFO*>& InFriendSocketInfoCont)
		: 	pSocketInfo(pInSocketInfo)
		,	id(InID)
		,	nickName(InNickName) 
		,	winCount(InWinCount) 
		,	loseCount(InLoseCount) 
		,	money(InMoney)
		,	achievementBit(InAchievementBit)
		,	titleBit(InTitleBit) 
		,	characterBit(InCharacterBit)
		,	friendNicknameCont(InFriendStringCont)
		,	friendUserDataCont()
		,	demandFriendContIndex(-1)
	{};

	//ģ���� ���� ���. default�� init��.
	UserData(SocketInfo* pInSocketInfo, const std::string& InID, const std::wstring& InNickName,
		const int& InWinCount, const int& InLoseCount, const int& InMoney,
		const int& InAchievementBit, const int& InTitleBit, const int& InCharacterBit)
		:	pSocketInfo(pInSocketInfo)
		,	id(InID)
		,	nickName(InNickName)
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
	UserData(SocketInfo* pInSocketInfo, const std::string& InID)
		:	pSocketInfo(pInSocketInfo)
		,	id(InID)
		,	nickName()
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
	_NODISCARD __inline string&	GetKey() { return id; }

public:
	//!1 : �ش� �Լ��� ����� �뵵�Դϴ�.
	_NORETURN __inline void	PrintUserData() const noexcept
	{
		std::cout
			<< "id : " << id
			<< ", nickName : ";
		std::wcout 
			<< nickName;
		std::cout
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
	_NODISCARD __inline string&	GetID()  /*const*/ noexcept { return id; }
	_NODISCARD __inline wstring& GetNickName()   /*const*/ noexcept { return nickName; }
	_NODISCARD __inline int	GetWinCount()  const noexcept { return winCount; }
	_NODISCARD __inline int	GetLoseCount()  const noexcept { return loseCount; }
	_NODISCARD __inline int	GetMoney()  const noexcept { return money; }
	_NODISCARD __inline int	GetAchievementBit()  const noexcept { return achievementBit; }
	_NODISCARD __inline int	GetTitleBit()  const noexcept { return titleBit; }
	_NODISCARD __inline int	GetCharacterBit()  const noexcept { return characterBit; }
	_NODISCARD __inline vector<wstring>& GetFriendNicknameCont() /*const*/ noexcept { return friendNicknameCont; }
	_NODISCARD __inline wstring& GetFriendNicknameWithIndex(const int& InIndex ) /*const*/ noexcept { return friendNicknameCont[InIndex]; }
	_NODISCARD __inline weak_ptr<UserData>& GetFriendUserDataCont( const int& InIndex ) /*const*/ noexcept { return friendUserDataCont[InIndex]; }
	_NODISCARD __inline int	GetFriendNicknameContSize() const noexcept { return friendNicknameCont.size(); }
	_NODISCARD __inline int	GetDemandFriendContIndex() const noexcept { return demandFriendContIndex; }
	// �ִ� �� �ߺ�...?
	//__inline void	SetWinOrLose(const int& value) {
	//	if (value == 1) { winCount++; }
	//	else if (value == 2) { loseCount++; }
	//}

	// True�� ��� �¸�, False�� ��� �й�.
	_NORETURN __inline void	SetGameResult(const bool& InWinOrLose)	noexcept { if (InWinOrLose) ++winCount; 	else ++loseCount; }
	_NORETURN __inline void   SetNickName(const wstring& InNickName)	noexcept { nickName = InNickName; }
	_NORETURN __inline void	SetMoney(const int& InMoney) noexcept { money = InMoney; }
	_NORETURN __inline void   SetFreindUserDataWithIndex(const shared_ptr<UserData>& InSocketInfo, const int& InIndex) noexcept { friendUserDataCont[InIndex] = InSocketInfo; }
	
	__inline int	SetInsertFriendID(const wstring& InFriendID) noexcept { 
		if (friendNicknameCont.size() >= 4) return -1;

		friendNicknameCont.emplace_back(InFriendID); 
		return friendNicknameCont.size() - 1;
	}

	_NORETURN __inline void	SetDeleteFriendID() noexcept
	{
		friendNicknameCont.erase(friendNicknameCont.begin() + demandFriendContIndex);
		demandFriendContIndex = -1;
	}
	_NORETURN __inline void	SetDemandFriendContIndex(const int& InIndex) noexcept { demandFriendContIndex = InIndex; }
};



