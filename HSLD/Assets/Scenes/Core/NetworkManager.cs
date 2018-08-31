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


// 아직 코드 보지 마세요....테스트 코드라.. 리팩토링 안해서 눈 썩어요..
// 클라이언트 현재 네트워크 동기방식 -> 턴제라 그냥 써도 될듯 한데, 프레임 드랍 등, 문제되면 비동기방식으로 변경 필요

enum PROTOCOL : int
{
    DEMAND_LOGIN        =   100   ,
    FAIL_LOGIN          =   101   ,
    PERMIT_LOGIN        =   102   ,
    DEMAND_MAKEROOM     =   301   ,
    PERMIT_MAKEROOM     =   302   ,
    DEMAND_JOINROOM     =   303   ,
    PERMIT_JOINROOM     =   304   ,
    FAIL_JOINROOM       =   305   ,
    DEMAND_ROOMHOST     =   400   ,
    ROOMSTATE_VOID      =   410   ,
    ROOMSTATE_GUESTIN   =   411   ,
    DEMAND_GAMESTATE    =   500   
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


    private const string iP_ADDRESS = "119.193.229.172";
    private const int SERVER_PORT = 9000;

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

    public object _obj = new object();

    //public GameObject m_scenenManager;

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
            if (InMsg == (int)PROTOCOL.DEMAND_GAMESTATE)
            {
                byte[] sendDamandGameState = BitConverter.GetBytes((int)400);
                socket.Send(sendDamandGameState);
            }
            else if (InMsg == (int)PROTOCOL.DEMAND_LOGIN)
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
            else if (InMsg == (int)PROTOCOL.DEMAND_ROOMHOST)
            {
                Buffer.BlockCopy(BitConverter.GetBytes((int)PROTOCOL.DEMAND_ROOMHOST), 0, DataSendBuffer, 0, 4);
                socket.Send(DataSendBuffer, 4, SocketFlags.None);
            }

            RecvProcess();
        }
        else
        {
            Debug.Log("주의 : OnlyClientTest 환경에서, SendData가 호출되었습니다.");
        }
    }

    public void RecvProcess()
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

    public void ProcessRecvData()
    {
        if (recvType > (int)PROTOCOL.DEMAND_GAMESTATE)
        {
           
        }
        else if (recvType == (int)PROTOCOL.FAIL_LOGIN)
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
        else if (recvType == (int)PROTOCOL.ROOMSTATE_VOID)
        {

        }
        else if (recvType == (int)PROTOCOL.ROOMSTATE_GUESTIN)
        {
            int idSizeBuffer = BitConverter.ToInt32(DataRecvBuffer, 4);
            enemyId = Encoding.Default.GetString(DataRecvBuffer, 8, idSizeBuffer);

            GameObject.Find("RoomSceneManager").GetComponent<RoomSceneManager>().GuestJoinRoom();
        }

        recvType = 0;
    }
}


