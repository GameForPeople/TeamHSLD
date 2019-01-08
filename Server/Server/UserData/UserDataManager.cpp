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

	// IOCPServer 생성자에서 UserDataManager의 생성자가 호출되기 때문에, 닉네임Cont의 로드가 모두 끝난 후 서버가 켜지는 것이 보장됨.
	while (!inFile.eof())
	{
		inFile >> idBuffer >> NicknameBuffer;

		// 영어일 경우, 0번 컨테이너, 한글일 경우 1번 컨테이너에 삽입함.
		if (int firstCharType = _mbsbtype(reinterpret_cast<const unsigned char *>(NicknameBuffer.data()), 0)
			; firstCharType == _MBC_SINGLE)
		{
			nicknameCont[static_cast<int>(LANGUAGE::ENGLISH)].Insert(NicknameBuffer, idBuffer);
			//std::cout << "영어입니다. \n";
		}
		else if (firstCharType == _MBC_LEAD)
		{
			nicknameCont[static_cast<int>(LANGUAGE::KOREAN)].Insert(NicknameBuffer, idBuffer);
			//std::cout << "아마 한글일껍니다. \n";
		}
		else  //  if (firstCharType == _MBC_ILLEGAL)
		{
			std::cout << "_MBC_ILLEGAL 입니다. \n";
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
	// 유저 데이터 번호
	int userDataContIndex = GetStringFirstChar(InID[0]);

	bool RetBoolBuffer;
	
	//pInSocketInfo->pUserNode = 
		userDataCont[userDataContIndex].Search(InID, RetBoolBuffer);
	//pInSocketInfo->pUserNode = nullptr;	//? 이게 무슨 짓이야?

	// 이미 로그인 여부 체크.
	if (RetBoolBuffer)
	{
		return 3; 
	}

	// 변수선언 부
	RetMoney = -1; // 재화가 -1일리 없음. 해당상황은, 파일 오픈 여부를 확인하기 위함.
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

	// DEV_66 닉네임 유니코드 처리
	//string NicknameBuffer{};

	// 파일 오픈
	std::ifstream inFile(fileNameBuffer, std::ios::in);

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
		>> friendNum;

	// [DEV_66] 멀티바이트에서 유니코드 변환
	//RetNickname = converter.from_bytes(NicknameBuffer);

	// 해당 파일이 없을 경우 리턴.
	if (RetMoney == -1)
	{
		pInSocketInfo->pUserNode = make_shared<UserData>();
		/*pInSocketInfo->pUserNode = */ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);

		// RetMoney가 -1일 경우, 클라언트에서 닉네임 입력 필요. (Login Scene ? // Main UI Scene ?)
		std::cout << "유저 데이터 로드에 실패하여 회원가입했습니다. \n";
		return 0;
	}

	//친구 없음. 정상 로그인.
	if (friendNum == 0)
	{
		pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickname,
			RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit);
		
		userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
		return 0;
	}

	// 친구 수에 따라 벡터 reserve.
	RetFriendNicknameCont.reserve(friendNum); // 해당 벡터 reserve.

	// 친구 데이터를, 적재.
	for (int iter = 0; iter < friendNum; ++iter) // idBuffer를 버퍼로해서 사용.
	{
		inFile >> idStringBuffer;
		
		//DEV_66 저장및 불러들이는 것은 멀티바이트로, 이걸 내부에서는 결과적으로 유니코드로, 
		RetFriendNicknameCont.emplace_back(/*converter.from_bytes(*/idStringBuffer/*)*/);

		// 친구가 로그인 중인지 아닌지의 체크가, 여기서 이루어지면 안되고, 친구 UI를 On할 경우 해야함. -> 짜피 하드에서 Load할 필요 없는 데이터임.
		//if (userDataCont[userDataContHashValueBuffer].find(idStringBuffer) == userDataCont[userDataContHashValueBuffer].end())
	}

	// 정상적인 로그인.
	pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickname,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetFriendNicknameCont);

	/*pInSocketInfo->pUserNode =*/ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
	return 0;
}

void UserDataManager::LogoutProcess(shared_ptr<UserData> pUserNode)
{
	if (pUserNode->GetNickname().size() == 0)
	{
		// 야 이거는 닉네임 없는거야 저장하지 마 
	}
	else
	{
		// 친구 기능중 이였으면, 친구 마지막 데이터 삭제함 // 이거 잘못하면 간다 간다 훅간다!
		if (pUserNode->GetDemandFriendContIndex() != -1)
		{
			pUserNode->SetDeleteFriendID();
		}

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
			<< " " << pUserNode->GetTitleBit()
			<< " " << pUserNode->GetCharacterBit()
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

shared_ptr<UserData> UserDataManager::SearchUserNode(const Type_ID& keyID, bool& RetBool)
{
	return userDataCont[GetStringFirstChar(keyID[0])].Search(keyID, RetBool);
}

shared_ptr<UserData> UserDataManager::SearchUserNodeWithNickname(const Type_Nickname& KeyNickname, bool& RetIsOnLogin, bool& RetIsMatch)
{
	bool isReturnTrue{ false };
	Type_ID idBuffer{};

	std::cout << "[FRIEND] DEBUG 0 - 1 " << std::endl;

	// 영어일 경우, 0번 컨테이너, 한글일 경우 1번 컨테이너에 삽입함.
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

	// 못찾음.
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

