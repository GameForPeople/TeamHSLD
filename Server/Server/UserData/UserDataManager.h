#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"

struct SocketInfo;

//using user_iter = map<const std::string, UserData>::iterator;
typedef map<const std::string, UserData>::iterator user_iter;

class UserDataManager {
	std::vector<map<string, UserData>> userDataCont;

public:
	UserDataManager(); // = delete;
	~UserDataManager(); // = delete;

public:
	// �α��� �� ȸ������ ��� ���⼭ ó���մϴ�.
	int LoginProcess(SocketInfo* InPSocketInfo, const string& InID, string& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<string>& RetFriendStringCont);

	void LogoutProcess(SocketInfo* InPSocketInfo);

private:
	// for userDataCont Hash, ù����ó���ʿ�.
	int GetStringFirstChar(const char& InStringFirstChar);

public:
	void SetGameResult(user_iter* InUserIter, const bool& InWinOrLose);
};

