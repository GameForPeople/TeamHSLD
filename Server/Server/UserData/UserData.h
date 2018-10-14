#pragma once

/*
UserData.h ���� �ؾߵ� ��!
  1. UserData�� �����̻� Ŀ����, Vector���� �ٸ� ������ ������ �ʿ��� �� ����. (�ӵ� ����) -> �ٵ� �� ���� ptr-> �κп� ���̵�, ��й�ȣ �߰��ϰ� ���� ������� �Ϳ�!
  2. Vector�����̳��� Index�� Key�� ���� ���� �������� Sort ���� �� �ϸ�, ������ ���� ������..
  3. (�� userData �����̳ʸ� Vector�϶��� �ش� ���� ����, �ƴϸ� ������. ) SignIn, SignUp���� Iter�� ������ ����, �Լ� ���ڷ� ���޵�, RetIndex�� For�� [RetIndex] �������� ���� �ʿ� ( ����ȭ )

  // 1, 2, 3�� ������� �����, Index����, Iter Ȥ��, �ش� ����ü��ġ�� ������ ȹ���ؼ� UserData����ȯ�ؼ� ����ϴ��� �ؾ���! --> �̰� �� ���� ��.
*/

#include "../stdafx.h"

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
	__inline void  SignOut()
	{
		m_isLogin = false;
	}

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
	__inline void	SetIsLogin(const bool& bValue) { m_isLogin = bValue; }

	__inline void	SetWinOrLose(const int& value) {
		if (value == 1) { m_winCount++; }
		else if (value == 2) { m_loseCount++; }
		return;
	}
	__inline void	SetGameResult(const bool& InWinOrLose)
	{
		if (InWinOrLose)
			++m_winCount;
		else
			++m_loseCount;
	}
};


