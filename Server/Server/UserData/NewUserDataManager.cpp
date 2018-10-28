#include "../UserData/NewUserDataManager.h"

NewUserDataManager::NewUserDataManager()
{
	userDataCont.reserve(40); // A ~ Z : 26 , 0 ~ 9 : 10 , . : 1 -> 36
}


NewUserDataManager::~NewUserDataManager()
{

}

void NewUserDataManager::Load()
{
	std::ifstream inFile("UserData/UserData.txt", std::ios::in);

	std::string ID;
	int PW, winCount, loseCount, Money;
	int userDataCount{};

	inFile >> userDataCount;

	userDataCont.reserve(userDataCount);

	for (int i = 0; i < userDataCount; i++) {
		inFile >> ID >> PW >> winCount >> loseCount >> Money;

		userDataCont.emplace_back(ID, PW, winCount, loseCount, Money);
	}

	inFile.close();

	std::cout << "     [UserDataManager] Load UserData Complete! " << std::endl;
}

void NewUserDataManager::Save(bool& InIsSave)
{
	if (InIsSave) {
		InIsSave = false;

		Sleep(2000);
		std::ofstream outFile("UserData/UserData.txt", std::ios::out);

		outFile << userDataCont.size() << std::endl;

		for (auto i : userDataCont) {
			outFile << " " << i.GetID()
				<< " " << i.GetPW()
				<< " " << i.GetWinCount()
				<< " " << i.GetLoseCount()
				<< " " << i.GetMoney()
				<< std::endl;
		}
		outFile.close();

		std::cout << "     [UserDataManager] Save UserData! " << std::endl;

		InIsSave = false;

		Sleep(2000);
	}
}

int NewUserDataManager::SignIn(const string& InID, const int& InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney, int& RetIndex)
{
	RetIndex = 0;

	string fileNameBuffer = "UserData/Saved/AA_UserData.txt";
	fileNameBuffer[15] = InID[0];
	fileNameBuffer[16] = InID[1];

	std::ifstream inFile(fileNameBuffer, std::ios::in);

	std::string ID;
	int PW, winCount, loseCount, Money;
	int userDataCount{};

	inFile >> userDataCount;

	for (int i = 0; i < userDataCount; i++) {
		inFile >> ID >> PW >> winCount >> loseCount >> Money;

		
	}



	// �ش� ���̵� ����.
	return 1;

	for (auto &i : userDataCont)
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

int NewUserDataManager::SignIn(const char* InID, const int& InPW, int& RetWinCount, int& RetLoseCount, int& RetMoney, int& RetIndex)
{
	RetIndex = 0;
	for (auto &i : userDataCont)
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

int NewUserDataManager::SignUp(const string& InID, const int& InPW, int& RetIndex)
{
	for (auto &i : userDataCont)
	{
		if (i.GetID().compare(InID) == 0) {
			return 4;
		}
	}

	userDataCont.emplace_back(InID, InPW);
	RetIndex = userDataCont.size() - 1;

	return 0;
}

int NewUserDataManager::SignUp(const char* InID, const int& InPW, int& RetIndex)
{
	for (auto &i : userDataCont)
	{
		if (i.GetID().compare(InID) == 0) {
			return 4;
		}
	}

	userDataCont.emplace_back(InID, InPW);
	RetIndex = userDataCont.size() - 1;
	return 0;
}

void NewUserDataManager::SignOut(const int& InClientIndex)
{
	userDataCont[InClientIndex].SignOut();
}

//void UserDataManager::EmplaceBackToPlayer(const string& InID, const int& InPW, int& RetIndex) {
//	userDataCont.emplace_back(InID, InPW);
//	RetIndex = userDataCont.size() - 1;
//}