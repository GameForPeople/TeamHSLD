#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"
#include "../UserData/Custom_Map_RedBlackTree.h"


struct SocketInfo;

//using user_iter = map<const std::string, UserData>::iterator; // iter���� node Pointer�� ����.

class UserDataManager {

	enum class LANGUAGE
	{
			ENGLISH =	0
		,	KOREAN	=	1
	};

	std::vector<rbTree<string, UserData>>	userDataCont;
	std::vector<rbTree<wstring, string>>	nicknameCont;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; //DEV_66 �г��� ��Ƽ����Ʈ <-> �����ڵ� ��ȯ�� ���� ���.

	const wstring CONST_A; //DEV_66 �г��� Cont �ε����� �˱� ���� ���.
	const wstring CONST_Z;
	const wstring CONST_a;
	const wstring CONST_z;

public:
	UserDataManager(); // = delete;
	~UserDataManager(); // = delete;

public:
	// �α��� �� ȸ������ ��� ���⼭ ó���մϴ�.
	int LoginProcess(SocketInfo* pInSocketInfo, const string& InID, wstring& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<wstring>& RetFriendStringCont);

	void LogoutProcess(SocketInfo* pInSocketInfo);

	rbTreeNode<string, UserData>* SearchUserNode(const string& keyString, bool& RetBool);	// ���̵� ����, ���� ��带 Ž���մϴ�.

	rbTreeNode<string, UserData>* SearchUserNodeWithNickname(const wstring& KeyNickName, bool& RetBool);	// �г����� ����, ������带 Ž���մϴ�.

private:
	// for userDataCont Hash, ù����ó���ʿ�.
	int GetStringFirstChar(const char& InStringFirstChar);

public:
	void SetGameResult(rbTreeNode<string, UserData>* InUserIter, const bool& InWinOrLose);
};

