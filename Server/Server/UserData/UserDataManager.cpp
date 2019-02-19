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

	// IOCPServer 생성자에서 UserDataManager의 생성자가 호출되기 때문에, 닉네임Cont의 로드가 모두 끝난 후 서버가 켜지는 것이 보장됨.
	while (!inFile.eof())
	{
		inFile >> idBuffer >> NicknameBuffer >> activeCharacterIndexBuffer;

		// 영어일 경우, 0번 컨테이너, 한글일 경우 1번 컨테이너에 삽입함.
		if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(NicknameBuffer.data()), 0)
			; firstCharType == _MBC_SINGLE)
		{
			nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Insert(NicknameBuffer, NicknameNode(idBuffer, activeCharacterIndexBuffer));
			//std::cout << "영어입니다. \n";
		}
		else if (firstCharType == _MBC_LEAD)
		{
			nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Insert(NicknameBuffer, NicknameNode(idBuffer, activeCharacterIndexBuffer));
			//std::cout << "아마 한글일껍니다. \n";
		}
		else  //  if (firstCharType == _MBC_ILLEGAL)
		{
			std::cout << "_MBC_ILLEGAL 입니다. .. 엥 그럴리가요? \n";
			nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Insert(NicknameBuffer, NicknameNode(idBuffer, activeCharacterIndexBuffer));
		}
	}

	::InitializeCriticalSection(&csNicknameCont);
	::InitializeCriticalSection(&csSaveNickname);

	// 파일 포인터 개행.
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
     - 로그인을 시도하는 함수입니다.
	인자로는 로그인 성공 시, 클라이언트가 필요할 수 있는 데이터를 제공하기 위해 레퍼런스 인자로 제공합니다.

	return = 0 -> 정상적인 프로세싱.
			 1, 2 -> 에러상태였으나, 기획 변경으로 인한 삭제.
			 3 -> 이미 로그인한 상태(실제로 잘 사용되지 않음)
*/
int UserDataManager::LoginProcess(SocketInfo* pInSocketInfo, const Type_ID& InID, Type_Nickname& RetNickname, int& RetWinCount, int& RetLoseCount, int& RetMoney,
	int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, BYTE& RetActiveCharacterIndex ,vector<Type_Nickname>& RetFriendNicknameCont)
{
	int userDataContIndex = GetStringFirstChar(InID[0]);

	bool RetBoolBuffer;
	
	userDataCont[userDataContIndex].Search(InID, RetBoolBuffer);

	// 이미 로그인 여부 체크.
	if (RetBoolBuffer)
	{
		return 3; 
	}

	// 유저 수 증가
	++userNum;

	// 변수선언 부
	RetMoney = -1; // 재화가 -1일리 없음. 해당상황은, 파일 오픈 여부를 확인하기 위해 사용됨.
	int friendNum;			// (친구 명수버퍼)
	Type_ID idStringBuffer;			// idBuffer

	// 시간 정보.
	//time_t timeBuffer = time(0);   // get time now
	//tm* now = std::localtime(&timeBuffer);
	//(now->tm_year + 1900)
	//(now->tm_mon + 1)
	//now->tm_mday

	// 파일명 제작
	string fileNameBuffer = "UserData/Saved/.txt";
	fileNameBuffer.insert(fileNameBuffer.begin() + 15, InID.begin(), InID.end());
	//fileNameBuffer[15] = InID[0];
	//fileNameBuffer[16] = InID[1];

	// 파일 오픈
	std::ifstream inFile(fileNameBuffer, std::ios::in);

	// [DEV_78] int 하나 사용(파일 데이터는 int로 갖고있는게 관리하기 쉬움.
	int activeCharacterIndexBuffer{};

	// 파일 데이터 로드
	inFile 
		>> idStringBuffer 
		>> RetNickname /* == RetNickname // DEV_66으로 인해 버퍼 사용 */
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

	// 해당 파일이 없을 경우 리턴.
	if (RetMoney == -1)
	{
		pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID);
		/*pInSocketInfo->pUserNode = */ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);

		// RetMoney가 -1일 경우, 클라언트에서 닉네임 입력 필요. (Login Scene ? // Main UI Scene ?)
		// std::cout << "유저 데이터 로드에 실패하여 회원가입했습니다. \n";
		return 0;
	}

	//std::cout << "[DEBUG] - 2" << std::endl;

	//친구 없음. 정상 로그인.
	if (friendNum == 0)
	{
		pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickname,
			RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetActiveCharacterIndex);
		
		userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
		return 0;
	}

	//std::cout << "[DEBUG] - 3" << std::endl;

	//std::cout << "친구 수는 :" << friendNum << std::endl;
	// 친구 수에 따라 벡터 reserve.
	RetFriendNicknameCont.reserve(friendNum); // 해당 벡터 reserve.

	//std::cout << "[DEBUG] - 4" << std::endl;

	// 친구 데이터를, 적재.
	for (int iter = 0; iter < friendNum; ++iter) // idBuffer를 버퍼로해서 사용.
	{
		inFile >> idStringBuffer;
		
		RetFriendNicknameCont.emplace_back(/*converter.from_bytes(*/idStringBuffer/*)*/);

		// 친구가 로그인 중인지 아닌지의 체크가, 여기서 이루어지면 안되고, 친구 UI를 On할 경우 해야함. -> 짜피 하드에서 Load할 필요 없는 데이터임.
		//if (userDataCont[userDataContHashValueBuffer].find(idStringBuffer) == userDataCont[userDataContHashValueBuffer].end())
	}

	//std::cout << "[DEBUG] - 5" << std::endl;

	// 정상적인 로그인.
	pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickname,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetActiveCharacterIndex, RetFriendNicknameCont);

	/*pInSocketInfo->pUserNode =*/ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);

	return 0;
}

/*
	LogoutProcess
	 - 로그아웃을 처리하는 함수입니다.
	 - 해당 유저 데이터를 파일로 저장합니다.
*/
void UserDataManager::LogoutProcess(shared_ptr<UserData> pUserNode)
{
	userNum--;

	if (pUserNode->GetNickname().size() == 0)
	{
		// 야 이거는 닉네임 없는거야 저장하지 마 
	}
	else
	{
		// 친구 기능중 이였으면, 친구 마지막 데이터 삭제함 // 이거 잘못하면 간다 간다 훅간다!
		//if (pUserNode->GetDemandFriendContIndex() != -1)
		//{
			pUserNode->SetDeleteFriendID();
		//}

		// 파일명 제작
		string fileNameBuffer = "UserData/Saved/.txt";
		string keyBuffer = pUserNode->GetKey();

		fileNameBuffer.insert(fileNameBuffer.begin() + 15, keyBuffer.begin(), keyBuffer.end());

		// 파일 쓰기 모드로 오픈
		std::ofstream outFile(fileNameBuffer, std::ios::out);

		// 파일 쓰기 (저장)
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

		// for-iter에서 직접 받으면, begin하고 end하고 메모리주소 다르다고 오류남 ( called by Value )
		auto iterBuffer = pUserNode->GetFriendNicknameCont();

		for (auto iter 
			= iterBuffer.begin()
			; iter != iterBuffer.end()
			; ++iter)
		{
			outFile << " " << /*converter.to_bytes(*iter)*/ *iter << std::endl;	// [DEV_66]
		}
	}

	// UserCont(MAP)에서 해당 정보 삭제.
	userDataCont[GetStringFirstChar(pUserNode->GetID()[0])].DeleteWithSearch(pUserNode->GetKey());
}

/*
	SearchUserNode
		- 유저노드를 ID를 통해 UserDataCont에서 탐색합니다.
*/
auto UserDataManager::SearchUserNode(const Type_ID& keyID, bool& RetBool) -> shared_ptr<UserData>
{
	return userDataCont[GetStringFirstChar(keyID[0])].Search(keyID, RetBool);
}

/*
	SearchUserNodeByNickname
		- 닉네임을 통해 닉네임 컨테이너에서 그 유저의 ID를 탐색하고, 해당 유저의 ID를 통하여 UserDataCont에서 탐색합니다. (SearchUserNode)

*/
shared_ptr<UserData> UserDataManager::SearchUserNodeByNickname(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch)
{
	bool isReturnTrue{ false };
	Type_ID idBuffer{};

	//std::cout << "[FRIEND] 닉네임을 찾습니다. " << std::endl;

	// 영어일 경우, 0번 컨테이너, 한글일 경우 1번 컨테이너에 삽입함.
	if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(KeyNickname.data()), 0)
		; firstCharType == _MBC_SINGLE)
	{
		//std::cout << "[FRIEND] 닉네임은 MBC_Single : 영어입니다. " << std::endl;

		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Search(KeyNickname, isReturnTrue)->SetValue().GetID();
	}
	else if (firstCharType == _MBC_LEAD)
	{
		//std::cout << "[FRIEND]닉네임은 MBC_LEAD(선행비트) : 한글입니다. " << std::endl;

		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Search(KeyNickname, isReturnTrue)->SetValue().GetID();
	}
	else //  if (firstCharType == _MBC_ILLEGAL)
	{
#ifdef _DEBUG_MODE_
		std::cout << "[FRIEND]닉네임은 이상한 글자입니다. " << std::endl;
#endif
		idBuffer = nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Search(KeyNickname, isReturnTrue)->SetValue().GetID();
	}

	// 못찾음.
	if (!isReturnTrue)
	{
		//std::cout << "[FRIEND] 그런 닉네임의 유저가 없어요. " << std::endl;

		RetIsMatch = false;
		RetIsOnLogin = false;
		return CONST_NULL_USERDATA;
	}

	//std::cout << "[FRIEND] 해당 닉네임을 가진 유저를 닉네임 컨테이너에서 찾았습니다. UserData에서 탐색합니다. " << std::endl;

	RetIsMatch = true;
	return SearchUserNode(idBuffer, RetIsOnLogin);
}

/*
	SearchUserNodeByNickname
		- 닉네임을 통해 닉네임 컨테이너에서 그 유저의 ID를 탐색하고, 해당 유저의 ID를 통하여 UserDataCont에서 탐색합니다. (SearchUserNode)
*/
shared_ptr<UserData> UserDataManager::SearchUserNodeByNicknameWithActiveCharacterIndex(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch, BYTE& RetActiveCharacterIndex)
{
	bool isReturnTrue{ false };
	Type_ID idBuffer{};

	//std::cout << "[FRIEND] 닉네임을 찾습니다. " << std::endl;

	// 영어일 경우, 0번 컨테이너, 한글일 경우 1번 컨테이너에 삽입함.
	if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(KeyNickname.data()), 0)
		; firstCharType == _MBC_SINGLE)
	{
		//std::cout << "[FRIEND] 닉네임은 MBC_Single : 영어입니다. " << std::endl;

		auto retBuffer = nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Search(KeyNickname, isReturnTrue);
		idBuffer = retBuffer->SetValue().GetID();
		RetActiveCharacterIndex = retBuffer->SetValue().GetActiveCharacterIndex();
	}
	else if (firstCharType == _MBC_LEAD)
	{
		//std::cout << "[FRIEND]닉네임은 MBC_LEAD(선행비트) : 한글입니다. " << std::endl;

		auto retBuffer = nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Search(KeyNickname, isReturnTrue);
		idBuffer = retBuffer->SetValue().GetID();
		RetActiveCharacterIndex = retBuffer->SetValue().GetActiveCharacterIndex();
	}
	else //  if (firstCharType == _MBC_ILLEGAL)
	{
		//std::cout << "[FRIEND]닉네임은 이상한 글자입니다. " << std::endl;

		auto retBuffer = nicknameCont[static_cast<int>(LANGUAGE::OTHERS)].Search(KeyNickname, isReturnTrue);
		idBuffer = retBuffer->SetValue().GetID();
		RetActiveCharacterIndex = retBuffer->SetValue().GetActiveCharacterIndex();
	}

	// 못찾음.
	if (!isReturnTrue)
	{
		//std::cout << "[FRIEND] 그런 닉네임의 유저가 없어요. " << std::endl;

		RetIsMatch = false;
		RetIsOnLogin = false;
		return CONST_NULL_USERDATA;
	}

	//std::cout << "[FRIEND] 해당 닉네임을 가진 유저를 닉네임 컨테이너에서 찾았습니다. UserData에서 탐색합니다. " << std::endl;

	RetIsMatch = true;
	return SearchUserNode(idBuffer, RetIsOnLogin);
}


/*
	GetActiveCharacterIndexWithNickname
	 - 닉네임을 제공받을 경우, 해당 닉네임에 대한 ActiveCharacterIndex를 제공합니다.

	 Friend가 Off일 경우, 해당 친구의 일러스트를 확인하기 위해 사용됩니다.
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
	- 닉네임과 바뀐 캐릭터 인덱스를 제공받고, 해당 닉네임에 대한 ActiveCharacterIndex를 변경합니다.
	
	플레이어가, 인덱스 변경 시, 호출됩니다.
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
		- 신규가입자를 항상 적재합니다.
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

