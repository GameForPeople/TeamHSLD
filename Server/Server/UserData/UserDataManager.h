#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"
#include "../Custom_DataStructure/Custom_Map_RedBlackTree.h"
#include "../Custom_DataStructure/Custom_SET_RedBlackTree.h"


struct SocketInfo;

//using user_iter = map<const std::string, UserData>::iterator; // iter���� node Pointer�� ����.

class UserDataManager {

	enum class LANGUAGE // : int
	{
			ENGLISH =	0
		,	KOREAN	=	1
		,	OTHERS	=	2
	};

	string				nicknameContFileName;

	std::vector<CUSTOM_SET::rbTree<shared_ptr<UserData>, Type_ID>>	userDataCont;
	std::vector<CUSTOM_MAP::rbTree<Type_Nickname, Type_ID>>			nicknameCont;

	//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>			converter; //DEV_66 �г��� ��Ƽ����Ʈ <-> �����ڵ� ��ȯ�� ���� ���.
	shared_ptr<UserData> CONST_NULL_USERDATA{nullptr};

	CRITICAL_SECTION	csNicknameCont;
	CRITICAL_SECTION	csSaveNickname;

	std::vector<std::pair<Type_ID, Type_Nickname>> saveFileCont;

public:
	UserDataManager() noexcept; // = delete;
	~UserDataManager(); // = delete;

public:
	// �α��� �� ȸ������ ��� ���⼭ ó���մϴ�.
	int LoginProcess(SocketInfo* pInSocketInfo, const Type_ID& InID, Type_Nickname& RetNickname, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<Type_Nickname>& RetFriendNicknameCont);

	void LogoutProcess(shared_ptr<UserData>& pUserNode);

	_NODISCARD shared_ptr<UserData> SearchUserNode(const Type_ID& keyID, bool& RetIsOnLogin);	// ���̵� ����, ���� ��带 Ž���մϴ�.

	_NODISCARD shared_ptr<UserData> SearchUserNodeWithNickname(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch);	// �г����� ����, ������带 Ž���մϴ�.

	// Nickname Cont����, �ش� �г��� Ȯ�� ��, ���� ���, ����.
	_NODISCARD bool SetNewNickname(SocketInfo* pInClient, const Type_Nickname& InNewString);

private:
	// for userDataCont Hash, ù����ó���ʿ�.
	_NODISCARD int GetStringFirstChar(const char& InStringFirstChar) const noexcept;

	// for Nickname Cont Hash
	_NODISCARD int GetNicknameFirstChar(const Type_Nickname& InKeyNickname) const noexcept;

	void SaveNicknameContProcess(const Type_ID& InID, const Type_Nickname& InNickname);
	void _SaveNicknameCont();
public:
	__inline void SetGameResult(const shared_ptr<UserData>& InUserData, const bool& InWinOrLose) noexcept { InUserData->SetGameResult(InWinOrLose); }
};

