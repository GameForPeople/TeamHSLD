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
	// 유저 데이터 
	InPSocketInfo->userDataContIndex = GetStringFirstChar(InID[0]);

	// 이미 로그인 여부 체크.
	if (userDataCont[InPSocketInfo->userDataContIndex].find(InID) == userDataCont[InPSocketInfo->userDataContIndex].end())
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

	// 파일 오픈
	std::ifstream inFile(fileNameBuffer, std::ios::in);

	// 파일 데이터 로드
	inFile >> idStringBuffer >> RetNickName >> RetWinCount >> RetLoseCount >> RetMoney >> RetAchievementBit >> RetTitleBit >> RetCharacterBit >> friendNum;

	// 해당 파일이 없을 경우 리턴.
	if (RetMoney == -1)
	{
		InPSocketInfo->userIter = userDataCont[InPSocketInfo->userDataContIndex].emplace(InID, UserData(InPSocketInfo, InID)).first;
		std::cout << "유저 데이터 로드에 실패했습니다. \n";
		return 0;
	}

	std::cout << "로드한 유저의 닉네임은 : " << RetNickName << "입니다. \n";

	//친구 없음. 정상 로그인.
	if (friendNum == 0)
	{
		InPSocketInfo->userIter = userDataCont[InPSocketInfo->userDataContIndex].emplace(InID, UserData(InPSocketInfo, InID, RetNickName,
			RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit)).first;
		return 0;
	}

	// 친구 수에 따라 벡터 reserve.
	RetFriendStringCont.reserve(friendNum); // 해당 벡터 reserve.

	// 친구 데이터를, 적재.
	for (int iter = 0; iter < friendNum; ++iter) // idBuffer를 버퍼로해서 사용.
	{
		inFile >> idStringBuffer;
		RetFriendStringCont.emplace_back(idStringBuffer);

		// 친구가 로그인 중인지 아닌지의 체크가, 여기서 이루어지면 안되고, 친구 UI를 On할 경우 해야함. -> 짜피 하드에서 Load할 필요 없는 데이터임.
		//if (userDataCont[userDataContHashValueBuffer].find(idStringBuffer) == userDataCont[userDataContHashValueBuffer].end())
	}

	// 정상적인 로그인.
	InPSocketInfo->userIter = userDataCont[InPSocketInfo->userDataContIndex].emplace(InID, UserData(InPSocketInfo, InID, RetNickName,
		RetWinCount, RetLoseCount, RetMoney, RetAchievementBit, RetTitleBit, RetCharacterBit, RetFriendStringCont)).first;

	return 0;
}

void UserDataManager::LogoutProcess(SocketInfo* InPSocketInfo)
{
	// 파일명 제작
	string fileNameBuffer = "UserData/Saved/.txt";
	fileNameBuffer.insert(fileNameBuffer.begin() + 15, InPSocketInfo->userIter->second.GetID().begin(), InPSocketInfo->userIter->second.GetID().end());

	// 파일 쓰기 모드로 오픈
	std::ofstream outFile(fileNameBuffer, std::ios::out);
	
	// 파일 쓰기 (저장)
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

	// 친구 있으면 해당 이름 (저장)
	for (auto iter = InPSocketInfo->userIter->second.GetFriendStringCont().begin();
		iter != InPSocketInfo->userIter->second.GetFriendStringCont().end();
		++iter)
	{
		outFile << " " << *iter << std::endl;
	}

	// UserCont(MAP)에서 해당 정보 삭제.
	userDataCont[InPSocketInfo->userDataContIndex].erase(InPSocketInfo->userIter);
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

void UserDataManager::SetGameResult(user_iter* InUserIter, const bool& InWinOrLose)
{
	//InUserIter->second.SetGameResult(InWinOrLose);
}