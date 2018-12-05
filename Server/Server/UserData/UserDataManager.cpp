#include "../UserData/UserDataManager.h"
#include "../IOCPServer/SocketInfo.h"

UserDataManager::UserDataManager() 
	:	CONST_A(converter.from_bytes("A"))
	,	CONST_Z(converter.from_bytes("Z"))
	,	CONST_a(converter.from_bytes("a"))
	,	CONST_z(converter.from_bytes("z"))
{
	userDataCont.reserve(37); // A ~ Z : 26 , 0 ~ 9 : 10 , . : 1 -> 36

	for(int count = 0 ; count < 37; count++)
		userDataCont.emplace_back();

	//[DEV_66] 0 ENGLISH, 1 KOREAN
	nicknameCont.reserve(2);

	nicknameCont.emplace_back();
	nicknameCont.emplace_back();

	string fileNameBuffer = "UserData/Saved/_NickName.txt";

	std::ifstream inFile(fileNameBuffer, std::ios::in);

	string idBuffer;
	string nicknameBuffer;
	wstring nicknameUnicode;

	while (!inFile.eof())
	{
		inFile >> idBuffer >> nicknameBuffer;
		nicknameUnicode = converter.from_bytes(nicknameBuffer);

		// ������ ���, 0�� �����̳�, �ѱ��� ��� 1�� �����̳ʿ� ������.
		if (nicknameUnicode[0] >= CONST_A[0] && nicknameUnicode[0] <= CONST_Z[0])
		{
			nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Insert(nicknameUnicode, idBuffer);
		}
		else if (nicknameUnicode[0] >= CONST_a[0] && nicknameUnicode[0] <= CONST_z[0])
		{
			nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Insert(nicknameUnicode, idBuffer);
		}
		else
		{
			nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Insert(nicknameUnicode, idBuffer);
		}
	}

	// IOCPServer �����ڿ��� UserDataManager�� �����ڰ� ȣ��Ǳ� ������, �г���Cont�� �ε尡 ��� ���� �� ������ ������ ���� �����.
}

UserDataManager::~UserDataManager()
{

}

int UserDataManager::LoginProcess(SocketInfo* pInSocketInfo, const string& InID, wstring& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
	int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<wstring>& RetFriendStringCont)
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

	// DEV_66 �г��� �����ڵ� ó��
	string nickNameBuffer{};

	// ���� ����
	std::ifstream inFile(fileNameBuffer, std::ios::in);

	// ���� ������ �ε�
	inFile 
		>> idStringBuffer 
		>> nickNameBuffer /* == RetNickName // DEV_66���� ���� ���� ��� */
		>> RetWinCount 
		>> RetLoseCount 
		>> RetMoney 
		>> RetAchievementBit 
		>> RetTitleBit 
		>> RetCharacterBit 
		>> friendNum;

	// [DEV_66] ��Ƽ����Ʈ���� �����ڵ� ��ȯ
	RetNickName = converter.from_bytes(nickNameBuffer);

	// �ش� ������ ���� ��� ����.
	if (RetMoney == -1)
	{
		pInSocketInfo->pUserNode = userDataCont[pInSocketInfo->userDataContIndex].Insert(InID, UserData(pInSocketInfo, InID));

		// RetMoney�� -1�� ���, Ŭ���Ʈ���� �г��� �Է� �ʿ�. (Login Scene ? // Main UI Scene ?)

		std::cout << "���� ������ �ε忡 �����Ͽ� ȸ�������߽��ϴ�. \n";
		return 0;
	}

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
		
		//DEV_66 ����� �ҷ����̴� ���� ��Ƽ����Ʈ��, �̰� ���ο����� ��������� �����ڵ��, 
		RetFriendStringCont.emplace_back(converter.from_bytes(idStringBuffer));

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
		// ģ�� ����� �̿�����, ģ�� ������ ������ ������ // �̰� �߸��ϸ� ���� ���� �Ű���!
		if (pInSocketInfo->pUserNode->SetValue().GetDemandFriendContIndex() != -1)
		{
			pInSocketInfo->pUserNode->SetValue().SetDeleteFriendID();
		}

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
			<< " " << keyBuffer //== valueBuffer.GetID()
			<< " " << converter.to_bytes(valueBuffer.GetNickName())	// [DEV_66]
			<< " " << valueBuffer.GetWinCount()
			<< " " << valueBuffer.GetLoseCount()
			<< " " << valueBuffer.GetMoney()
			<< " " << valueBuffer.GetAchievementBit()
			<< " " << valueBuffer.GetTitleBit()
			<< " " << valueBuffer.GetCharacterBit()
			<< " " << valueBuffer.GetFriendStringCont().size() 
			<< std::endl;

		// for-iter���� ���� ������, begin�ϰ� end�ϰ� �޸��ּ� �ٸ��ٰ� ������ ( called by Value )
		auto iterBuffer = valueBuffer.GetFriendStringCont();

		for (auto iter 
			= iterBuffer.begin()
			; iter != iterBuffer.end()
			; ++iter)
		{
			outFile << " " << converter.to_bytes(*iter) << std::endl;	// [DEV_66]
		}
	}

	// UserCont(MAP)���� �ش� ���� ����.
	userDataCont[pInSocketInfo->userDataContIndex].Delete(pInSocketInfo->pUserNode);
}

rbTreeNode<string, UserData>* UserDataManager::SearchUserNode(const string& keyString, bool& RetBool)
{
	return userDataCont[GetStringFirstChar(keyString[0])].Search(keyString, RetBool);
}

rbTreeNode<string, UserData>* UserDataManager::SearchUserNodeWithNickname(const wstring& KeyNickName, bool& RetBool)
{
	bool isReturnTrue{ false };
	string idBuffer{};

	// ������ ���, 0�� �����̳�, �ѱ��� ��� 1�� �����̳ʿ� ������.
	if (KeyNickName[0] >= CONST_A[0] && KeyNickName[0] <= CONST_Z[0])
	{
		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Search(KeyNickName, isReturnTrue)->SetValue();
	}
	else if (KeyNickName[0] >= CONST_a[0] && KeyNickName[0] <= CONST_z[0])
	{
		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Search(KeyNickName, isReturnTrue)->SetValue();
	}
	else
	{
		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Search(KeyNickName, isReturnTrue)->SetValue();
	}

	// ��ã��.
	if (!isReturnTrue)
	{
		RetBool = false;
		return nullptr;
	}

	return SearchUserNode(idBuffer, RetBool);
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