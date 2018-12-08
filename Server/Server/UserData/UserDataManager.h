#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"
#include "../Custom_DataStructure/Custom_Map_RedBlackTree.h"
#include "../Custom_DataStructure/Custom_SET_RedBlackTree.h"


struct SocketInfo;

//using user_iter = map<const std::string, UserData>::iterator; // iter���� node Pointer�� ����.

class UserDataManager {

	enum class LANGUAGE
	{
			ENGLISH =	0
		,	KOREAN	=	1
	};

	std::vector<CUSTOM_SET::rbTree<shared_ptr<UserData>, string>>	userDataCont;
	std::vector<CUSTOM_MAP::rbTree<wstring, string>>				nicknameCont;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>			converter; //DEV_66 �г��� ��Ƽ����Ʈ <-> �����ڵ� ��ȯ�� ���� ���.

	const wstring CONST_A; //DEV_66 �г��� Cont �ε����� �˱� ���� ���.
	const wstring CONST_Z;
	const wstring CONST_a;
	const wstring CONST_z;

	shared_ptr<UserData> CONST_NULL_USERDATA{nullptr};

public:
	UserDataManager(); // = delete;
	~UserDataManager(); // = delete;

public:
	// �α��� �� ȸ������ ��� ���⼭ ó���մϴ�.
	int LoginProcess(SocketInfo* pInSocketInfo, const string& InID, wstring& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<wstring>& RetFriendStringCont);

	_NORETURN void LogoutProcess(shared_ptr<UserData>& pUserNode);

	shared_ptr<UserData> SearchUserNode(const string& keyString, bool& RetIsOnLogin);	// ���̵� ����, ���� ��带 Ž���մϴ�.

	shared_ptr<UserData> SearchUserNodeWithNickname(const wstring& KeyNickName, bool& RetIsOnLogin, bool& RetIsMatch);	// �г����� ����, ������带 Ž���մϴ�.

private:
	// for userDataCont Hash, ù����ó���ʿ�.
	_NODISCARD int GetStringFirstChar(const char& InStringFirstChar) noexcept;

public:
	_NORETURN __inline void SetGameResult(const shared_ptr<UserData>& InUserData, const bool& InWinOrLose) noexcept { InUserData->SetGameResult(InWinOrLose); }
};

