#include "../UserData/UserDataManager.h"
#include "../IOCPServer/SocketInfo.h"

UserDataManager::UserDataManager() noexcept
	: nicknameContFileName("UserData/Saved/_Nickname.txt")
{
	userDataCont.reserve(37); // A ~ Z : 26 , 0 ~ 9 : 10 , . : 1 -> 36

	for(int count = 0 ; count < 37; count++)
		userDataCont.emplace_back();

	//[DEV_66] 0 ENGLISH, 1 KOREAN, 2 ILLEGAL
	nicknameCont.reserve(3);
	saveFileCont.reserve(10);

	nicknameCont.emplace_back();
	nicknameCont.emplace_back();
	nicknameCont.emplace_back();

	std::ifstream inFile(nicknameContFileName, std::ios::in);

	Type_ID idBuffer;
	Type_Nickname NicknameBuffer;

	// IOCPServer �����ڿ��� UserDataManager�� �����ڰ� ȣ��Ǳ� ������, �г���Cont�� �ε尡 ��� ���� �� ������ ������ ���� �����.
	while (!inFile.eof())
	{
		inFile >> idBuffer >> NicknameBuffer;

		// ������ ���, 0�� �����̳�, �ѱ��� ��� 1�� �����̳ʿ� ������.
		if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(NicknameBuffer.data()), 0)
			; firstCharType == _MBC_SINGLE)
		{
			nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Insert(NicknameBuffer, idBuffer);
			//std::cout << "�����Դϴ�. \n";
		}
		else if (firstCharType == _MBC_LEAD)
		{
			nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Insert(NicknameBuffer, idBuffer);
			//std::cout << "�Ƹ� �ѱ��ϲ��ϴ�. \n";
		}
		else  //  if (firstCharType == _MBC_ILLEGAL)
		{
			std::cout << "_MBC_ILLEGAL �Դϴ�. \n";
			nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Insert(NicknameBuffer, idBuffer);
		}
	}

	::InitializeCriticalSection(&csNicknameCont);
	::InitializeCriticalSection(&csSaveNickname);
}

UserDataManager::~UserDataManager()
{
	if (saveFileCont.size())
	{
		_SaveNicknameCont();
	}

	::DeleteCriticalSection(&csNicknameCont);
	::DeleteCriticalSection(&csSaveNickname);
}

int UserDataManager::LoginProcess(SocketInfo* pInSocketInfo, const Type_ID& InID, Type_Nickname& RetNickname, int& RetWinCount, int& RetLoseCount, int& RetMoney,
	int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<Type_Nickname>& RetFriendNicknameCont)
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
	Type_ID idStringBuffer;			// idBuffer

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
	//string NicknameBuffer{};

	// ���� ����
	std::ifstream inFile(fileNameBuffer, std::ios::in);

	// ���� ������ �ε�
	inFile 
		>> idStringBuffer 
		>> RetNickname /* == RetNickname // DEV_66���� ���� ���� ��� */
		>> RetWinCount 
		>> RetLoseCount 
		>> RetMoney 
		>> RetAchievementBit 
		>> RetTitleBit 
		>> RetCharacterBit 
		>> friendNum;

	// [DEV_66] ��Ƽ����Ʈ���� �����ڵ� ��ȯ
	//RetNickname = converter.from_bytes(NicknameBuffer);

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
		pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickname,
			RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit);
		
		userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
		return 0;
	}

	// ģ�� ���� ���� ���� reserve.
	RetFriendNicknameCont.reserve(friendNum); // �ش� ���� reserve.

	// ģ�� �����͸�, ����.
	for (int iter = 0; iter < friendNum; ++iter) // idBuffer�� ���۷��ؼ� ���.
	{
		inFile >> idStringBuffer;
		
		//DEV_66 ����� �ҷ����̴� ���� ��Ƽ����Ʈ��, �̰� ���ο����� ��������� �����ڵ��, 
		RetFriendNicknameCont.emplace_back(/*converter.from_bytes(*/idStringBuffer/*)*/);

		// ģ���� �α��� ������ �ƴ����� üũ��, ���⼭ �̷������ �ȵǰ�, ģ�� UI�� On�� ��� �ؾ���. -> ¥�� �ϵ忡�� Load�� �ʿ� ���� ��������.
		//if (userDataCont[userDataContHashValueBuffer].find(idStringBuffer) == userDataCont[userDataContHashValueBuffer].end())
	}

	// �������� �α���.
	pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickname,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetFriendNicknameCont);

	/*pInSocketInfo->pUserNode =*/ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
	return 0;
}

void UserDataManager::LogoutProcess(shared_ptr<UserData> pUserNode)
{
	if (pUserNode->GetNickname().size() == 0)
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
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetNickname()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetWinCount()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetLoseCount()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetMoney()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetAchievementBit()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetTitleBit()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetCharacterBit()
		//	<< " " << pInSocketInfo->pUserNode->GetValue().GetFriendStringCont().size() << std::endl;

		outFile
			<< " " << keyBuffer //== valueBuffer.GetID()
			<< " " << pUserNode->GetNickname()	// [DEV_66]
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
			outFile << " " << /*converter.to_bytes(*iter)*/ *iter << std::endl;	// [DEV_66]
		}
	}

	// UserCont(MAP)���� �ش� ���� ����.
	userDataCont[GetStringFirstChar(pUserNode->GetID()[0])].DeleteWithSearch(pUserNode->GetKey());
}

shared_ptr<UserData> UserDataManager::SearchUserNode(const Type_ID& keyID, bool& RetBool)
{
	return userDataCont[GetStringFirstChar(keyID[0])].Search(keyID, RetBool);
}

shared_ptr<UserData> UserDataManager::SearchUserNodeWithNickname(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch)
{
	bool isReturnTrue{ false };
	Type_ID idBuffer{};

	std::cout << "[FRIEND] DEBUG 0 - 1 " << std::endl;

	// ������ ���, 0�� �����̳�, �ѱ��� ��� 1�� �����̳ʿ� ������.
	if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(KeyNickname.data()), 0)
		; firstCharType == _MBC_SINGLE)
	{
		std::cout << "[FRIEND] DEBUG 0 - 2 " << std::endl;

		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Search(KeyNickname, isReturnTrue)->SetValue();
	}
	else if (firstCharType == _MBC_LEAD)
	{
		std::cout << "[FRIEND] DEBUG 0 - 3 " << std::endl;

		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Search(KeyNickname, isReturnTrue)->SetValue();
	}
	else //  if (firstCharType == _MBC_ILLEGAL)
	{
		std::cout << "[FRIEND] DEBUG 0 - 4 " << std::endl;

		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Search(KeyNickname, isReturnTrue)->SetValue();
	}

	// ��ã��.
	if (!isReturnTrue)
	{
		std::cout << "[FRIEND] DEBUG 0 - 5 " << std::endl;

		RetIsMatch = false;
		RetIsOnLogin = false;
		return CONST_NULL_USERDATA;
	}

	std::cout << "[FRIEND] DEBUG 0 - 6 " << std::endl;

	RetIsMatch = true;
	return SearchUserNode(idBuffer, RetIsOnLogin);
}

int UserDataManager::GetStringFirstChar(const char InStringFirstChar) const noexcept
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

int UserDataManager::GetNicknameFirstChar(const Type_Nickname& InKeyNickname) const noexcept
{
	if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(InKeyNickname.data()), 0)
		; firstCharType == _MBC_SINGLE)
	{
		return 0;
	}
	else if (firstCharType == _MBC_LEAD)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

bool UserDataManager::SetNewNickname(SocketInfo* pInClient, const Type_Nickname& InNewString)
{
	bool isOnLogin{ false };
	bool isOnMatch{ false };

	::EnterCriticalSection(&csNicknameCont);
	SearchUserNodeWithNickname(InNewString, isOnLogin, isOnMatch);

	if (!isOnMatch)
	{
		nicknameCont[GetNicknameFirstChar(InNewString)].Insert(InNewString, pInClient->pUserNode->GetID());
		::LeaveCriticalSection(&csNicknameCont);

		SaveNicknameContProcess(pInClient->pUserNode->GetID(), InNewString);
		pInClient->pUserNode->SetNickname(InNewString);
		return true;
	}
	else
	{
		::LeaveCriticalSection(&csNicknameCont);
		return false;
	}
}

void UserDataManager::SaveNicknameContProcess(const Type_ID& InID, const Type_Nickname& InNickname)
{
	::EnterCriticalSection(&csSaveNickname);
	saveFileCont.emplace_back(make_pair(InID, InNickname));

	if (saveFileCont.size() < 10)
	{
		::LeaveCriticalSection(&csSaveNickname);
		return;
	}
	else
	{
		_SaveNicknameCont();
		::LeaveCriticalSection(&csSaveNickname);
		return;
	}
}

void UserDataManager::_SaveNicknameCont()
{
	std::ofstream outFile(nicknameContFileName, std::ios::app);

	for (auto iter = saveFileCont.begin()
		; iter != saveFileCont.end()
		; ++iter)
	{
		outFile
			<< " " << iter->first
			<< " " << iter->second
			<< std::endl;
	}

	saveFileCont.clear();
	saveFileCont.reserve(10);
}

