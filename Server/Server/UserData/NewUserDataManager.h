#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/NewUserData.h"

using user_iter = map<const std::string, NewUserData>::iterator;

class NewUserDataManager {
	std::vector<map<string, NewUserData>> userDataCont;

public:
	//NewUserDataManager() = delete;
	//~NewUserDataManager() = delete;

public:
	// 로그인 및 회원가입 모두 여기서 처리합니다.
	int LoginProcess(SocketInfo* InPSocketInfo, const string& InID, string& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<string>& RetFriendStringCont);

	void LogoutProcess(SocketInfo* InPSocketInfo);

private:
	// for userDataCont Hash, 첫글자처리필요.
	int GetStringFirstChar(const char& InStringFirstChar);

public:
	__inline void SetGameResult(map<const std::string, NewUserData>::iterator& InUserIter, const bool& InWinOrLose)
	{
		InUserIter->second.SetGameResult(InWinOrLose);
	}
};

