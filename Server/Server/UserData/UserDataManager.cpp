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

		// 영어일 경우, 0번 컨테이너, 한글일 경우 1번 컨테이너에 삽입함.
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

	// IOCPServer 생성자에서 UserDataManager의 생성자가 호출되기 때문에, 닉네임Cont의 로드가 모두 끝난 후 서버가 켜지는 것이 보장됨.
}

UserDataManager::~UserDataManager()
{

}

int UserDataManager::LoginProcess(SocketInfo* pInSocketInfo, const string& InID, wstring& RetNickName, int& RetWinCount, int& RetLoseCount, int& RetMoney,
	int& RetAchievementBit, int& RetTitleBit, int& RetCharacterBit, vector<wstring>& RetFriendStringCont)
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
	string idStringBuffer;			// idBuffer

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
	string nickNameBuffer{};

	// 파일 오픈
	std::ifstream inFile(fileNameBuffer, std::ios::in);

	// 파일 데이터 로드
	inFile 
		>> idStringBuffer 
		>> nickNameBuffer /* == RetNickName // DEV_66으로 인해 버퍼 사용 */
		>> RetWinCount 
		>> RetLoseCount 
		>> RetMoney 
		>> RetAchievementBit 
		>> RetTitleBit 
		>> RetCharacterBit 
		>> friendNum;

	// [DEV_66] 멀티바이트에서 유니코드 변환
	RetNickName = converter.from_bytes(nickNameBuffer);

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
		pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickName,
			RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit);
		
		userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
		return 0;
	}

	// 친구 수에 따라 벡터 reserve.
	RetFriendStringCont.reserve(friendNum); // 해당 벡터 reserve.

	// 친구 데이터를, 적재.
	for (int iter = 0; iter < friendNum; ++iter) // idBuffer를 버퍼로해서 사용.
	{
		inFile >> idStringBuffer;
		
		//DEV_66 저장및 불러들이는 것은 멀티바이트로, 이걸 내부에서는 결과적으로 유니코드로, 
		RetFriendStringCont.emplace_back(converter.from_bytes(idStringBuffer));

		// 친구가 로그인 중인지 아닌지의 체크가, 여기서 이루어지면 안되고, 친구 UI를 On할 경우 해야함. -> 짜피 하드에서 Load할 필요 없는 데이터임.
		//if (userDataCont[userDataContHashValueBuffer].find(idStringBuffer) == userDataCont[userDataContHashValueBuffer].end())
	}

	// 정상적인 로그인.
	pInSocketInfo->pUserNode = make_shared<UserData>(pInSocketInfo, InID, RetNickName,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetFriendStringCont);

	/*pInSocketInfo->pUserNode =*/ userDataCont[userDataContIndex].Insert(pInSocketInfo->pUserNode);
	return 0;
}

void UserDataManager::LogoutProcess(shared_ptr<UserData>& pUserNode)
{
	if (pUserNode->GetNickName().size() == 0)
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

		// for-iter에서 직접 받으면, begin하고 end하고 메모리주소 다르다고 오류남 ( called by Value )
		auto iterBuffer = pUserNode->GetFriendNicknameCont();

		for (auto iter 
			= iterBuffer.begin()
			; iter != iterBuffer.end()
			; ++iter)
		{
			outFile << " " << converter.to_bytes(*iter) << std::endl;	// [DEV_66]
		}
	}

	// UserCont(MAP)에서 해당 정보 삭제.
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

	// 영어일 경우, 0번 컨테이너, 한글일 경우 1번 컨테이너에 삽입함.
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

	// 못찾음.
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
