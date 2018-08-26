#pragma once

#include "stdafx.h"

class UserData {
	//basic Data
	std::string m_id{};
	int m_pw{};
	int m_winCount{};
	int m_loseCount{};
	int m_money{};

	//Game use Data
	bool m_isLogin{ false };
	IN_ADDR m_userAddr{};

public:
	__inline UserData() {};

	// ȸ�����Կ�! �ٷ� �α��ε�!
	__inline UserData(std::string InID, const int InPW) : m_id(InID), m_pw(InPW), m_winCount(0), m_loseCount(0), m_money(0), m_isLogin(true)
	{ };

	// Data Load ��! �α��ε��� ����!! ȸ�����Կ� ���� �ȉ�!!
	__inline UserData(std::string InID, const int InPW, const int InWinCount, const int InloseCount, const int InMoney)
		: m_id(InID), m_pw(InPW), m_winCount(InWinCount), m_loseCount(InloseCount), m_money(InMoney), m_isLogin(false)
	{ };

	__inline ~UserData() {};

public:
	__inline void	SetIPAddress(IN_ADDR& inputAddress) { m_userAddr = inputAddress; m_isLogin = true; }

	__inline void	PrintUserData() const 
	{
		std::cout << m_id << "  " << m_pw << "  " << m_winCount << "  " << m_loseCount << std::endl;
	}

	__inline string	GetID()  const { return m_id; }
	__inline int	GetPW()   const { return m_pw; }
	__inline int	GetWinCount()  const { return m_winCount; }
 	__inline int	GetLoseCount()  const { return m_loseCount; }
	__inline int	GetMoney()  const { return m_money; } 
	__inline bool	GetIsLogin()  const { return m_isLogin; } 
	__inline void	SetIsLogin(bool bValue) { m_isLogin = bValue; }

	__inline void	SetWinOrLose(const int value) {
		if (value == 1) { m_winCount++; }
		else if (value == 2) { m_loseCount++; }
		return;
	}
};

class CUserData {
	std::vector<UserData> player;

public:
	__inline CUserData() = default;
	__inline ~CUserData() = default;
public:
	void Load() 
	{
		std::ifstream inFile("UserData.txt", std::ios::in);

		std::string ID;
		int PW, winCount, loseCount, Money;
		int userDataCount{};

		inFile >> userDataCount;
		player.reserve(userDataCount);

		for (int i = 0; i < userDataCount; i++) {
			inFile >> ID >> PW >> winCount >> loseCount >> Money;

			player.emplace_back(ID, PW, winCount, loseCount, Money);
		}

		inFile.close();

		std::cout << " [System] Load UserData Complete! " << std::endl << std::endl;
	}
	void Save(bool InIsSave)
	{
		if (InIsSave) {
			InIsSave = false;

			Sleep(2000);
			std::ofstream outFile("UserData.txt", std::ios::out);

			outFile << player.size() << std::endl;

			for (auto i : player) {
				outFile << " " << i.GetID()
					<< " " << i.GetPW()
					<< " " << i.GetWinCount()
					<< " " << i.GetLoseCount()
					<< " " << i.GetMoney()
					<< std::endl;
			}
			outFile.close();

			std::cout << " [System] Save UserData! " << std::endl;
			InIsSave = false;

			Sleep(2000);
		}
	}

	int SignIn(string InID, const int InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney)
	{
		for (auto &i : player)
		{
			if (i.GetID().compare(InID) == 0)
			{
				if (!i.GetIsLogin())
				{
					if (i.GetPW() == InPW)
					{
						i.SetIsLogin(true);
						RetWinCount = i.GetWinCount();
						RetLoseCount = i.GetLoseCount();
						RetMoney = i.GetMoney();
						return 0;
					}
					else
					{
						return 2;
					}
				}
				else
				{
					return 3;
				}
			}
		}

		return 1;
	}
	int SignIn(char* InID, const int InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney)
	{
		for (auto &i : player)
		{
			if (i.GetID().compare(InID) == 0)
			{
				if (!i.GetIsLogin())
				{
					if (i.GetPW() == InPW)
					{
						i.SetIsLogin(true);
						RetWinCount = i.GetWinCount();
						RetLoseCount = i.GetLoseCount();
						RetMoney = i.GetMoney();
						return 0;
					}
					else
					{
						return 2;
					}
				}
				else
				{
					return 3;
				}
			}
		}
		return 1;
	}

	__inline int SignUp(string InID)
	{
		for (auto &i : player)
		{
			if (i.GetID().compare(InID) == 0) {
				return 4;
			}
		}

		return 0;
	}
	__inline int SignUp(char* InID)
	{
		for (auto &i : player)
		{
			if (i.GetID().compare(InID) == 0) {
				return 4;
			}
		}

		return 0;
	}

	__inline void EmplaceBackToPlayer(string InID, const int InPW) {
		player.emplace_back(InID, InPW);
	}

	__inline int GetUserDataSize() {
		return player.size();
	}

	//�̰� ����Ǵ��� Ȯ���� �� �� �ֳ�..? �̰� �����ų�Ÿ� �׳� ���� Public�� �ξ�����..�Ѥ�
	__inline std::vector<UserData> GetPlayer() {
		return player;
	}

};
