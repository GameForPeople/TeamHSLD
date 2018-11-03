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

int UserDataManager::LoginProcess(SocketInfo* InPSocketInfo, const string& InID, string& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
	int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<string>& RetFriendStringCont)
{
	//RetFailReson = 0;
	// ���� ������ 
	InPSocketInfo->userDataContIndex = GetStringFirstChar(InID[0]);

	bool RetBoolBuffer;
	
	//InPSocketInfo->pUserNode = 
		userDataCont[InPSocketInfo->userDataContIndex].Search(InID, RetBoolBuffer);

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
		InPSocketInfo->pUserNode = userDataCont[InPSocketInfo->userDataContIndex].Insert(InID, UserData(InPSocketInfo, InID));

		std::cout << "���� ������ �ε忡 �����Ͽ� ȸ�������߽��ϴ�. \n";
		return 0;
	}

	std::cout << "�ε��� ������ �г����� : " << RetNickName << "�Դϴ�. \n";

	//ģ�� ����. ���� �α���.
	if (friendNum == 0)
	{
		InPSocketInfo->pUserNode = userDataCont[InPSocketInfo->userDataContIndex].Insert(InID, UserData(InPSocketInfo, InID, RetNickName,
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
	InPSocketInfo->pUserNode = userDataCont[InPSocketInfo->userDataContIndex].Insert(InID, UserData(InPSocketInfo, InID, RetNickName,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetFriendStringCont));

	return 0;
}

void UserDataManager::LogoutProcess(SocketInfo* InPSocketInfo)
{
	// ���ϸ� ����
	string fileNameBuffer = "UserData/Saved/.txt";
	fileNameBuffer.insert(fileNameBuffer.begin() + 15, InPSocketInfo->pUserNode->GetKey().begin(), InPSocketInfo->pUserNode->GetKey().end());

	// ���� ���� ���� ����
	std::ofstream outFile(fileNameBuffer, std::ios::out);
	
	// ���� ���� (����)
	outFile
		<< " " << InPSocketInfo->pUserNode->GetValue().GetID()
		<< " " << InPSocketInfo->pUserNode->GetValue().GetNickName()
		<< " " << InPSocketInfo->pUserNode->GetValue().GetWinCount()
		<< " " << InPSocketInfo->pUserNode->GetValue().GetLoseCount()
		<< " " << InPSocketInfo->pUserNode->GetValue().GetMoney()
		<< " " << InPSocketInfo->pUserNode->GetValue().GetAchievementBit()
		<< " " << InPSocketInfo->pUserNode->GetValue().GetTitleBit()
		<< " " << InPSocketInfo->pUserNode->GetValue().GetCharacterBit()
		<< " " << InPSocketInfo->pUserNode->GetValue().GetFriendStringCont().size() << std::endl;

	// ģ�� ������ �ش� �̸� (����)
	for (auto iter = InPSocketInfo->pUserNode->GetValue().GetFriendStringCont().begin();
		iter != InPSocketInfo->pUserNode->GetValue().GetFriendStringCont().end();
		++iter)
	{
		outFile << " " << *iter << std::endl;
	}

	// UserCont(MAP)���� �ش� ���� ����.
	userDataCont[InPSocketInfo->userDataContIndex].Delete(InPSocketInfo->pUserNode);
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
	InUserIter->GetValue().SetGameResult(InWinOrLose);
}