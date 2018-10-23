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

// 아직 코드 보지 마세요....테스트 코드라.. 리팩토링 안해서 눈 썩어요..
// 클라이언트 현재 네트워크 동기방식 -> 턴제라 그냥 써도 될듯 한데, 프레임 드랍 등, 문제되면 비동기방식으로 변경 필요

// State object for receiving data from remote device.

public class NetworkManager : MonoBehaviour
{
    //////////////////////////////// FOR ONLY CLIENT TEST
    public bool isOnNetwork = false;
    ////////////////////////////////

    //[StructLayout(LayoutKind.Sequential)]
    //public class socket_data
    //{
    //    public int msg;
    //
    //    [MarshalAs(UnmanagedType.ByValArray, SizeConst = 100)] public char[] data;
    //}

    [StructLayout(LayoutKind.Sequential)]
    public class DemandLoginStruct
    {
        public int msg;
        public int type;
        public int pw;
        public int IDSize;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 20)] public char[] data;
    }

    public string iP_ADDRESS;
    private const int SERVER_PORT = 9000;
    private const string CLIENT_VERSION = "181023";

    //public Thread thread;
    public Socket socket;

    public bool isRecvOn = false;
    public int recvType = 0;
    public int sendType = 0;

    //Client Data

    // Init Login Scene
    public string ID = "TEST_Account";
    public int PW = 1234;
    public int winCount = 0;
    public int loseCount = 0;
    public int money = 7777777;

    // Init LobbyScene
    public int roomIndex = 0;
    public bool isHost = true;

    // Init RoomScene
    public string enemyId;

    public byte[] DataRecvBuffer = new byte[100];   //얌마 나중에 이거 알아서 해라
    public byte[] DataSendBuffer = new byte[100];   // 정신나간놈;; 100바이트나 한번에!
    public byte[] NewDataSendBuffer = new byte[100];
    public byte[] NewDataRecvBuffer = new byte[100];


    // For InGameScene
    public bool isAttackFirst;
    public int playerMissionIndex;
    public int enemyMissionIndex;
    public int subMissionIndex;

    public int playerCharacterIndex;
    public int enemyCharacterIndex;

    public object _obj = new object();

    public InGameSceneManager inGameSceneManager; // 이 변수는 인게임씬 생성자에서 보장을 해줘야합니다.

    //public GameObject inGameSceneManager;
    //private int dataLength;                     // Send Data Length. (byte)
    //private byte[] sendBuffer;                        // Data encoding to send. ( to Change bytes)
    //private byte[] Receivebyte = new byte[2000];    // Receive data by this array to save.
    //private string ReceiveString;                     // Receive bytes to Change string.

    ~NetworkManager()
    {
        socket.Close();
        socket = null;
    }

    // Use this for initialization
    public void StartNetworkFunction()
    {
        isOnNetwork = true;

        // Socket Create
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.SendTimeout, 10000);
        socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, 10000);

        // Socket connect.
        try
        {
            IPAddress ipAddr = System.Net.IPAddress.Parse(iP_ADDRESS);
            IPEndPoint ipEndPoint = new System.Net.IPEndPoint(ipAddr, SERVER_PORT);
            socket.Connect(ipEndPoint); // 아니 솔직히 커넥트는 비동기로 안한다. 이거는 c++서버도 안했다 오바지
        }
        catch (SocketException SCE)
        {
            Debug.Log("Socket connect error! : " + SCE.ToString());
            GameObject.Find("GameCores").transform.Find("AppQuitManager").GetComponent<AppQuitManager>().Quit(true, 1);
            return;
        }

        Debug.Log("Connect가 정상적으로 완료됐습니다!");
        //SendData(100);
        //m_scenenManager = GameObject.Find("SceneManager");
    }
    // Update is called once per frame

    //[ComVisibleAttribute(true)]
    public void StructToBytes(object obj, ref byte[] packet)
    {
        int size = Marshal.SizeOf(obj);
        packet = new byte[size];
        IntPtr buffer = Marshal.AllocHGlobal(size + 1);
        Marshal.StructureToPtr(obj, buffer, false);
        Marshal.Copy(buffer, packet, 0, size);
        Marshal.FreeHGlobal(buffer);
    }

    public void BytesToStructure(byte[] bValue, ref object obj, Type t)
    {
        int size = Marshal.SizeOf(t);
        IntPtr buffer = Marshal.AllocHGlobal(size);
        Marshal.Copy(bValue, 0, buffer, size);
        obj = Marshal.PtrToStructure(buffer, t);
        Marshal.FreeHGlobal(buffer);
    }

    public void SendData(int InMsg)
    {
        if (NetworkInterface.GetIsNetworkAvailable())
        {
            if (isOnNetwork)
            {
                //LoginScene
                if (InMsg == (int)PROTOCOL.DEMAND_LOGIN)
                {
                    DemandLoginStruct demandLogin = new DemandLoginStruct
                    {
                        msg = InMsg,
                        type = GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().typeBuffer,
                        pw = GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().PWBuffer
                    };

                    string IDBuffer = GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().IDBuffer;
                    int lengthBuffer = IDBuffer.Length;

                    demandLogin.data = new char[20]; // data는 ID String Byte 배열

                    Debug.Log("lengthBuffer = " + lengthBuffer);

                    for (int i = 0; i < lengthBuffer; i++)
                        demandLogin.data[i] = IDBuffer[i];

                    demandLogin.IDSize = lengthBuffer;

                    byte[] packet = new byte[1];
                    StructToBytes(demandLogin, ref packet);
                    socket.Send(packet);
                }

                //LobbyScene - old
                else if (InMsg == (int)PROTOCOL.DEMAND_MAKEROOM)
                {
                    isHost = true;

                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_MAKEROOM), 0, DataSendBuffer, 0, 4);

                    socket.Send(DataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.DEMAND_JOINROOM)
                {
                    isHost = false;

                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_JOINROOM), 0, DataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(roomIndex), 0, DataSendBuffer, 4, 4);

                    socket.Send(DataSendBuffer, 8, SocketFlags.None);
                }

                //LobbyScene - new
                else if (InMsg == (int)PROTOCOL.DEMAND_RANDOM_MATCH)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_RANDOM_MATCH), 0, DataSendBuffer, 0, 4);
                    socket.Send(DataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.DEMAND_GUEST_JOIN)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_GUEST_JOIN), 0, DataSendBuffer, 0, 4);
                    socket.Send(DataSendBuffer, 4, SocketFlags.None);
                }

                // RoomScene - old
                else if (InMsg == (int)PROTOCOL.DEMAND_ROOMHOST)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_ROOMHOST), 0, DataSendBuffer, 0, 4);
                    socket.Send(DataSendBuffer, 4, SocketFlags.None);
                }

                // RoomScene - new
                else if (InMsg == (int)PROTOCOL.DEMAND_ENEMY_CHARACTER)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_ENEMY_CHARACTER), 0, DataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(playerCharacterIndex), 0, DataSendBuffer, 4, 4);

                    socket.Send(DataSendBuffer, 8, SocketFlags.None);
                }

                //InGameScene // Defense Turn
                else if (InMsg == (int)PROTOCOL.VOID_GAME_STATE)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.VOID_GAME_STATE), 0, DataSendBuffer, 0, 4);
                    socket.Send(DataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_CHANGE_TURN)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_CHANGE_TURN), 0, DataSendBuffer, 0, 4);
                    socket.Send(DataSendBuffer, 4, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_DICE_VALUE)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_DICE_VALUE), 0, DataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_changeTerrainCount), 0, DataSendBuffer, 4, 4);

                    socket.Send(DataSendBuffer, 8, SocketFlags.None);
                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_TERRAIN_TYPE)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_TERRAIN_TYPE), 0, DataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainType), 0, DataSendBuffer, 4, 4);

                    //Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainType), 0, DataSendBuffer, 4, 4);
                    //Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_changeTerrainCount), 0, DataSendBuffer, 8, 4);
                    //Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainIndex[0]), 0, DataSendBuffer, 12, 4 * inGameSceneManager.network_changeTerrainCount);

                    socket.Send(DataSendBuffer, 8, SocketFlags.None);  // 70..? 나중에 계산하기..!
                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_TERRAIN_INDEXS)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_TERRAIN_INDEXS), 0, NewDataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_changeTerrainCount), 0, NewDataSendBuffer, 4, 4);

                    Debug.Log(" " + inGameSceneManager.network_changeTerrainCount + " 이 주사위 값, 적재해야하는 인덱스의 크기입니다.");

                    int iBuffer;
                    for (int i = 0; i < inGameSceneManager.network_changeTerrainCount; i++)
                    {
                        iBuffer = inGameSceneManager.network_terrainIndex[i];
                        Buffer.BlockCopy(BitConverter.GetBytes(iBuffer), 0, NewDataSendBuffer, (8 + 4 * i), 4);
                        Debug.Log(" " + i + " 번 까지는 정상적으로 적재했습니다. 값 " + BitConverter.ToInt32(NewDataSendBuffer, (8 + 4 * i)));
                    }

                    //Debug.Log(" 전송하는 메모리의 크기는 ==> " + (8 + 4 * inGameSceneManager.network_changeTerrainCount) + " == " + BitConverter.ToInt32(NewDataSendBuffer, 4));

                    socket.Send(NewDataSendBuffer, 80, SocketFlags.None);  // 70..? 나중에 계산하기;;

                }
                else if (InMsg == (int)PROTOCOL.NOTIFY_EVENTCARD_INDEX)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_EVENTCARD_INDEX), 0, DataSendBuffer, 0, 4);
                    Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_eventCardType), 0, DataSendBuffer, 4, 4);

                    socket.Send(DataSendBuffer, 8, SocketFlags.None);
                }
                // Network Exception
                else if (InMsg == (int)PROTOCOL.DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT)
                {
                    Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.NOTIFY_CHANGE_TURN), 0, DataSendBuffer, 0, 4);
                    socket.Send(DataSendBuffer, 4, SocketFlags.None);
                }

                RecvProcess();
            }
            else
            {
                Debug.Log("주의 : OnlyClientTest 환경에서, SendData가 호출되었습니다.");
            }
        }
        else
        {
            GameObject.Find("GameCores").transform.Find("AppQuitManager").GetComponent<AppQuitManager>().Quit(true, 1);
            return;
        }
    }

    void RecvProcess()
    {
        RecvProtocolType();
        ProcessRecvData();
    }

    void RecvProtocolType()
    {
        try
        {
            socket.Receive(DataRecvBuffer);
        }
        catch (SocketException SCE)
        {
            Debug.Log("Socket connect error! : " + SCE.ToString());
            GameObject.Find("GameCores").transform.Find("AppQuitManager").GetComponent<AppQuitManager>().Quit(true, 1);
            return;
        }

        recvType = BitConverter.ToInt32(DataRecvBuffer, 0);

        if(recvType != (int)PROTOCOL.VOID_GAME_STATE)
            Debug.Log("RecvType is : " + recvType);
    }

    void ProcessRecvData()
    {
        //LoginScene
        if (recvType == (int)PROTOCOL.FAIL_LOGIN)
        {
            GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().failReason = BitConverter.ToInt32(DataRecvBuffer, 4);

            GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().FailLoginProcess();
        }
        else if (recvType == (int)PROTOCOL.PERMIT_LOGIN)
        {
            winCount = BitConverter.ToInt32(DataRecvBuffer, 4);
            Debug.Log("win is --> " + recvType);

            loseCount = BitConverter.ToInt32(DataRecvBuffer, 8);
            Debug.Log("lose is --> " + recvType);

            money = BitConverter.ToInt32(DataRecvBuffer, 12);
            Debug.Log("money is --> " + recvType);

            //
            GameObject.Find("LoginSceneManager").GetComponent<LoginSceneManager>().PermitLoginProcess();
        }

        //LobbyScene - old
        else if (recvType == (int)PROTOCOL.PERMIT_MAKEROOM)
        {
            roomIndex = BitConverter.ToInt32(DataRecvBuffer, 4);

            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().PermitMakeRoom();
        }
        else if (recvType == (int)PROTOCOL.PERMIT_JOINROOM)
        {
            roomIndex = BitConverter.ToInt32(DataRecvBuffer, 4);

            int idSizeBuffer = BitConverter.ToInt32(DataRecvBuffer, 8);

            //enemyId = System.String.Empty;
            enemyId = Encoding.Default.GetString(DataRecvBuffer, 12, idSizeBuffer);

            Debug.Log("받은 방장의 아이디는 " + enemyId + ", 크기는  " + idSizeBuffer + " 입니다. ");
            Debug.Log("복사한 방장의 아이디는 " + (char)DataRecvBuffer[12] + DataRecvBuffer[13] + DataRecvBuffer[14] + DataRecvBuffer[15] + "입니다. ");

            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().PermitJoinRoom();
        }
        else if (recvType == (int)PROTOCOL.FAIL_JOINROOM)
        {
            int failReason = BitConverter.ToInt32(DataRecvBuffer, 4);

            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().FailJoinRoom(failReason);
        }

        //LobbyScene - new
        else if (recvType == (int)PROTOCOL.PERMIT_MAKE_RANDOM)
        {
            isHost = true;
            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().isRecvTrue = true;

            roomIndex = BitConverter.ToInt32(DataRecvBuffer, 4);

            if (BitConverter.ToInt32(DataRecvBuffer, 8) == 1)
            {
                if (isHost == true)
                {
                    isAttackFirst = true;
                }
                else
                {
                    isAttackFirst = false;
                }
            }
            else
            {
                if (isHost == true)
                {
                    isAttackFirst = false;
                }
                else
                {
                    isAttackFirst = true;
                }
            }

            playerMissionIndex = BitConverter.ToInt32(DataRecvBuffer, 12);
            enemyMissionIndex = BitConverter.ToInt32(DataRecvBuffer, 16);
            subMissionIndex = BitConverter.ToInt32(DataRecvBuffer, 20);
        }
        else if (recvType == (int)PROTOCOL.PERMIT_JOIN_RANDOM)
        {
            isHost = false;
            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().isRecvTrue = true;

            roomIndex = BitConverter.ToInt32(DataRecvBuffer, 4);

            //int isHostFirst;
            if (BitConverter.ToInt32(DataRecvBuffer, 8) == 1)
            {
                if (isHost == true)
                {
                    isAttackFirst = true;
                }
                else
                {
                    isAttackFirst = false;
                }
            }
            else
            {
                if (isHost == true)
                {
                    isAttackFirst = false;
                }
                else
                {
                    isAttackFirst = true;
                }
            }

            playerMissionIndex = BitConverter.ToInt32(DataRecvBuffer, 12);
            enemyMissionIndex = BitConverter.ToInt32(DataRecvBuffer, 16);
            subMissionIndex = BitConverter.ToInt32(DataRecvBuffer, 20);

            int idSizeBuffer = BitConverter.ToInt32(DataRecvBuffer, 24);
            enemyId = Encoding.Default.GetString(DataRecvBuffer, 28, idSizeBuffer);
            Debug.Log("적 아이디는 " + enemyId);

            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().ChangeRoomScene();
        }

        else if (recvType == (int)PROTOCOL.PERMIT_GUEST_JOIN)
        {
            int idSizeBuffer = BitConverter.ToInt32(DataRecvBuffer, 4);
            enemyId = Encoding.Default.GetString(DataRecvBuffer, 8, idSizeBuffer);

            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().ChangeRoomScene();
        }
        else if (recvType == (int)PROTOCOL.PERMIT_GUEST_NOT_JOIN)
        {
            //아모고토못하죠
        }

        //RoomScene - old
        else if (recvType == (int)PROTOCOL.ROOMSTATE_VOID)
        {

        }
        else if (recvType == (int)PROTOCOL.ROOMSTATE_GUESTIN)
        {
            int idSizeBuffer = BitConverter.ToInt32(DataRecvBuffer, 4);
            enemyId = Encoding.Default.GetString(DataRecvBuffer, 8, idSizeBuffer);

            GameObject.Find("RoomSceneManager").GetComponent<RoomSceneManager>().GuestJoinRoom();
        }

        //RoomScene - new
        else if (recvType == (int)PROTOCOL.PERMIT_ENEMY_CHARACTER)
        {
            enemyCharacterIndex = BitConverter.ToInt32(DataRecvBuffer, 4);

            GameObject.Find("RoomSceneManager").GetComponent<RoomSceneManager>().SetEnemyCharacter_Network();
        }

        //InGameScene //-> 이부분 다 InGameScene으로 들어내도 괜찮을듯!

        else if (recvType == (int)PROTOCOL.VOID_GAME_STATE)
        {
            // 뭐야 니 아무것도 없엉
            return; //recvProtocolFlag 안쓸것 같긴 한데, 할튼 일단 꺼졍.
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_CHANGE_TURN)
        {
            inGameSceneManager.RecvChangeTurn();
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_DICE_VALUE)
        {
            //inGameSceneManager.network_terrainType = BitConverter.ToInt32(DataRecvBuffer, 4);
            // inGameSceneManager.network_changeTerrainCount = BitConverter.ToInt32(DataRecvBuffer, 8);

            // for (int i = 0; i < inGameSceneManager.network_changeTerrainCount; ++i)
            // {
            //     inGameSceneManager.network_terrainIndex[i] = BitConverter.ToInt32(DataRecvBuffer, 12 + 4 * i);
            // }
            inGameSceneManager.RecvDiceValue(BitConverter.ToInt32(DataRecvBuffer, 4));
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_TERRAIN_TYPE)
        {
            //inGameSceneManager.network_eventCardType = BitConverter.ToInt32(DataRecvBuffer, 4);
            // inGameSceneManager.network_terrainType = BitConverter.ToInt32(DataRecvBuffer, 8);
            //inGameSceneManager.network_changeTerrainCount = BitConverter.ToInt32(DataRecvBuffer, 12);

            //for (int i = 0; i < inGameSceneManager.network_changeTerrainCount; ++i)
            // {
            //     inGameSceneManager.network_terrainIndex[i] = BitConverter.ToInt32(DataRecvBuffer, 16 + 4 * i);
            // }
            inGameSceneManager.RecvTerrainType(BitConverter.ToInt32(DataRecvBuffer, 4));
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_TERRAIN_INDEXS)
        {
            //inGameSceneManager.network_eventCardType = BitConverter.ToInt32(DataRecvBuffer, 4);
            int arrSizeBuffer = BitConverter.ToInt32(DataRecvBuffer, 4);
            Debug.Log(" 총 받아야하는 배열의 크기는 " + arrSizeBuffer + "입니다");

            for (int i = 0; i < arrSizeBuffer; i++)
            {
                Debug.Log(" " + i + " 번 까지는 정상적으로 넣었습니다. "+ (8 + (4 * i)) + "의 부터 형변환 한 값 ::" + BitConverter.ToInt32(DataRecvBuffer, (8 + (4 * i))));
                inGameSceneManager.recvTerrainIndex[i] = BitConverter.ToInt32(DataRecvBuffer, (8 + (4 * i)));
            }

            inGameSceneManager.RecvTerrainIndex();
        }
        else if (recvType == (int)PROTOCOL.NOTIFY_EVENTCARD_INDEX)
        {
            //inGameSceneManager.network_eventCardType = BitConverter.ToInt32(DataRecvBuffer, 4);
        }

        // Network Exception
        else if (recvType == (int)PROTOCOL.DISCONNECTED_ENEMY_CLIENT)
        {
            //
            // BGM 제가 꺼드리겠습니다.
            GameObject.Find("GameCores").transform.Find("SoundManager").GetComponent<SoundManager>().BGMMaximumVolume = 0;
            GameObject.Find("GameCores").transform.Find("SceneControlManager").GetComponent<SceneControlManager>().ChangeScene(SCENE_NAME.MainUI_SCENE);
            //
            //SendData((int)PROTOCOL.DOUBLECHECK_DISCONNECTED_ENEMY_CLIENT);
        }

        recvType = 0;
    }

    public void ParsingServerIP()
    {
        StartCoroutine(ParsingServerIPCoroutine());
    }

    IEnumerator ParsingServerIPCoroutine()
    {
        UnityWebRequest www = UnityWebRequest.Get("http://koreagamemaker.wixsite.com/hsld-server"); // 나중에 GitPage로 바꾸기.
        yield return www.SendWebRequest();

        if (www.isNetworkError || www.isHttpError)
        {
            Debug.Log(www.error);
        }
        else
        {
            // Show results as text
            int index1 = www.downloadHandler.text.IndexOf("Server IP : ") + 12;
            int index2 = www.downloadHandler.text.IndexOf(".HSLD", index1);

            iP_ADDRESS = www.downloadHandler.text.Substring(index1, index2 - index1);
            // Or retrieve results as binary data
            Debug.Log("Server의 IP는 : " + iP_ADDRESS);

            int index3 = www.downloadHandler.text.IndexOf("Ver : ", index2);

            string parsingClientVerStringBuffer = www.downloadHandler.text.Substring(index3 + 6, 6);

            Debug.Log("최신 Client의 Ver는 : " + parsingClientVerStringBuffer + " 입니다. 현재 클라이언트 버전은 : " + CLIENT_VERSION + " 입니다.");

            int index4 = www.downloadHandler.text.IndexOf("NotifyNum : ", index3) + 12;

            string notifyNumString = www.downloadHandler.text.Substring(index4, 1);

            if (notifyNumString.Equals("0"))
            {
                Debug.Log("NotifyNum은 0으로 서버는 정상적으로 작동하는 중입니다.");
            }
            else if (notifyNumString.Equals("1"))
            {
                Debug.Log("NotifyNum은 1로 현재 서버는 작동하지 않습니다.");
            }
            else if (notifyNumString.Equals("2"))
            {
                Debug.Log("NotifyNum은 2로 현재 게임은 점검 - 업데이트 중입니다.");
            }

            GameObject.Find("WaitParsingUI").SetActive(false);
        }
    }
}



enum PROTOCOL : int
{
    //for LoginScene
    DEMAND_LOGIN = 100,
    FAIL_LOGIN = 101,
    PERMIT_LOGIN = 102,

    //for LobbyScene

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
};


