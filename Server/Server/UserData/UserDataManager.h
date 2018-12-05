#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"
#include "../UserData/Custom_Map_RedBlackTree.h"


struct SocketInfo;

//using user_iter = map<const std::string, UserData>::iterator; // iter에서 node Pointer로 변경.

class UserDataManager {

	enum class LANGUAGE
	{
			ENGLISH =	0
		,	KOREAN	=	1
	};

	std::vector<rbTree<string, UserData>>	userDataCont;
	std::vector<rbTree<wstring, string>>	nicknameCont;

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; //DEV_66 닉네임 멀티바이트 <-> 유니코드 변환을 위해 사용.

	const wstring CONST_A; //DEV_66 닉네임 Cont 인덱스를 알기 위해 사용.
	const wstring CONST_Z;
	const wstring CONST_a;
	const wstring CONST_z;

public:
	UserDataManager(); // = delete;
	~UserDataManager(); // = delete;

public:
	// 로그인 및 회원가입 모두 여기서 처리합니다.
	int LoginProcess(SocketInfo* pInSocketInfo, const string& InID, wstring& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<wstring>& RetFriendStringCont);

	void LogoutProcess(SocketInfo* pInSocketInfo);

	rbTreeNode<string, UserData>* SearchUserNode(const string& keyString, bool& RetBool);	// 아이디를 통해, 유저 노드를 탐색합니다.

	rbTreeNode<string, UserData>* SearchUserNodeWithNickname(const wstring& KeyNickName, bool& RetBool);	// 닉네임을 통해, 유저노드를 탐색합니다.

private:
	// for userDataCont Hash, 첫글자처리필요.
	int GetStringFirstChar(const char& InStringFirstChar);

public:
	void SetGameResult(rbTreeNode<string, UserData>* InUserIter, const bool& InWinOrLose);
};

