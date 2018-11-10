#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"
#include "../UserData/Custom_Map_RedBlackTree.h"


struct SocketInfo;

//using user_iter = map<const std::string, UserData>::iterator; // iter에서 node Pointer로 변경.

class UserDataManager {
	std::vector<rbTree<string, UserData>> userDataCont;

public:
	UserDataManager(); // = delete;
	~UserDataManager(); // = delete;

public:
	// 로그인 및 회원가입 모두 여기서 처리합니다.
	int LoginProcess(SocketInfo* pInSocketInfo, const string& InID, string& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<string>& RetFriendStringCont);

	void LogoutProcess(SocketInfo* pInSocketInfo);

private:
	// for userDataCont Hash, 첫글자처리필요.
	int GetStringFirstChar(const char& InStringFirstChar);

public:
	void SetGameResult(rbTreeNode<string, UserData>* InUserIter, const bool& InWinOrLose);
};

