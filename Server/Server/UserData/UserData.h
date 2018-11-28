#pragma once

#include "../PCH/stdafx.h"

struct SocketInfo;

class UserData {
	SocketInfo*				pSocketInfo;			// 소켓 정보 구조체

	std::string				id{};					// 아이디 -> 키 값으로 변경되면서, 제거 필요.
	std::string				nickName{};				//닉네임

	int						winCount;				//승리 횟수
	int						loseCount;				//패배 횟수
	int						money;					//인게임 재화

	int						achievementBit;			//업적 비트
	int						titleBit;				//칭호 비트
	int						characterBit;			//보유 캐릭터 비트

	vector<std::string>		friendStringCont;		// 친구 이름 컨테이너
	vector<SocketInfo*>		friendSocketInfoCont;	// 친구 소켓 정보 컨테이너 <- nullptr 에러 문제에 의해 안쓰는 걸로 변환.
	int						demandFriendContIndex;	// 친구 추가 시, 추가하는 친구가 받는 친구에게

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

	// 친구가 있을 경우. friendSocketInfoCont는, 플레이어가 친구 관련 UI를 요청할 경우에만 체크합니다. (최적화 및, DB 미필요 데이터)
	UserData(SocketInfo* pInSocketInfo, const std::string& InID, const std::string& InNickName,
		const int& InWinCount, const int& InLoseCount, const int& InMoney, 
		const int& InAchievementBit, const int& InTitleBit, const int& InCharacterBit, 
		const vector<string>& InFriendStringCont) //, const std::vector<SOCKETINFO*>& InFriendSocketInfoCont)
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

	//친구가 없을 경우. default로 init함.
	UserData(SocketInfo* pInSocketInfo, const std::string& InID, const std::string& InNickName,
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
		,	friendStringCont()
		,	friendSocketInfoCont()
		,	demandFriendContIndex(-1)
	{};

	//디져랏!
	~UserData() 
	{
		pSocketInfo = nullptr;

		for (auto iter = friendSocketInfoCont.begin(); iter != friendSocketInfoCont.end(); ++iter)
		{
			*iter = nullptr;
		}
	};

public:
	//주의 : 해당 함수는 디버그 용도로만 사용하세요.
	__inline void	PrintUserData() const
	{
		std::cout 
			<< "id : " << id
			<< ", nickName : " << nickName
			<< ", winCount : " << winCount
			<< ", loseCount : " << loseCount
			<< ", money : " << money
			<< ", achievementBit : " << achievementBit
			<< ", titleBit : " << titleBit
			<< ", characterBit : " << characterBit
			// friend는 출력하기 어렵습 -> 귀찮습니다.
			<< "\n";
	}

	__inline SocketInfo* GetSocketInfo() const { return pSocketInfo; }
	__inline string	GetID()  const { return id; }
	__inline string	GetNickName()   const { return nickName; }
	__inline int	GetWinCount()  const { return winCount; }
	__inline int	GetLoseCount()  const { return loseCount; }
	__inline int	GetMoney()  const { return money; }
	__inline int	GetAchievementBit()  const { return achievementBit; }
	__inline int	GetTitleBit()  const { return titleBit; }
	__inline int	GetCharacterBit()  const { return characterBit; }
	__inline vector<string> GetFriendStringCont() const { return friendStringCont; }
	__inline string GetFriendStringWithIndex(const int& InIndex ) const { return friendStringCont[InIndex]; }
	__inline SocketInfo* GetFriendSocketInfo( const int& InIndex ) const { return friendSocketInfoCont[InIndex]; }
	__inline int	GetFriendStringContSize() const { return friendStringCont.size(); }
	__inline int	GetDemandFriendContIndex() const { return demandFriendContIndex; }
	// 애는 왜 중복...?
	//__inline void	SetWinOrLose(const int& value) {
	//	if (value == 1) { winCount++; }
	//	else if (value == 2) { loseCount++; }
	//}

	// True일 경우 승리, False일 경우 패배.
	__inline void	SetGameResult(const bool& InWinOrLose)	{ if (InWinOrLose) ++winCount; 	else ++loseCount; }
	__inline void   SetNickName(const string& InNickName)	{	nickName = InNickName; }
	__inline void	SetMoney(const int& InMoney)			{ money = InMoney; }
	__inline void   SetFreindSocketInfo(SocketInfo* InSocketInfo, const int& InIndex) { friendSocketInfoCont[InIndex] = InSocketInfo; }
	__inline int	SetInsertFriendID(const string& InFriendID) { 
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



