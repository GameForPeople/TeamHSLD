#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/NewUserData.h"

class NewUserDataManager {
	std::vector<map<string, NewUserData>> userDataCont;

public:
	//NewUserDataManager() = delete;
	//~NewUserDataManager() = delete;

public:
	// �α��� �� ȸ������ ��� ���⼭ ó���մϴ�.
	int LoginProcess(SocketInfo* InPSocketInfo, const string& InID, string& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
		int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<string>& RetFriendStringCont);

	void LogoutProcess(SocketInfo* InPSocketInfo);

	//void EmplaceBackToPlayer(const string& InID, const int& InPW, int& RetIndex);

public:
	// for userDataCont Hash, ù����ó���ʿ�.
	int GetStringFirstChar(const char& InStringFirstChar);

	//NewUserData* a = const_cast<NewUserData*>(&userDataCont[InIndex].find("abc")->second);
	__inline int GetUserDataSize() const
	{
		return userDataCont.size();
	}

	//�̰� ����Ǵ��� Ȯ���� �� �� �ֳ�..? �̰� �����ų�Ÿ� �׳� ���� Public�� �ξ�����..�Ѥ�
	//__inline std::vector<UserData> GetPlayer() {
	//	return player;
	//}

	__inline void SetGameResult(const int& InPlayerIndex, const bool& InWinOrLose)
	{
		userDataCont[InPlayerIndex].SetGameResult(InWinOrLose);
	}
};

