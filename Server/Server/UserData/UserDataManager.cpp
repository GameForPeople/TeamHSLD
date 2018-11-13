#include "../UserData/UserDataManager.h"
#include "../IOCPServer/SocketInfo.h"

UserDataManager::UserDataManager()
{
	userDataCont.reserve(37); // A ~ Z : 26 , 0 ~ 9 : 10 , . : 1 -> 36

	for(int count = 0 ; count < 37; count++)
		userDataCont.emplace_back();
}

UserDataManager::~UserDataManager()
{

}

int UserDataManager::LoginProcess(SocketInfo* pInSocketInfo, const string& InID, string& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
	int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<string>& RetFriendStringCont)
{

	//RetFailReson = 0;
	// ���� ������ 
	pInSocketInfo->userDataContIndex = GetStringFirstChar(InID[0]);

	bool RetBoolBuffer;
	
	//pInSocketInfo->pUserNode = 
		userDataCont[pInSocketInfo->userDataContIndex].Search(InID, RetBoolBuffer);

	// �̹� �α��� ���� üũ.
	if (RetBoolBuffer)
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
		pInSocketInfo->pUserNode = userDataCont[pInSocketInfo->userDataContIndex].Insert(InID, UserData(pInSocketInfo, InID));

		// RetMoney�� -1�� ���, Ŭ���Ʈ���� �г��� �Է� �ʿ�. (Login Scene ? // Main UI Scene ?)

		std::cout << "���� ������ �ε忡 �����Ͽ� ȸ�������߽��ϴ�. \n";
		return 0;
	}

	std::cout << "�ε��� ������ �г����� : " << RetNickName << "�Դϴ�. \n";

	//ģ�� ����. ���� �α���.
	if (friendNum == 0)
	{
		pInSocketInfo->pUserNode = userDataCont[pInSocketInfo->userDataContIndex].Insert(InID, UserData(pInSocketInfo, InID, RetNickName,
			RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit));
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
	pInSocketInfo->pUserNode = userDataCont[pInSocketInfo->userDataContIndex].Insert(InID, UserData(pInSocketInfo, InID, RetNickName,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetFriendStringCont));

	return 0;
}

void UserDataManager::LogoutProcess(SocketInfo* pInSocketInfo)
{
	if (pInSocketInfo->pUserNode->SetValue().GetNickName().size() == 0)
	{
		// �� �̰Ŵ� �г��� ���°ž� �������� �� 
	}
	else
	{
		// ���ϸ� ����
		string fileNameBuffer = "UserData/Saved/.txt";
		string keyBuffer = pInSocketInfo->pUserNode->GetKey();

		UserData valueBuffer = pInSocketInfo->pUserNode->SetValue();

		fileNameBuffer.insert(fileNameBuffer.begin() + 15, keyBuffer.begin(), keyBuffer.end());

		// ���� ���� ���� ����
		std::ofstream outFile(fileNameBuffer, std::ios::out);

		// ���� ���� (����)
		//outFile
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetID()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetNickName()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetWinCount()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetLoseCount()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetMoney()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetAchievementBit()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetTitleBit()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetCharacterBit()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetFriendStringCont().size() << std::endl;

		outFile
			<< " " << valueBuffer.GetID()
			<< " " << valueBuffer.GetNickName()
			<< " " << valueBuffer.GetWinCount()
			<< " " << valueBuffer.GetLoseCount()
			<< " " << valueBuffer.GetMoney()
			<< " " << valueBuffer.GetAchievementBit()
			<< " " << valueBuffer.GetTitleBit()
			<< " " << valueBuffer.GetCharacterBit()
			<< " " << valueBuffer.GetFriendStringCont().size() << std::endl;

		// ģ�� ������ �ش� �̸� (����)
		auto iterBuffer = valueBuffer.GetFriendStringCont();

		for (auto iter = iterBuffer.begin();
			iter != iterBuffer.end();
			++iter)
		{
			outFile << " " << *iter << std::endl;
		}
	}

	// UserCont(MAP)���� �ش� ���� ����.
	userDataCont[pInSocketInfo->userDataContIndex].Delete(pInSocketInfo->pUserNode);
}

int UserDataManager::GetStringFirstChar(const char& InStringFirstChar)
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

void UserDataManager::SetGameResult(rbTreeNode<string, UserData>* InUserIter, const bool& InWinOrLose)
{
	//InUserIter->second.SetGameResult(InWinOrLose);
	InUserIter->SetValue().SetGameResult(InWinOrLose);
}