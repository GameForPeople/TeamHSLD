#pragma once

#include "../PCH/stdafx.h"

struct SocketInfo;

class UserData {
	SocketInfo*						pSocketInfo;			// 소켓 정보 구조체

	std::string						id{};					// 아이디 -> 키 값으로 변경되면서, 제거 필요.
	std::wstring					nickName{};				//닉네임

	int								winCount;				//승리 횟수
	int								loseCount;				//패배 횟수
	int								money;					//인게임 재화

	int								achievementBit;			//업적 비트
	int								titleBit;				//칭호 비트
	int								characterBit;			//보유 캐릭터 비트

	vector<std::wstring>			friendNicknameCont;		// 친구 닉네임 컨테이너
	vector<weak_ptr<UserData>>		friendUserDataCont;		// 친구 소켓 정보 컨테이너
	int								demandFriendContIndex;	// 친구 추가 시, 추가하는 친구가 받는 친구에게

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

	// 친구가 있을 경우. friendSocketInfoCont는, 플레이어가 친구 관련 UI를 요청할 경우에만 체크합니다. (최적화 및, DB 미필요 데이터)
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

	//친구가 없을 경우. default로 init함.
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

	//회원가입처리
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

	//디져랏!
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
	//!1 : 해당 함수는 디버그 용도입니다.
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
			// friend는 출력하기 않습니다. -> 사실 귀찮습니다.
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
	// 애는 왜 중복...?
	//__inline void	SetWinOrLose(const int& value) {
	//	if (value == 1) { winCount++; }
	//	else if (value == 2) { loseCount++; }
	//}

	// True일 경우 승리, False일 경우 패배.
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



