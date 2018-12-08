#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"
#include "../Custom_DataStructure/Custom_Map_RedBlackTree.h"
#include "../Custom_DataStructure/Custom_SET_RedBlackTree.h"


struct SocketInfo;

//using user_iter = map<const std::string, UserData>::iterator; // iter에서 node Pointer로 변경.

class UserDataManager {

	enum class LANGUAGE
	{
			ENGLISH =	0
		,	KOREAN	=	1
	};

	std::vector<CUSTOM_SET::rbTree<shared_ptr<UserData>, string>>	userDataCont;
	std::vector<CUSTOM_MAP::rbTree<wstring, string>>				nicknameCont;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>			converter; //DEV_66 닉네임 멀티바이트 <-> 유니코드 변환을 위해 사용.

	const wstring CONST_A; //DEV_66 닉네임 Cont 인덱스를 알기 위해 사용.
	const wstring CONST_Z;
	const wstring CONST_a;
	const wstring CONST_z;

	shared_ptr<UserData> CONST_NULL_USERDATA{nullptr};

public:
	UserDataManager(); // = delete;
	~UserDataManager(); // = delete;

public:
	// 로그인 및 회원가입 모두 여기서 처리합니다.
	int LoginProcess(SocketInfo* pInSocketInfo, const string& InID, wstring& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<wstring>& RetFriendStringCont);

	_NORETURN void LogoutProcess(shared_ptr<UserData>& pUserNode);

	shared_ptr<UserData> SearchUserNode(const string& keyString, bool& RetIsOnLogin);	// 아이디를 통해, 유저 노드를 탐색합니다.

	shared_ptr<UserData> SearchUserNodeWithNickname(const wstring& KeyNickName, bool& RetIsOnLogin, bool& RetIsMatch);	// 닉네임을 통해, 유저노드를 탐색합니다.

private:
	// for userDataCont Hash, 첫글자처리필요.
	_NODISCARD int GetStringFirstChar(const char& InStringFirstChar) noexcept;

public:
	_NORETURN __inline void SetGameResult(const shared_ptr<UserData>& InUserData, const bool& InWinOrLose) noexcept { InUserData->SetGameResult(InWinOrLose); }
};

