#pragma once

#include "../PCH/stdafx.h"

struct SocketInfo;

class UserData {
	SocketInfo*				pSocketInfo;			// ���� ���� ����ü

	std::string				id{};					// ���̵� -> Ű ������ ����Ǹ鼭, ���� �ʿ�.
	std::wstring			nickName{};				//�г���

	int						winCount;				//�¸� Ƚ��
	int						loseCount;				//�й� Ƚ��
	int						money;					//�ΰ��� ��ȭ

	int						achievementBit;			//���� ��Ʈ
	int						titleBit;				//Īȣ ��Ʈ
	int						characterBit;			//���� ĳ���� ��Ʈ

	vector<std::wstring>	friendStringCont;		// ģ�� �̸� �����̳�
	vector<SocketInfo*>		friendSocketInfoCont;	// ģ�� ���� ���� �����̳� <- nullptr ���� ������ ���� �Ⱦ��� �ɷ� ��ȯ.
	int						demandFriendContIndex;	// ģ�� �߰� ��, �߰��ϴ� ģ���� �޴� ģ������

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
		,	friendStringCont()
		,	friendSocketInfoCont()
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
		,	friendStringCont(InFriendStringCont)
		,	friendSocketInfoCont()
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
		,	friendStringCont()
		,	friendSocketInfoCont()
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
		,	friendStringCont()
		,	friendSocketInfoCont()
		,	demandFriendContIndex(-1)
	{};

	//������!
	~UserData() 
	{
		pSocketInfo = nullptr;

		for (auto iter = friendSocketInfoCont.begin(); iter != friendSocketInfoCont.end(); ++iter)
		{
			*iter = nullptr;
		}
	};

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

	_NODISCARD __inline SocketInfo* GetSocketInfo() const { return pSocketInfo; }
	_NODISCARD __inline string	GetID()  const { return id; }
	_NODISCARD __inline wstring GetNickName()   const { return nickName; }
	_NODISCARD __inline int	GetWinCount()  const { return winCount; }
	_NODISCARD __inline int	GetLoseCount()  const { return loseCount; }
	_NODISCARD __inline int	GetMoney()  const { return money; }
	_NODISCARD __inline int	GetAchievementBit()  const { return achievementBit; }
	_NODISCARD __inline int	GetTitleBit()  const { return titleBit; }
	_NODISCARD __inline int	GetCharacterBit()  const { return characterBit; }
	_NODISCARD __inline vector<wstring> GetFriendStringCont() const { return friendStringCont; }
	_NODISCARD __inline wstring GetFriendStringWithIndex(const int& InIndex ) const { return friendStringCont[InIndex]; }
	_NODISCARD __inline SocketInfo* GetFriendSocketInfo( const int& InIndex ) const { return friendSocketInfoCont[InIndex]; }
	_NODISCARD __inline int	GetFriendStringContSize() const { return friendStringCont.size(); }
	_NODISCARD __inline int	GetDemandFriendContIndex() const { return demandFriendContIndex; }
	// �ִ� �� �ߺ�...?
	//__inline void	SetWinOrLose(const int& value) {
	//	if (value == 1) { winCount++; }
	//	else if (value == 2) { loseCount++; }
	//}

	// True�� ��� �¸�, False�� ��� �й�.
	__inline void	SetGameResult(const bool& InWinOrLose)	{ if (InWinOrLose) ++winCount; 	else ++loseCount; }
	__inline void   SetNickName(const wstring& InNickName)	{	nickName = InNickName; }
	__inline void	SetMoney(const int& InMoney)			{ money = InMoney; }
	__inline void   SetFreindSocketInfo(SocketInfo* InSocketInfo, const int& InIndex) { friendSocketInfoCont[InIndex] = InSocketInfo; }
	
	__inline int	SetInsertFriendID(const wstring& InFriendID) { 
		if (friendStringCont.size() >= 4) return -1;

		friendStringCont.emplace_back(InFriendID); 
		return friendStringCont.size() - 1;
	}

	__inline void	SetDeleteFriendID()
	{
		friendStringCont.erase(friendStringCont.begin() + demandFriendContIndex);
		demandFriendContIndex = -1;
	}
	__inline void	SetDemandFriendContIndex(const int& InIndex) { demandFriendContIndex = InIndex; }
};



