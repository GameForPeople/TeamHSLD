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

	std::vector<CUSTOM_SET::rbTree<shared_ptr<UserData>, Type_ID>>	userDataCont;
	std::vector<CUSTOM_MAP::rbTree<Type_Nickname, Type_ID>>			nicknameCont;

	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>			converter; //DEV_66 닉네임 멀티바이트 <-> 유니코드 변환을 위해 사용.

	const Type_Nickname CONST_A; //DEV_66 닉네임 Cont 인덱스를 알기 위해 사용.
	const Type_Nickname CONST_Z;
	const Type_Nickname CONST_a;
	const Type_Nickname CONST_z;

	shared_ptr<UserData> CONST_NULL_USERDATA{nullptr};

public:
	UserDataManager() noexcept; // = delete;
	~UserDataManager(); // = delete;

public:
	// 로그인 및 회원가입 모두 여기서 처리합니다.
	int LoginProcess(SocketInfo* pInSocketInfo, const Type_ID& InID, Type_Nickname& RetNickname, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<Type_Nickname>& RetFriendNicknameCont);

	void LogoutProcess(shared_ptr<UserData>& pUserNode);

	_NODISCARD shared_ptr<UserData> SearchUserNode(const Type_ID& keyID, bool& RetIsOnLogin);	// 아이디를 통해, 유저 노드를 탐색합니다.

	_NODISCARD shared_ptr<UserData> SearchUserNodeWithNickname(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch);	// 닉네임을 통해, 유저노드를 탐색합니다.

private:
	// for userDataCont Hash, 첫글자처리필요.
	_NODISCARD int GetStringFirstChar(const char& InStringFirstChar) const noexcept;

public:
	__inline void SetGameResult(const shared_ptr<UserData>& InUserData, const bool& InWinOrLose) noexcept { InUserData->SetGameResult(InWinOrLose); }
};

