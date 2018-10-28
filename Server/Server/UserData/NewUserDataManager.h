#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/NewUserData.h"

class NewUserDataManager {
	std::vector<map<string, NewUserData>> userDataCont;

public:
	//NewUserDataManager() = delete;
	//~NewUserDataManager() = delete;

public:
	//void Load();

	//void Save(bool& InIsSave);

	int SignIn(const string& InID, const int& InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney, int& RetIndex);

	int SignIn(const char* InID, const int& InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney, int& RetIndex);

	int SignUp(const string& InID, const int& InPW, int& RetIndex);

	int SignUp(const char* InID, const int& InPW, int& RetIndex);

	void SignOut(const int& InClientIndex);

	//void EmplaceBackToPlayer(const string& InID, const int& InPW, int& RetIndex);

public:
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

