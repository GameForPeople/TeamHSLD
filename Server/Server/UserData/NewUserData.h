#pragma once

#include "../PCH/stdafx.h"
#include "../IOCPServer/SocketInfo.h"

class NewUserData {
	SocketInfo*				pSocketInfo;			// ���� ���� ����ü

	std::string				id{};					// ���̵�
	std::string				nickName{};				//�г���

	int						winCount;				//�¸� Ƚ��
	int						loseCount;				//�й� Ƚ��
	int						money;					//�ΰ��� ��ȭ

	int						achievementBit;			//���� ��Ʈ
	int						titleBit;				//Īȣ ��Ʈ
	int						characterBit;			//���� ĳ���� ��Ʈ

	vector<std::string>		friendStringCont;		// ģ�� �̸� �����̳�
	vector<SocketInfo*>		friendSocketInfoCont;	// ģ�� ���� ���� �����̳�

public:
	NewUserData() = delete;

	// ģ���� ���� ���. friendSocketInfoCont��, �÷��̾ ģ�� ���� UI�� ��û�� ��쿡�� üũ�մϴ�. (����ȭ ��, DB ���ʿ� ������)
	NewUserData(SocketInfo* InPSocketInfo, const std::string& InID, const std::string& InNickName,
		const int& InWinCount, const int& InLoseCount, const int& InMoney, 
		const int& InAchievementBit, const int& InTitleBit, const int& InCharacterBit, 
		const vector<string>& InFriendStringCont) //, const std::vector<SOCKETINFO*>& InFriendSocketInfoCont)

		: pSocketInfo(InPSocketInfo), id(InID), nickName(InNickName), 
		winCount(InWinCount), loseCount(InLoseCount), money(InMoney), 
		achievementBit(InAchievementBit), titleBit(InTitleBit), characterBit(InCharacterBit),
		friendStringCont(InFriendStringCont), friendSocketInfoCont()
	{};

	//ģ���� ���� ���. default�� init��.
	NewUserData(SocketInfo* InPSocketInfo, const std::string& InID, const std::string& InNickName,
		const int& InWinCount, const int& InLoseCount, const int& InMoney,
		const int& InAchievementBit, const int& InTitleBit, const int& InCharacterBit)
		
		: pSocketInfo(InPSocketInfo), id(InID), nickName(InNickName),
		winCount(InWinCount), loseCount(InLoseCount), money(InMoney),
		achievementBit(InAchievementBit), titleBit(InTitleBit), characterBit(InCharacterBit),
		friendStringCont(), friendSocketInfoCont()
	{};

	//ȸ������ó��
	NewUserData(SocketInfo* InPSocketInfo, const std::string& InID)

		: pSocketInfo(InPSocketInfo), id(InID), nickName(),
		winCount(0), loseCount(0), money(0),
		achievementBit(0), titleBit(0), characterBit(0),
		friendStringCont(), friendSocketInfoCont()
	{};

	//������!
	~NewUserData() 
	{
		pSocketInfo = nullptr;

		for (auto iter = friendSocketInfoCont.begin(); iter != friendSocketInfoCont.end(); ++iter)
		{
			*iter = nullptr;
		}
	};

public:
	//���� : �ش� �Լ��� ����� �뵵�θ� ����ϼ���.
	__inline void	PrintUserData() const
	{
		std::cout 
			<< "id : " << id
			<< ", nickName : " << nickName
			<< ", winCount : " << winCount
			<< ", loseCount : " << loseCount
			<< ", money : " << money
			<< ", achievementBit : " << achievementBit
			<< ", titleBit : " << titleBit
			<< ", characterBit : " << characterBit
			// friend�� ����ϱ� ��ƽ� -> �������ϴ�.
			<< "\n";
	}

	__inline string	GetID()  const { return id; }
	__inline string	GetNickName()   const { return nickName; }
	__inline int	GetWinCount()  const { return winCount; }
	__inline int	GetLoseCount()  const { return loseCount; }
	__inline int	GetMoney()  const { return money; }
	__inline int	GetAchievementBit()  const { return achievementBit; }
	__inline int	GetTitleBit()  const { return titleBit; }
	__inline int	GetCharacterBit()  const { return characterBit; }
	__inline vector<string> GetFriendStringCont() const { return friendStringCont; }
	// �ִ� �� �ߺ�...?
	//__inline void	SetWinOrLose(const int& value) {
	//	if (value == 1) { winCount++; }
	//	else if (value == 2) { loseCount++; }
	//}

	// True�� ��� �¸�, False�� ��� �й�.
	__inline void	SetGameResult(const bool& InWinOrLose)
	{
		if (InWinOrLose) ++winCount;
		else ++loseCount;
	}
};