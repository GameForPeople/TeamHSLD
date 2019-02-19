#include "../UserData/UserDataManager.h"
#include "../IOCPServer/SocketInfo.h"
#include "../IOCPServer/UDPManager.h"

UserDataManager::UserDataManager() noexcept
	: nicknameContFileName("UserData/Saved/_Nickname.txt")
	, userNum(0)
	, outFile(nicknameContFileName, std::ios::app)
{
	userDataCont.reserve(37); // A ~ Z : 26 , 0 ~ 9 : 10 , . : 1 -> 36

	for(int count = 0 ; count < 37; count++)
		userDataCont.emplace_back();

	//[DEV_66] 0 ENGLISH, 1 KOREAN, 2 ILLEGAL
	nicknameCont.reserve(3);
	nicknameCont.emplace_back();
	nicknameCont.emplace_back();
	nicknameCont.emplace_back();

	//saveFileCont.reserve(10);

	std::ifstream inFile(nicknameContFileName, std::ios::in);

	Type_ID idBuffer;
	Type_Nickname NicknameBuffer;
	int activeCharacterIndexBuffer;

	// IOCPServer �����ڿ��� UserDataManager�� �����ڰ� ȣ��Ǳ� ������, �г���Cont�� �ε尡 ��� ���� �� ������ ������ ���� �����.
	while (!inFile.eof())
	{
		inFile >> idBuffer >> NicknameBuffer >> activeCharacterIndexBuffer;

		// ������ ���, 0�� �����̳�, �ѱ��� ��� 1�� �����̳ʿ� ������.
		if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(NicknameBuffer.data()), 0)
			; firstCharType == _MBC_SINGLE)
		{
			nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Insert(NicknameBuffer, NicknameNode(idBuffer, activeCharacterIndexBuffer));
			//std::cout << "�����Դϴ�. \n";
		}
		else if (firstCharType == _MBC_LEAD)
		{
			nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Insert(NicknameBuffer, NicknameNode(idBuffer, activeCharacterIndexBuffer));
			//std::cout << "�Ƹ� �ѱ��ϲ��ϴ�. \n";
		}
		else  //  if (firstCharType == _MBC_ILLEGAL)
		{
			std::cout << "_MBC_ILLEGAL �Դϴ�. .. �� �׷�������? \n";
			nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Insert(NicknameBuffer, NicknameNode(idBuffer, activeCharacterIndexBuffer));
		}
	}

	::InitializeCriticalSection(&csNicknameCont);
	::InitializeCriticalSection(&csSaveNickname);

	// ���� ������ ����.
	//outFile << std::endl;
}

UserDataManager::~UserDataManager()
{
	//if (saveFileCont.size())
	//{
	//	_SaveNicknameCont();
	//}

	::DeleteCriticalSection(&csNicknameCont);
	::DeleteCriticalSection(&csSaveNickname);
}

/*
	LoginProcess
     - �α����� �õ��ϴ� �Լ��Դϴ�.
	���ڷδ� �α��� ���� ��, Ŭ���̾�Ʈ�� �ʿ��� �� �ִ� �����͸� �����ϱ� ���� ���۷��� ���ڷ� �����մϴ�.

	return = 0 -> �������� ���μ���.
			 1, 2 -> �������¿�����, ��ȹ �������� ���� ����.
			 3 -> �̹� �α����� ����(������ �� ������ ����)
*/
int UserDataManager::LoginProcess(SocketInfo* pInSocketInfo, const Type_ID& InID, Type_Nickname& RetNickname, int& RetWinCount, int& RetLoseCount, int& RetMoney,
	int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, BYTE& RetActiveCharacterIndex ,vector<Type_Nickname>& RetFriendNicknameCont)
{
	int userDataContIndex = GetStringFirstChar(InID[0]);

	bool RetBoolBuffer;
	
	userDataCont[userDataContIndex].Search(InID, RetBoolBuffer);

	// �̹� �α��� ���� üũ.
	if (RetBoolBuffer)
	{
		return 3; 
	}

	// ���� �� ����
	++userNum;

	// �������� ��
	RetMoney = -1; // ��ȭ�� -1�ϸ� ����. �ش��Ȳ��, ���� ���� ���θ� Ȯ���ϱ� ���� ����.
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

	// ���� ����
	std::ifstream inFile(fileNameBuffer, std::ios::in);

	// [DEV_78] int �ϳ� ���(���� �����ʹ� int�� �����ִ°� �����ϱ� ����.
	int activeCharacterIndexBuffer{};

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
		>> activeCharacterIndexBuffer
		>> friendNum;

	RetActiveCharacterIndex = static_cast<BYTE>(activeCharacterIndexBuffer);

	//std::cout << "[DEBUG] - 1" << std::endl;

	// �ش� ������ ���� ��� ����.
	if (RetMoney == -1)
	{
		pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID);
		/*pInSocketInfo->pUserNode = */ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);

		// RetMoney�� -1�� ���, Ŭ���Ʈ���� �г��� �Է� �ʿ�. (Login Scene ? // Main UI Scene ?)
		// std::cout << "���� ������ �ε忡 �����Ͽ� ȸ�������߽��ϴ�. \n";
		return 0;
	}

	//std::cout << "[DEBUG] - 2" << std::endl;

	//ģ�� ����. ���� �α���.
	if (friendNum == 0)
	{
		pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickname,
			RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetActiveCharacterIndex);
		
		userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
		return 0;
	}

	//std::cout << "[DEBUG] - 3" << std::endl;

	//std::cout << "ģ�� ���� :" << friendNum << std::endl;
	// ģ�� ���� ���� ���� reserve.
	RetFriendNicknameCont.reserve(friendNum); // �ش� ���� reserve.

	//std::cout << "[DEBUG] - 4" << std::endl;

	// ģ�� �����͸�, ����.
	for (int iter = 0; iter < friendNum; ++iter) // idBuffer�� ���۷��ؼ� ���.
	{
		inFile >> idStringBuffer;
		
		RetFriendNicknameCont.emplace_back(/*converter.from_bytes(*/idStringBuffer/*)*/);

		// ģ���� �α��� ������ �ƴ����� üũ��, ���⼭ �̷������ �ȵǰ�, ģ�� UI�� On�� ��� �ؾ���. -> ¥�� �ϵ忡�� Load�� �ʿ� ���� ��������.
		//if (userDataCont[userDataContHashValueBuffer].find(idStringBuffer) == userDataCont[userDataContHashValueBuffer].end())
	}

	//std::cout << "[DEBUG] - 5" << std::endl;

	// �������� �α���.
	pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickname,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetActiveCharacterIndex, RetFriendNicknameCont);

	/*pInSocketInfo->pUserNode =*/ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);

	return 0;
}

/*
	LogoutProcess
	 - �α׾ƿ��� ó���ϴ� �Լ��Դϴ�.
	 - �ش� ���� �����͸� ���Ϸ� �����մϴ�.
*/
void UserDataManager::LogoutProcess(shared_ptr<UserData> pUserNode)
{
	userNum--;

	if (pUserNode->GetNickname().size() == 0)
	{
		// �� �̰Ŵ� �г��� ���°ž� �������� �� 
	}
	else
	{
		// ģ�� ����� �̿�����, ģ�� ������ ������ ������ // �̰� �߸��ϸ� ���� ���� �Ű���!
		//if (pUserNode->GetDemandFriendContIndex() != -1)
		//{
			pUserNode->SetDeleteFriendID();
		//}

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
			<< " " << pUserNode->GetItemBit()
			<< " " << pUserNode->GetCharacterBit()
			<< " " << static_cast<int>(pUserNode->GetActiveCharacterIndex())
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

/*
	SearchUserNode
		- ������带 ID�� ���� UserDataCont���� Ž���մϴ�.
*/
auto UserDataManager::SearchUserNode(const Type_ID& keyID, bool& RetBool) -> shared_ptr<UserData>
{
	return userDataCont[GetStringFirstChar(keyID[0])].Search(keyID, RetBool);
}

/*
	SearchUserNodeByNickname
		- �г����� ���� �г��� �����̳ʿ��� �� ������ ID�� Ž���ϰ�, �ش� ������ ID�� ���Ͽ� UserDataCont���� Ž���մϴ�. (SearchUserNode)

*/
shared_ptr<UserData> UserDataManager::SearchUserNodeByNickname(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch)
{
	bool isReturnTrue{ false };
	Type_ID idBuffer{};

	//std::cout << "[FRIEND] �г����� ã���ϴ�. " << std::endl;

	// ������ ���, 0�� �����̳�, �ѱ��� ��� 1�� �����̳ʿ� ������.
	if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(KeyNickname.data()), 0)
		; firstCharType == _MBC_SINGLE)
	{
		//std::cout << "[FRIEND] �г����� MBC_Single : �����Դϴ�. " << std::endl;

		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Search(KeyNickname, isReturnTrue)->SetValue().GetID();
	}
	else if (firstCharType == _MBC_LEAD)
	{
		//std::cout << "[FRIEND]�г����� MBC_LEAD(�����Ʈ) : �ѱ��Դϴ�. " << std::endl;

		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Search(KeyNickname, isReturnTrue)->SetValue().GetID();
	}
	else //  if (firstCharType == _MBC_ILLEGAL)
	{
#ifdef _DEBUG_MODE_
		std::cout << "[FRIEND]�г����� �̻��� �����Դϴ�. " << std::endl;
#endif
		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Search(KeyNickname, isReturnTrue)->SetValue().GetID();
	}

	// ��ã��.
	if (!isReturnTrue)
	{
		//std::cout << "[FRIEND] �׷� �г����� ������ �����. " << std::endl;

		RetIsMatch = false;
		RetIsOnLogin = false;
		return CONST_NULL_USERDATA;
	}

	//std::cout << "[FRIEND] �ش� �г����� ���� ������ �г��� �����̳ʿ��� ã�ҽ��ϴ�. UserData���� Ž���մϴ�. " << std::endl;

	RetIsMatch = true;
	return SearchUserNode(idBuffer, RetIsOnLogin);
}

/*
	SearchUserNodeByNickname
		- �г����� ���� �г��� �����̳ʿ��� �� ������ ID�� Ž���ϰ�, �ش� ������ ID�� ���Ͽ� UserDataCont���� Ž���մϴ�. (SearchUserNode)
*/
shared_ptr<UserData> UserDataManager::SearchUserNodeByNicknameWithActiveCharacterIndex(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch, BYTE& RetActiveCharacterIndex)
{
	bool isReturnTrue{ false };
	Type_ID idBuffer{};

	//std::cout << "[FRIEND] �г����� ã���ϴ�. " << std::endl;

	// ������ ���, 0�� �����̳�, �ѱ��� ��� 1�� �����̳ʿ� ������.
	if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(KeyNickname.data()), 0)
		; firstCharType == _MBC_SINGLE)
	{
		//std::cout << "[FRIEND] �г����� MBC_Single : �����Դϴ�. " << std::endl;

		auto retBuffer = nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Search(KeyNickname, isReturnTrue);
		idBuffer = retBuffer->SetValue().GetID();
		RetActiveCharacterIndex = retBuffer->SetValue().GetActiveCharacterIndex();
	}
	else if (firstCharType == _MBC_LEAD)
	{
		//std::cout << "[FRIEND]�г����� MBC_LEAD(�����Ʈ) : �ѱ��Դϴ�. " << std::endl;

		auto retBuffer = nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Search(KeyNickname, isReturnTrue);
		idBuffer = retBuffer->SetValue().GetID();
		RetActiveCharacterIndex = retBuffer->SetValue().GetActiveCharacterIndex();
	}
	else //  if (firstCharType == _MBC_ILLEGAL)
	{
		//std::cout << "[FRIEND]�г����� �̻��� �����Դϴ�. " << std::endl;

		auto retBuffer = nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Search(KeyNickname, isReturnTrue);
		idBuffer = retBuffer->SetValue().GetID();
		RetActiveCharacterIndex = retBuffer->SetValue().GetActiveCharacterIndex();
	}

	// ��ã��.
	if (!isReturnTrue)
	{
		//std::cout << "[FRIEND] �׷� �г����� ������ �����. " << std::endl;

		RetIsMatch = false;
		RetIsOnLogin = false;
		return CONST_NULL_USERDATA;
	}

	//std::cout << "[FRIEND] �ش� �г����� ���� ������ �г��� �����̳ʿ��� ã�ҽ��ϴ�. UserData���� Ž���մϴ�. " << std::endl;

	RetIsMatch = true;
	return SearchUserNode(idBuffer, RetIsOnLogin);
}


/*
	GetActiveCharacterIndexWithNickname
	 - �г����� �������� ���, �ش� �г��ӿ� ���� ActiveCharacterIndex�� �����մϴ�.

	 Friend�� Off�� ���, �ش� ģ���� �Ϸ���Ʈ�� Ȯ���ϱ� ���� ���˴ϴ�.
*/
std::pair<char, bool> UserDataManager::GetActiveCharacterIndexWithNickname(Type_Nickname& KeyNickname)
{
	std::pair<char, bool> retNode = std::make_pair(0,false);

	if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(KeyNickname.data()), 0)
		; firstCharType == _MBC_SINGLE)
	{
		retNode.first = nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Search(KeyNickname, retNode.second)->SetValue().GetActiveCharacterIndex();
	}
	else if (firstCharType == _MBC_LEAD)
	{
		retNode.first = nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Search(KeyNickname, retNode.second)->SetValue().GetActiveCharacterIndex();
	}
	else //  if (firstCharType == _MBC_ILLEGAL)
	{
		retNode.first = nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Search(KeyNickname, retNode.second)->SetValue().GetActiveCharacterIndex();
	}

	return retNode;
}

/*
	SetActiveCharacterIndexWithNickname
	- �г��Ӱ� �ٲ� ĳ���� �ε����� �����ް�, �ش� �г��ӿ� ���� ActiveCharacterIndex�� �����մϴ�.
	
	�÷��̾, �ε��� ���� ��, ȣ��˴ϴ�.
*/
void UserDataManager::SetActiveCharacterIndexWithNickname(Type_Nickname& KeyNickname, char InNewIndex)
{
	bool returnPointerIsNotNull{ false };
	CUSTOM_MAP::rbTreeNode<Type_Nickname, NicknameNode>* retNodeBuffer{ nullptr };

	if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(KeyNickname.data()), 0)
		; firstCharType == _MBC_SINGLE)
	{
		retNodeBuffer = nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Search(KeyNickname, returnPointerIsNotNull);
	}
	else if (firstCharType == _MBC_LEAD)
	{
		retNodeBuffer = nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Search(KeyNickname, returnPointerIsNotNull);
	}
	else //  if (firstCharType == _MBC_ILLEGAL)
	{
		retNodeBuffer = nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Search(KeyNickname, returnPointerIsNotNull);
	}

	if (returnPointerIsNotNull)
		retNodeBuffer->SetValue().SetActiveCharacterIndex(InNewIndex);
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
		return static_cast<int>(LANGUAGE::ENGLISH);
	}
	else if (firstCharType == _MBC_LEAD)
	{
		return static_cast<int>(LANGUAGE::KOREAN);
	}
	else
	{
		return static_cast<int>(LANGUAGE::OTHERS);
	}
}

bool UserDataManager::SetNewNickname(SocketInfo* pInClient, const Type_Nickname& InNewString)
{
	bool isOnLogin{ false };
	bool isOnMatch{ false };

	::EnterCriticalSection(&csNicknameCont);
	SearchUserNodeByNickname(InNewString, isOnLogin, isOnMatch);

	if (!isOnMatch)
	{
		nicknameCont[GetNicknameFirstChar(InNewString)].Insert(InNewString, NicknameNode(pInClient->pUserNode->GetID(), 0));
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

void UserDataManager::TraversalForAnnouncement(UDPManager* const pInUDPManager)
{
	for (int vectorIter = 0; vectorIter < userDataCont.size(); ++vectorIter)
	{
		userDataCont[vectorIter].TraversalForAnnouncement(pInUDPManager, userDataCont[vectorIter].GetRootNode());
	}
}

/*
	SaveNicknameContProcess
		- �ű԰����ڸ� �׻� �����մϴ�.
*/
void UserDataManager::SaveNicknameContProcess(const Type_ID& InID, const Type_Nickname& InNickname)
{
	::EnterCriticalSection(&csSaveNickname);
	//saveFileCont.emplace_back(make_pair(InID, InNickname));

	outFile
		<< " " << InID
		<< " " << InNickname
		<< " " << 0
		<< std::endl;

#ifdef _DEBUG_MODE_

#endif

	::LeaveCriticalSection(&csSaveNickname);
	//if (saveFileCont.size() < 10)
	//{
	//	::LeaveCriticalSection(&csSaveNickname);
	//	return;
	//}
	//else
	//{
	//	_SaveNicknameCont();
	//	::LeaveCriticalSection(&csSaveNickname);
	//	return;
	//}
}

//void UserDataManager::_SaveNicknameCont()
//{
//	std::ofstream outFile(nicknameContFileName, std::ios::app);
//
//	for (auto iter = saveFileCont.begin()
//		; iter != saveFileCont.end()
//		; ++iter)
//	{
//		outFile
//			<< " " << iter->first
//			<< " " << iter->second
//			<< " " << 0 
//			<< std::endl;
//	}
//
//	saveFileCont.clear();
//	saveFileCont.reserve(10);
//}

