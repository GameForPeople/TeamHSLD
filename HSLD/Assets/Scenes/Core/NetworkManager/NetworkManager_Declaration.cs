using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using System;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

using System.Runtime.InteropServices;

using System.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

using UnityEngine.Networking;
using System.Net.NetworkInformation;

public enum PROTOCOL : int
{
    //for LoginScene
    DEMAND_LOGIN = 100,
    FAIL_LOGIN = 101,
    PERMIT_LOGIN = 102,

    //for LobbyScene
    CHANGE_NICKNAME = 103,  // Client To Server
    PERMIT_NICKNAME = 104,  // Server to Client - buffer

    // for MainUiScene
    DEMAND_FRIEND_INFO = 201,   // Client to Server - 친구 정보 보내주세요.
    PERMIT_FRIEND_INFO = 202,   // Server to Client

    DEMAND_FRIEND_INVITE = 203,   // Client to Server - with Index , A클라이언트가, index(B) 클라이언트에게 같이 플레이하고 싶다고 요청.
    NOTIFY_FRIEND_INVITE = 204, // Server to Client - with bool , Search를 통해, nullptr이 아닐 경우, true및 UDP에 Push

    ANSWER_FRIEND_INVITE = 205, // Client to Server  - index(B) 클라이언트에게서 대답이 옴, 근데 이 때 만약 대답이 true일 경우, 206
                                // 대답이 False일 경우, 서버내부(Server Define)에서, 이를 호스트 클라이언트에게 알리는데 쓰임. -> 안쓰임
    GUESTCHECK_FRIEND_INVITE = 206, // Server to Client  - 최초 A클라이언트와 roomPtr의 포인터 유효성 검사 후, 가능하면 true.

    // 이전 까지는, DEMAND_ROOMHOST 호출.
    DELAY_FRIEND_INVITE = 207,  // Client to Server  - 야 씨, 나 7초나 기달렸는데 왜 대답없냐?
    HOSTCHECK_FRIEND_INVITE = 208,  // Server to Client - 야 GuestUser nullptr 맞다. 게임안하려나 봐.

    DEMAND_MAKE_FRIEND = 209,   // 친구하고 싶어요. (상대방 허락 안맡음. -> 내가 4명 이상이면 애초에 안되고, 상대방이 4명 이하면 OK)
    CHECK_DEMAND_MAKE_FRIEND = 210, // UDP Message 전송 여부. 및 실패 사유

    DEMAND_MAKE_FRIEND_INFO = 211,  // 닉네임 정보 및 관련 정보 전달 요구
    NOTIFY_MAKE_FRIEND_INFO = 212,  // 야 닉네임 이런 애가 너한테 친구하재

    ANSWER_MAKE_FRIEND = 213,       // 클라로부터 답변이 옴. 친구할지 안할지 여부.
    CHECK_ANSWER_MAKE_FRIEND = 214, // 해당 답변 처리중 에러여부에 대해 확인시켜줌.

    // for MainUiScene -- shop
    DEMAND_BUY_ITEM = 215,      // 아이템 인덱스 날라옴.
    ANSWER_BUY_ITEM = 216,      // 샀으면 트루, 못샀으면 False날라옴.

    // for MainUiScene -- VipCode
    DEMAND_VIP_CODE = 217,      // 님 이거 VIP Code 맞아요?
    ANSWER_VIP_CODE = 218,      // 맞네 틀리네~
    
    // for MainUiScene -- Character Buy
    DEMAND_BUY_CHARACTER = 219,     // 이 캐릭터 살게요
    ANSWER_BUY_CHARACTER = 220,     // 응 살수 있나 보고 살수있으면 팔아줄게

    // 구 Lobby Protocol
    DEMAND_MAKEROOM = 301,   // 안쓰도록 변경할 예정입니다.//아니다 친구와 같이하기 기능을 위해 남겨둡니다..
    PERMIT_MAKEROOM = 302,   // 안쓰도록 변경할 예정입니다.//아니다 친구와 같이하기 기능을 위해 남겨둡니다..
    DEMAND_JOINROOM = 303,   // 안쓰도록 변경할 예정입니다.//아니다 친구와 같이하기 기능을 위해 남겨둡니다..
    PERMIT_JOINROOM = 304,   // 안쓰도록 변경할 예정입니다.//아니다 친구와 같이하기 기능을 위해 남겨둡니다..
    FAIL_JOINROOM = 305,   // 안쓰도록 변경할 예정입니다.//아니다 친구와 같이하기 기능을 위해 남겨둡니다..

    // 신 Lobby Protocol
    DEMAND_RANDOM_MATCH = 311,   // 해당 프로토콜을 받을 경우, 먼저 방 접속 가능 여부를 확인하고, 불가능 시 방을 생성합니다.
    PERMIT_MAKE_RANDOM = 312,   // 방을 만들었다고 알립니다.
    PERMIT_JOIN_RANDOM = 313,   // 방에 접속했다고 알립니다.

    DEMAND_GUEST_JOIN = 314,   // 방에 게스트가 접속했는지의 여부를 확인합니다.
    PERMIT_GUEST_JOIN = 315,    // 방에 게스트가 접속했음.
    PERMIT_GUEST_NOT_JOIN = 316,	// 방에 게스트가 접속했음.

    DEMAND_EXIT_RANDOM = 317, // 랜덤큐를 취소함을 알림.
    ANSWER_EXIT_RANDOM = 318, // 랜덤큐 취소 여부에 대해 알림 (성공시만 받고, 실패시는, guest Join 으로 날라옴)

    DEMAND_FRIEND_JOIN = 319,   // [FRIEND] 친구가 초대에 응하는 지 여부 판단 (Friend 전용)
    ANSWER_FRIEND_JOIN = 320,	// [FRIEND] 이에 따른 대답 해줌.
    //for RoomScene

    // 구 Room Protocol
    DEMAND_ROOMHOST = 400,
    ROOMSTATE_VOID = 410,
    ROOMSTATE_GUESTIN = 411,


    // 신 Room Protocol
    DEMAND_ENEMY_CHARACTER = 421,   // 상대방 캐릭터의 변경 정보를 확인합니다.
    PERMIT_ENEMY_CHARACTER = 422,   // 상대방의 캐릭터 정보를 받아옵니다.

    DISCONNECTED_ENEMY_CLIENT = 506, // 상대편 클라이언트 네트워크 예외에 대한 처리 요청 (roomProtocol 및 클라에서 적용) - (Server to client)
    DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT = 506, // 상대편 클라이언트가 나갔음을 인지하고, 해당 처리를 요청함. (Client to Server)


    // for GameScene // 단방향으로 수정 적용 완료(2018/10/19ver)
    VOID_GAME_STATE = 500,  // 디펜스 턴인 친구가, 야 공격턴이 공격햇어??를 여쭤봄

    NOTIFY_CHANGE_TURN = 501,   // 야 나 다했다!

    NOTIFY_DICE_VALUE = 502,    // 서버야 주사위 값 받아라

    NOTIFY_TERRAIN_TYPE = 503,  // A클라이언트가 선택한 지형 카드 서버에 알림

    NOTIFY_TERRAIN_INDEXS = 504,    // A클라이언트가 변경한 지형 카드 인덱스를 서버에 전달

    NOTIFY_EVENTCARD_INDEX = 505, // A클라이언트가 사용한 이벤트 카드 인덱스 

    NOTIFY_GAME_READY = 507,    // 이거 주고 받으면 꼴토롱됨.

    NOTIFY_EMOJI = 508	// 이모티콘 // 이모지 관련입니다.
};

public partial class NetworkManager : MonoBehaviour {
    //////////////////////////////// FOR ONLY CLIENT TEST
    public bool isOnNetwork = false;
    ////////////////////////////////

    public string iP_ADDRESS;
    private const int SERVER_PORT = 9000;
    private const string CLIENT_VERSION = "181023";

    //public Thread thread;
    public Socket socket;

    public bool networkSyncLock = false;    // 임시적으로 네트워크 데이터 통신에 대한 중첩적인 함수를 제어합니다.

    public int recvType = 0;
    public int sendType = 0;

    //Client Data

    #region [ In Scene's variable]

    // Init Login Scene
    public string ID = "TEST_해솔리드";
    public string nickName = "TEST_해솔리드";

    //public int PW = 1234;
    public int winCount = 0;
    public int loseCount = 0;
    public int money = 20190102;

    // Use Main UI Scene
    public int friendNum;
    //public string/*Builder*/[] friendIdCont = new string/*Builder*/[4];
    public string/*Builder*/[] friendNickNameCont = new string/*Builder*/[4];
    public int[] friendState = new int[4];

    // Init LobbyScene
    public bool isHost = true;

    // Init RoomScene
    public string enemyId;

    //public byte[] DataRecvBuffer = new byte[100];   //얌마 나중에 이거 알아서 해라
    //public byte[] DataSendBuffer = new byte[100];   // 정신나간놈;; 100바이트나 한번에!
    public byte[] NewDataSendBuffer = new byte[100];
    public byte[] NewDataRecvBuffer = new byte[100];
    //public byte[] BigDataRecvBuffer = new byte[300];

    // For InGameScene
    public bool isAttackFirst;
    public int playerMissionIndex;
    public int enemyMissionIndex;
    public int subMissionIndex;

    public int playerCharacterIndex;
    public int enemyCharacterIndex;

    #endregion 

    public object _obj = new object();

    public InGameSceneManager inGameSceneManager; // 이 변수는 인게임씬 생성자에서 보장을 해줘야합니다.

    void OnDestroy()
    {
        socket.Close();
        socket = null;
    }
}
