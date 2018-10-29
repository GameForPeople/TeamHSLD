#include "../UserData/NewUserDataManager.h"

NewUserDataManager::NewUserDataManager()
{
	userDataCont.reserve(37); // A ~ Z : 26 , 0 ~ 9 : 10 , . : 1 -> 36

	for(int count = 0 ; count < 37; count++)
		userDataCont.emplace_back();
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

int NewUserDataManager::LoginProcess(SocketInfo* InPSocketInfo, const string& InID, string& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
	int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<string>& RetFriendStringCont)
{
	//RetFailReson = 0;
	// ���� ������ 
	int userDataContHashValueBuffer = GetStringFirstChar(InID[0]);

	// �̹� �α��� ���� üũ.
	if (userDataCont[userDataContHashValueBuffer].find(InID) == userDataCont[userDataContHashValueBuffer].end())
	{
		return 3; 
	}

	// �������� ��
	RetMoney = -1; // ��ȭ�� -1�ϸ� ����. �ش��Ȳ��, ���� ���� ���θ� Ȯ���ϱ� ����.
	int friendNum;			// (ģ�� �������)
	string idStringBuffer;			// idBuffer

	// �ð� ����.
	//time_t timeBuffer = time(0);   // get time now
	//tm* now = std::localtime(&timeBuffer);
	//(now->tm_year + 1900)
	//(now->tm_mon + 1)
	//now->tm_mday

	// ���ϸ� ����
	string fileNameBuffer = "UserData/Saved/.txt";
	fileNameBuffer.insert(fileNameBuffer.begin() + 15, InID.begin(), InID.end());
	//fileNameBuffer[15] = InID[0];
	//fileNameBuffer[16] = InID[1];

	// ���� ����
	std::ifstream inFile(fileNameBuffer, std::ios::in);

	// ���� ������ �ε�
	inFile >> idStringBuffer >> RetNickName >> RetWinCount >> RetLoseCount >> RetMoney >> RetAchievementBit >> RetTitleBit >> RetCharacterBit >> friendNum;
	

	// �ش� ������ ���� ��� ����.
	if (RetMoney == -1)
	{
		InPSocketInfo->userIter = userDataCont[userDataContHashValueBuffer].emplace(InID, NewUserData(InPSocketInfo, InID)).first;
		return 0;
	}

	//ģ�� ����. ���� �α���.
	if (friendNum == 0)
	{
		InPSocketInfo->userIter = userDataCont[userDataContHashValueBuffer].emplace(InID, NewUserData(InPSocketInfo, InID, RetNickName,
			RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit)).first;
		return 0;
	}

	// ģ�� ���� ���� ���� reserve.
	RetFriendStringCont.reserve(friendNum); // �ش� ���� reserve.

	// ģ�� �����͸�, ����.
	for (int iter = 0; iter < friendNum; ++iter) // idBuffer�� ���۷��ؼ� ���.
	{
		inFile >> idStringBuffer;
		RetFriendStringCont.emplace_back(idStringBuffer);

		// ģ���� �α��� ������ �ƴ����� üũ��, ���⼭ �̷������ �ȵǰ�, ģ�� UI�� On�� ��� �ؾ���. -> ¥�� �ϵ忡�� Load�� �ʿ� ���� ��������.
		//if (userDataCont[userDataContHashValueBuffer].find(idStringBuffer) == userDataCont[userDataContHashValueBuffer].end())
	}

	// �������� �α���.
	InPSocketInfo->userIter = userDataCont[userDataContHashValueBuffer].emplace(InID, NewUserData(InPSocketInfo, InID, RetNickName,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetFriendStringCont)).first;

	return 0;
}

void NewUserDataManager::LogoutProcess(SocketInfo* InPSocketInfo)
{
	// ���ϸ� ����
	string fileNameBuffer = "UserData/Saved/.txt";
	fileNameBuffer.insert(fileNameBuffer.begin() + 15, InPSocketInfo->userIter->second.GetID().begin(), InPSocketInfo->userIter->second.GetID().end());

	// ���� ���� ���� ����
	std::ofstream outFile(fileNameBuffer, std::ios::out);
	
	// ���� ���� (����)
	outFile
		<< " " << InPSocketInfo->userIter->second.GetID()
		<< " " << InPSocketInfo->userIter->second.GetNickName()
		<< " " << InPSocketInfo->userIter->second.GetWinCount()
		<< " " << InPSocketInfo->userIter->second.GetLoseCount()
		<< " " << InPSocketInfo->userIter->second.GetMoney()
		<< " " << InPSocketInfo->userIter->second.GetAchievementBit()
		<< " " << InPSocketInfo->userIter->second.GetTitleBit()
		<< " " << InPSocketInfo->userIter->second.GetCharacterBit()
		<< " " << InPSocketInfo->userIter->second.GetFriendStringCont().size() << std::endl;

	// ģ�� ������ �ش� �̸� (����)
	for (auto iter = InPSocketInfo->userIter->second.GetFriendStringCont().begin();
		iter != InPSocketInfo->userIter->second.GetFriendStringCont().end();
		++iter)
	{
		outFile << " " << *iter << std::endl;
	}

	// UserCont(MAP)���� �ش� ���� ����.
	userDataCont[GetStringFirstChar(InPSocketInfo->userIter->second.GetID[0])].erase(InPSocketInfo->userIter);
}

int NewUserDataManager::GetStringFirstChar(const char& InStringFirstChar)
{
	// 0 ~ 25
	// '@' = 64, 'A' = 65,,, 'Z' = 90, '[' = 91
	if (InStringFirstChar > '@' && InStringFirstChar < '[')
	{
		return InStringFirstChar - 65;
	}
	// ''' = 96, 'a' = 97,,, 'z' = 122, '{' = 123
	else if (InStringFirstChar > 96 && InStringFirstChar < '{')
	{
		return InStringFirstChar - 97;
	}
	// 26 ~ 35
	// '/' = 47, '0' = 48,,, '9' = 57, ':' = 58
	else if (InStringFirstChar > '/' && InStringFirstChar < ':')
	{
		return InStringFirstChar - 22;
	}
	else
	{
		return 36;
	}
}




//void UserDataManager::EmplaceBackToPlayer(const string& InID, const int& InPW, int& RetIndex) {
//	userDataCont.emplace_back(InID, InPW);
//	RetIndex = userDataCont.size() - 1;
//}