#pragma once

#include "../PCH/stdafx.h"

struct SocketInfo;

class UserData {
	SocketInfo*						pSocketInfo;			// 소켓 정보 구조체

	Type_ID							id{};					// 아이디 -> 키 값으로 변경되면서, 제거 필요.
	Type_Nickname					nickname{};				//닉네임

	int								winCount;				//승리 횟수
	int								loseCount;				//패배 횟수
	int								money;					//인게임 재화

	int								achievementBit;			//업적 비트
	int								titleBit;				//칭호 비트
	int								characterBit;			//보유 캐릭터 비트

	vector<std::string>				friendNicknameCont;		// 친구 닉네임 컨테이너
	vector<weak_ptr<UserData>>		friendUserDataCont;		// 친구 소켓 정보 컨테이너

	int								demandFriendContIndex;	// 친구 추가 시, 사용될 인덱스 버퍼.

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

	// 친구가 있을 경우. friendSocketInfoCont는, 플레이어가 친구 관련 UI를 요청할 경우에만 체크합니다. (최적화 및, DB 미필요 데이터)
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
		// reserver만 하지말고, 메모리 할당 해버려야함.
		// 그 이유는, 클라이언트에서 친구정보를 요청할 떄, 
		// emplace를 하지않고(할당하지 않고), 해당 인덱스로 set하기 때문에 오버플로우 에러가 발생함.

		friendUserDataCont.reserve(InFriendStringCont.size());
		
		for (int i = 0; i < InFriendStringCont.size(); ++i)
			friendUserDataCont.emplace_back();	//dummy -> nullptr넣으면 컴파일 에러남( 생성 불가), 어짜피 거의 무조건 사영될 메모리임....미리쓴다고 생각하자..힘내자...
	};

	//친구가 없을 경우. default로 init함.
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

	//회원가입처리
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
	_NODISCARD __inline Type_ID& GetKey() noexcept { return id; }

public:
	//!1 : 해당 함수는 디버그 용도입니다.
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
			// friend는 출력하기 않습니다. -> 사실 귀찮습니다.
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
	
	// 애는 왜 중복...?
	//__inline void	SetWinOrLose(const int& value) {
	//	if (value == 1) { winCount++; }
	//	else if (value == 2) { loseCount++; }
	//}

	// True일 경우 승리, False일 경우 패배.
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



