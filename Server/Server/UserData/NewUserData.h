#pragma once

#include "../PCH/stdafx.h"

class NewUserData {
	//basic Data
	std::string m_id{};
	int m_pw{};
	int m_winCount{};
	int m_loseCount{};
	int m_money{};

	//Game use Data
	SOCKETINFO* pSocketInfo{};

public:
	NewUserData() = delete;

	NewUserData(SOCKETINFO* InPSocketInfo, std::string InID, const int InPW) 
		: pSocketInfo(InPSocketInfo), m_id(InID), m_pw(InPW), m_winCount(0), m_loseCount(0), m_money(0)
	{ };

	NewUserData(SOCKETINFO* InPSocketInfo, std::string InID, const int InPW, const int InWinCount, const int InloseCount, const int InMoney)
		: pSocketInfo(InPSocketInfo), m_id(InID), m_pw(InPW), m_winCount(InWinCount), m_loseCount(InloseCount), m_money(InMoney)
	{ };

	~NewUserData() 
	{
		pSocketInfo = nullptr;
	};

public:
	__inline void	PrintUserData() const
	{
		std::cout << m_id << "  " << m_pw << "  " << m_winCount << "  " << m_loseCount << std::endl;
	}

	__inline string	GetID()  const { return m_id; }
	__inline int	GetPW()   const { return m_pw; }
	__inline int	GetWinCount()  const { return m_winCount; }
	__inline int	GetLoseCount()  const { return m_loseCount; }
	__inline int	GetMoney()  const { return m_money; }

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
