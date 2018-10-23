#pragma once

#include "../stdafx.h"
#include "../UserData/UserData.h"

class UserDataManager {
	std::vector<UserData> userDataCont;

public:
	__inline UserDataManager() = default;
	__inline ~UserDataManager() = default;

public:
	void Load();

	void Save(bool& InIsSave);

	int SignIn(const string& InID, const int& InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney, int& RetIndex);

	int SignIn(const char* InID, const int& InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney, int& RetIndex);

	int SignUp(const string& InID, const int& InPW, int& RetIndex);

	int SignUp(const char* InID, const int& InPW, int& RetIndex);

	void SignOut(const int& InClientIndex);

	//void EmplaceBackToPlayer(const string& InID, const int& InPW, int& RetIndex);

public:
	__inline int GetUserDataSize() const
	{
		return userDataCont.size();
	}

	//�̰� ����Ǵ��� Ȯ���� �� �� �ֳ�..? �̰� �����ų�Ÿ� �׳� ���� Public�� �ξ�����..�Ѥ�
	//__inline std::vector<UserData> GetPlayer() {
	//	return player;
	//}

	__inline string GetUserID(const int& InIndex) const
	{
		return userDataCont[InIndex].GetID();
	}

	__inline void SetGameResult(const int& InPlayerIndex, const bool& InWinOrLose) 
	{
		userDataCont[InPlayerIndex].SetGameResult(InWinOrLose);
	}
};

