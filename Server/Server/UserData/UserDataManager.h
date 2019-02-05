#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"
#include "../UserData/NicknameNode.h"

#include "../Custom_DataStructure/Custom_Map_RedBlackTree.h"
#include "../Custom_DataStructure/Custom_SET_RedBlackTree.h"

//
struct SocketInfo;
class UDPManager;

//using user_iter = map<const std::string, UserData>::iterator; // iter에서 node Pointer로 변경.

class UserDataManager {

	enum class LANGUAGE // : int
	{
			ENGLISH =	0
		,	KOREAN	=	1
		,	OTHERS	=	2
	};

	atomic<int>			userNum;

	string				nicknameContFileName;

	std::vector<CUSTOM_SET::rbTree<shared_ptr<UserData>, Type_ID>>	userDataCont;
	std::vector<CUSTOM_MAP::rbTree<Type_Nickname, NicknameNode>>	nicknameCont;

	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>			converter; //DEV_66 닉네임 멀티바이트 <-> 유니코드 변환을 위해 사용.
	shared_ptr<UserData> CONST_NULL_USERDATA{nullptr};

	CRITICAL_SECTION	csNicknameCont;
	CRITICAL_SECTION	csSaveNickname;

	std::vector<std::pair<Type_ID, Type_Nickname>> saveFileCont;

public:
	UserDataManager() noexcept; // = delete;
	~UserDataManager(); // = delete;
	
	_NODISCARD _inline int GetUserNum() const noexcept { return userNum; }

public:
	// 로그인 및 회원가입 모두 여기서 처리합니다.
	int LoginProcess(SocketInfo* pInSocketInfo, const Type_ID& InID, Type_Nickname& RetNickname, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, BYTE& RetActiveCharacterIndex, vector<Type_Nickname>& RetFriendNicknameCont);

	void LogoutProcess(shared_ptr<UserData> pUserNode);

	_NODISCARD shared_ptr<UserData> SearchUserNode(const Type_ID& keyID, bool& RetIsOnLogin);	// 아이디를 통해, 유저 노드를 탐색합니다.

	_NODISCARD shared_ptr<UserData> SearchUserNodeByNickname(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch);	// 닉네임을 통해, 유저노드를 탐색합니다.

	_NODISCARD shared_ptr<UserData> SearchUserNodeByNicknameWithActiveCharacterIndex(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch, BYTE& RetActiveCharacterIndex);	// 닉네임을 통해, 유저노드를 탐색합니다.
	
	_NODISCARD bool SetNewNickname(SocketInfo* pInClient, const Type_Nickname& InNewString);

public:
	void TraversalForAnnouncement(UDPManager* const pInUDPManager);

public:
	//DEV_78
	_NODISCARD std::pair<char, bool> GetActiveCharacterIndexWithNickname(Type_Nickname& InNickname);
	void SetActiveCharacterIndexWithNickname(Type_Nickname& KeyNickname, char InNewIndex);

private:
	// for userDataCont Hash, 첫글자처리필요.
	_NODISCARD int GetStringFirstChar(const char InStringFirstChar) const noexcept;

	// for Nickname Cont Hash
	_NODISCARD int GetNicknameFirstChar(const Type_Nickname& InKeyNickname) const noexcept;

	void SaveNicknameContProcess(const Type_ID& InID, const Type_Nickname& InNickname);
	void _SaveNicknameCont();
public:
	__inline void SetGameResult(const shared_ptr<UserData>& InUserData, const bool InWinOrLose) noexcept { InUserData->SetGameResult(InWinOrLose); }
};

