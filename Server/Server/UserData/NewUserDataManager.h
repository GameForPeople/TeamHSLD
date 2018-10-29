#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/NewUserData.h"

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

	//void EmplaceBackToPlayer(const string& InID, const int& InPW, int& RetIndex);

public:
	// for userDataCont Hash, 첫글자처리필요.
	int GetStringFirstChar(const char& InStringFirstChar);

	//NewUserData* a = const_cast<NewUserData*>(&userDataCont[InIndex].find("abc")->second);
	__inline int GetUserDataSize() const
	{
		return userDataCont.size();
	}

	//이거 복사되는지 확실히 알 수 있나..? 이거 복사시킬거면 그냥 차라리 Public이 훨씐낮지..ㅡㅡ
	//__inline std::vector<UserData> GetPlayer() {
	//	return player;
	//}

	__inline void SetGameResult(const int& InPlayerIndex, const bool& InWinOrLose)
	{
		userDataCont[InPlayerIndex].SetGameResult(InWinOrLose);
	}
};

