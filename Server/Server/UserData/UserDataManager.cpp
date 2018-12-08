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
	// ���� ������ ��ȣ
	int userDataContIndex = GetStringFirstChar(InID[0]);

	bool RetBoolBuffer;
	
	//pInSocketInfo->pUserNode = 
		userDataCont[userDataContIndex].Search(InID, RetBoolBuffer);
	//pInSocketInfo->pUserNode = nullptr;	//? �̰� ���� ���̾�?

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
		pInSocketInfo->pUserNode = make_shared<UserData>();
		/*pInSocketInfo->pUserNode = */ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);

		// RetMoney�� -1�� ���, Ŭ���Ʈ���� �г��� �Է� �ʿ�. (Login Scene ? // Main UI Scene ?)
		std::cout << "���� ������ �ε忡 �����Ͽ� ȸ�������߽��ϴ�. \n";
		return 0;
	}

	//ģ�� ����. ���� �α���.
	if (friendNum == 0)
	{
		pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickName,
			RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit);
		
		userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
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
	pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickName,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetFriendStringCont);

	/*pInSocketInfo->pUserNode =*/ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
	return 0;
}

void UserDataManager::LogoutProcess(shared_ptr<UserData>& pUserNode)
{
	if (pUserNode->GetNickName().size() == 0)
	{
		// �� �̰Ŵ� �г��� ���°ž� �������� �� 
	}
	else
	{
		// ģ�� ����� �̿�����, ģ�� ������ ������ ������ // �̰� �߸��ϸ� ���� ���� �Ű���!
		if (pUserNode->GetDemandFriendContIndex() != -1)
		{
			pUserNode->SetDeleteFriendID();
		}

		// ���ϸ� ����
		string fileNameBuffer = "UserData/Saved/.txt";
		string keyBuffer = pUserNode->GetKey();

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
			<< " " << converter.to_bytes(pUserNode->GetNickName())	// [DEV_66]
			<< " " << pUserNode->GetWinCount()
			<< " " << pUserNode->GetLoseCount()
			<< " " << pUserNode->GetMoney()
			<< " " << pUserNode->GetAchievementBit()
			<< " " << pUserNode->GetTitleBit()
			<< " " << pUserNode->GetCharacterBit()
			<< " " << pUserNode->GetFriendNicknameCont().size() 
			<< std::endl;

		// for-iter���� ���� ������, begin�ϰ� end�ϰ� �޸��ּ� �ٸ��ٰ� ������ ( called by Value )
		auto iterBuffer = pUserNode->GetFriendNicknameCont();

		for (auto iter 
			= iterBuffer.begin()
			; iter != iterBuffer.end()
			; ++iter)
		{
			outFile << " " << converter.to_bytes(*iter) << std::endl;	// [DEV_66]
		}
	}

	// UserCont(MAP)���� �ش� ���� ����.
	userDataCont[GetStringFirstChar(pUserNode->GetID()[0])].DeleteWithSearch(pUserNode->GetKey());
}

shared_ptr<UserData> UserDataManager::SearchUserNode(const string& keyString, bool& RetBool)
{
	return userDataCont[GetStringFirstChar(keyString[0])].Search(keyString, RetBool);
}

shared_ptr<UserData> UserDataManager::SearchUserNodeWithNickname(const wstring& KeyNickName, bool& RetIsOnLogin, bool& RetIsMatch)
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
		RetIsMatch = false;
		RetIsOnLogin = false;
		return CONST_NULL_USERDATA;
	}

	RetIsMatch = true;
	return SearchUserNode(idBuffer, RetIsOnLogin);
}

int UserDataManager::GetStringFirstChar(const char& InStringFirstChar) noexcept
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
