#pragma once

/*
UserData.h 에서 해야될 일!
  1. UserData가 일정이상 커지면, Vector말고 다른 로직의 적용이 필요할 수 있음. (속도 측면) -> 근데 그 순간 ptr-> 부분에 아이디, 비밀번호 추가하고 로직 엎어야함 와우!
  2. Vector컨테이너의 Index를 Key로 쓰는 지금 로직에서 Sort 같은 짓 하면, 유저들 정보 난리남..
  3. (단 userData 컨테이너를 Vector일때만 해당 내용 적용, 아니면 동일함. ) SignIn, SignUp에서 Iter로 돌리지 말고, 함수 인자로 전달된, RetIndex로 For문 [RetIndex] 돌리도록 변경 필요 ( 최적화 )

  // 1, 2, 3의 변경건이 생기면, Index말고, Iter 혹은, 해당 구조체위치의 포인터 획득해서 UserData형변환해서 사용하던가 해야함! --> 이게 더 좋은 듯.
*/

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

	// 회원가입용! 바로 로그인됨!
	__inline UserData(std::string InID, const int InPW) : m_id(InID), m_pw(InPW), m_winCount(0), m_loseCount(0), m_money(0), m_isLogin(true)
	{ };

	// Data Load 용! 로그인되지 않음!! 회원가입에 쓰면 안됌!!
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

	int SignIn(string InID, const int InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney, int& RetIndex)
	{
		RetIndex = 0;
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
			++RetIndex;
		}

		return 1;
	}
	int SignIn(char* InID, const int InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney, int& RetIndex)
	{
		RetIndex = 0;
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
			++RetIndex;
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

	__inline void EmplaceBackToPlayer(string InID, const int InPW, int& RetIndex) {
		player.emplace_back(InID, InPW);
		RetIndex = player.size() - 1;
	}

	__inline int GetUserDataSize() {
		return player.size();
	}

	//이거 복사되는지 확실히 알 수 있나..? 이거 복사시킬거면 그냥 차라리 Public이 훨씐낮지..ㅡㅡ
	//__inline std::vector<UserData> GetPlayer() {
	//	return player;
	//}
	__inline string GetUserID(const int InIndex)
	{
		return player[InIndex].GetID();
	}
};
