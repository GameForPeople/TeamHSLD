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

// 아직 코드 보지 마세요....테스트 코드라.. 리팩토링 안해서 눈 썩어요..
// 클라이언트 현재 네트워크 동기방식 -> 턴제라 그냥 써도 될듯 한데, 프레임 드랍 등, 문제되면 비동기방식으로 변경 필요

enum PROTOCOL : int
{
    //for LoginScene
    DEMAND_LOGIN        =   100     ,
    FAIL_LOGIN          =   101     ,
    PERMIT_LOGIN        =   102     ,

    //for LobbyScene
    DEMAND_MAKEROOM     =   301     ,
    PERMIT_MAKEROOM     =   302     ,
    DEMAND_JOINROOM     =   303     ,
    PERMIT_JOINROOM     =   304     ,
    FAIL_JOINROOM       =   305     ,

    //for RoomScene
    DEMAND_ROOMHOST     =   400     ,
    ROOMSTATE_VOID      =   410     ,
    ROOMSTATE_GUESTIN   =   411     ,


    // for GameScene
    DEMAND_GAME_STATE   =   500     ,    // 디펜스 턴인 친구가, 야 공격턴이 공격햇어??를 여쭤봄
    VOID_GAME_STATE     =   501     ,  // 야 수비야 공격이 아무것도 안했어!

    VOID_CLIENT_TO_SERVER                       =   511 ,                // 공격턴 클라이언트가 시간초과로 아무것도 보내지 않을 때,
    CHANGE_PLANET_CLIENT_TO_SERVER              =   512 ,       // 공격턴 클라이언트가 땅을 바꿧을 때,
    ACTION_EVENTCARD_TERRAIN_CLIENT_TO_SERVER   =   513 ,    // 공격턴 클라이언트의 이벤트 카드(공격, 지형변경) 처리
    ACTION_EVENTCARD_DICEBUFF_CLIENT_TO_SERVER  =   514 ,   // 공격턴 클라이언트의 이벤트 카드(공격, 주사위 관련) 처리
    ACTION_EVENTCARD_DEFENSE_CLIENT_TO_SERVER   =   515 ,    // 미구현... ??턴 클라이언트의 이벤트 카드 쉴드 처리??


    VOID_SERVER_TO_CLIENT                       =   521 ,                // 서버가 수비턴 클라이언트에게 VOID전송 다만, 이거 받으면 턴 변경임!
    CHANGE_PLANET_SERVER_TO_CLIENT              =   522 ,       // 서버가 수비턴 클라이언트에게 공격 클라이언트가 바꾼 행성 정보를 전송
    ACTION_EVENTCARD_TERRAIN_SERVER_TO_CLIENT   =   523 ,    // 서버가 수비턴 클라이언트에게 공격턴 클라이언트 이벤트 카드(공격, 지형변경) 정보를 전송
    ACTION_EVENTCARD_DICEBUFF_SERVER_TO_CLIENT  =   524 ,   // 서버가 수비턴 클라이언트에게 공격턴 클라이언트 이벤트 카드(공격, 주사위 관련) 처리
    ACTION_EVENTCARD_DEFENSE_SERVER_TO_CLIENT   =   525 ,    // 미구현.... ??턴 클라이언트의 이벤트 카드 쉴드 처리??

    // Not yet used
    DEMAND_DICE_CLIENT_TO_SERVER                =   521 ,     // 주사위 숫자를 요구할 때,
    PERMIT_DICE_SERVER_TO_CLIENT                =   522 ,

    DEMAND_EVENTCARD_CLIENT_TO_SERVER           =   523 ,    // 공통덱에서 이벤트 카드를 요구할 때,
    PERMIT_EVENTCARD_SERVER_TO_CLIENT           =   524 ,

    DEMAND_EVENTCARD_DICE_CLIENT_TO_SERVER      =   525 , // 이벤트 카드의 숫자를 요구할 때
    PERMIT_EVENTCARD_DICE_SERVER_TO_CLIENT      =   526
};

public class NetworkManager : MonoBehaviour
{
    public bool isOnNetwork = false;

    // State object for receiving data from remote device.  
   // public class StateObject
   // {
   //     // Client socket.  
   //     public Socket workSocket = null;
   //     // Size of receive buffer.  
   //     public const int BufferSize = 256;
   //     // Receive buffer.  
   //     public byte[] buffer = new byte[BufferSize];
   //     // Received data string.  
   //     public StringBuilder sb = new StringBuilder();
   // }

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
    private const string CLIENT_VERSION = "180909";

    public Thread thread;
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

    public byte[] DataRecvBuffer = new byte[100];
    public byte[] DataSendBuffer = new byte[8];
    public byte[] Data50_SendBuffer = new byte[64]; // max - ACTION_EVENTCARD_TERRAIN_CLIENT_TO_SERVER

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
            return;
        }

        Debug.Log("Connect가 정상적으로 완료됐습니다!");
        //SendData(100);
        //m_scenenManager = GameObject.Find("SceneManager");
    }
    // Update is called once per frame
    void Update()
    {
       // if (sendType > 0)
       // {
       //     SendData(sendType);
       // }
       // else
       // {
       //     // 나중에 여기에 else말고 else if로 바꾸고, 현제 씬이 인게임씬인지를 확인하는 코드가 추가되어야함
       //     SendData((int)PROTOCOL.DEMAND_GAMESTATE);
       // }
       // 
       // if (isRecvOn)
       // {
       //     RecvType(socket);
       //     ProcessRecvData();
       //     //if (recvType > 0)
       //     //{
       //     //    RecvData(socket);
       //     //}
       //     //else
       //     //{
       //     //    RecvType(socket);
       //     //}
       // 
       //     isRecvOn = false;
       // }
    }

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
        if (isOnNetwork)
        {
            //if (InMsg == (int)PROTOCOL.DEMAND_GAMESTATE)
            //{
            //    byte[] sendDamandGameState = BitConverter.GetBytes((int)400);
            //    socket.Send(sendDamandGameState);
            //}
            //else 

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

            //LobbyScene
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

            // RoomScene
            else if (InMsg == (int)PROTOCOL.DEMAND_ROOMHOST)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_ROOMHOST), 0, DataSendBuffer, 0, 4);
                socket.Send(DataSendBuffer, 4, SocketFlags.None);
            }

            //InGameScene // Defense Turn
            else if (InMsg == (int)PROTOCOL.DEMAND_GAME_STATE)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_GAME_STATE), 0, DataSendBuffer, 0, 4);

                socket.Send(DataSendBuffer, 4, SocketFlags.None);
            }

            // InGameScene Attack Turn
            else if (InMsg == (int)PROTOCOL.VOID_CLIENT_TO_SERVER)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.VOID_CLIENT_TO_SERVER), 0, DataSendBuffer, 0, 4);
            }
            else if (InMsg == (int)PROTOCOL.CHANGE_PLANET_CLIENT_TO_SERVER)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.CHANGE_PLANET_CLIENT_TO_SERVER), 0, Data50_SendBuffer, 0, 4);
                Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainType), 0, Data50_SendBuffer, 4, 4);
                Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_changeTerrainCount), 0, Data50_SendBuffer, 8, 4);
                Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainIndex[0]), 0, Data50_SendBuffer, 12, 4 * inGameSceneManager.network_changeTerrainCount);
            }
            else if (InMsg == (int)PROTOCOL.ACTION_EVENTCARD_TERRAIN_CLIENT_TO_SERVER)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.ACTION_EVENTCARD_TERRAIN_CLIENT_TO_SERVER), 0, Data50_SendBuffer, 0, 4);
                Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_eventCardType), 0, Data50_SendBuffer, 4, 4);
                Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainType), 0, Data50_SendBuffer, 8, 4);
                Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_changeTerrainCount), 0, Data50_SendBuffer, 12, 4);
                Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_terrainIndex[0]), 0, Data50_SendBuffer, 16, 4 * inGameSceneManager.network_changeTerrainCount);
            }
            else if (InMsg == (int)PROTOCOL.ACTION_EVENTCARD_DICEBUFF_CLIENT_TO_SERVER)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.ACTION_EVENTCARD_DICEBUFF_CLIENT_TO_SERVER), 0, DataSendBuffer, 0, 4);
                Buffer.BlockCopy(BitConverter.GetBytes(inGameSceneManager.network_eventCardType), 0, DataSendBuffer, 4, 4);
            }
          
            RecvProcess();
        }
        else
        {
            Debug.Log("주의 : OnlyClientTest 환경에서, SendData가 호출되었습니다.");
        }
    }

    void RecvProcess()
    {
        RecvProtocolType();
        ProcessRecvData();
    }

    void RecvProtocolType()
    {
        socket.Receive(DataRecvBuffer);

        recvType = BitConverter.ToInt32(DataRecvBuffer, 0);
        Debug.Log("RecvType is : " + recvType);
    }

    void ProcessRecvData()
    {
        //if (recvType > (int)PROTOCOL.DEMAND_GAMESTATE)
        //{
        //   
        //}
        //else 
        
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

        //LobbyScene
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

            Debug.Log("받은 방장의 아이디는 " + enemyId + ", 크기는  " + idSizeBuffer+ " 입니다. ");
            Debug.Log("복사한 방장의 아이디는 " + (char)DataRecvBuffer[12] + DataRecvBuffer[13] + DataRecvBuffer[14] + DataRecvBuffer[15] + "입니다. ");

            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().PermitJoinRoom();
        }
        else if (recvType == (int)PROTOCOL.FAIL_JOINROOM)
        {
            int failReason = BitConverter.ToInt32(DataRecvBuffer, 4);

            GameObject.Find("LobbySceneManager").GetComponent<LobbySceneManager>().FailJoinRoom(failReason);
        }

        //RoomScene
        else if (recvType == (int)PROTOCOL.ROOMSTATE_VOID)
        {

        }
        else if (recvType == (int)PROTOCOL.ROOMSTATE_GUESTIN)
        {
            int idSizeBuffer = BitConverter.ToInt32(DataRecvBuffer, 4);
            enemyId = Encoding.Default.GetString(DataRecvBuffer, 8, idSizeBuffer);

            GameObject.Find("RoomSceneManager").GetComponent<RoomSceneManager>().GuestJoinRoom();
        }

        //InGameScene
        if (recvType > 500 && recvType < 600)
        { 
            if (recvType == (int)PROTOCOL.VOID_GAME_STATE)
            {
                // 뭐야 니 아무것도 없엉 메롱
            }
            else if (recvType == (int)PROTOCOL.CHANGE_PLANET_SERVER_TO_CLIENT)
            {
                inGameSceneManager.network_terrainType = BitConverter.ToInt32(DataRecvBuffer, 4);
                inGameSceneManager.network_changeTerrainCount = BitConverter.ToInt32(DataRecvBuffer, 8);

                for (int i = 0; i < inGameSceneManager.network_changeTerrainCount; ++i)
                {
                    inGameSceneManager.network_terrainIndex[i] = BitConverter.ToInt32(DataRecvBuffer, 12 + 4 * i);
                }
            }
            else if (recvType == (int)PROTOCOL.ACTION_EVENTCARD_TERRAIN_SERVER_TO_CLIENT)
            {
                inGameSceneManager.network_eventCardType = BitConverter.ToInt32(DataRecvBuffer, 4);
                inGameSceneManager.network_terrainType = BitConverter.ToInt32(DataRecvBuffer, 8);
                inGameSceneManager.network_changeTerrainCount = BitConverter.ToInt32(DataRecvBuffer, 12);

                for (int i = 0; i < inGameSceneManager.network_changeTerrainCount; ++i)
                {
                    inGameSceneManager.network_terrainIndex[i] = BitConverter.ToInt32(DataRecvBuffer, 16 + 4 * i);
                }
            }
            else if (recvType == (int)PROTOCOL.ACTION_EVENTCARD_DICEBUFF_SERVER_TO_CLIENT)
            {
                inGameSceneManager.network_eventCardType = BitConverter.ToInt32(DataRecvBuffer, 4);
            }

            inGameSceneManager.network_recvProtocolFlag = recvType;
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

            Debug.Log("최신 Client의 Ver는 : " + parsingClientVerStringBuffer + " 입니다. 현재 클라이언트 버전은 : " + CLIENT_VERSION + " 입니다." );

            int index4 = www.downloadHandler.text.IndexOf("NotifyNum : ", index3) + 12;

            string notifyNumString = www.downloadHandler.text.Substring(index4, 1);

            if(notifyNumString.Equals("0"))
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
        }
    }
}


