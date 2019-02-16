#define UDP

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

#if UDP
public partial class NetworkManager : MonoBehaviour
{
    string receiveData;
    IPEndPoint e;
    UdpClient u;
    //public IPAddress IPAddressBuffer;
    UDP_StateObject s;

    public class UDP_StateObject
    {
        public UdpClient u;
        public IPEndPoint e;
    }

    public static bool messageReceived = false;

    enum UDP_PROTOCOL : int
    {
            INVITE_FRIEND = 1
        , DEMAND_FRIEND = 2

        , RESULT_FRIEND = 7
        , ANNOUNCEMENT = 8  // 해당 사항은, 전체 유저에게 전송해야하므로, 큐형식이 아닌, 다르게 구현.

        , SEND_PORT = 9
        , CONFIRM_PORT = 10
    };

    public static void ProcessRecvUDPData(int InBuffer)
    {
        if (InBuffer == 1)
        {
            Debug.Log("UDP Message : 게임 초대를 받았습니다. ");
            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().OnUI_INVITE_FRIEND_UDP();
        }
        else if (InBuffer == 2)
        {
            Debug.Log("UDP Message : 친구 추가 요청을 받았습니다. ");
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData(PROTOCOL.DEMAND_MAKE_FRIEND_INFO);
        }
        else if (InBuffer == 3)
        {
            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>();
        }
        else if (InBuffer == 7)
        {
            Debug.Log("UDP Message : 친구 추가 요청에 대한 답변을 받았습니다. ");
            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().OnOffResultMakeFriendUI(true);
        }
        else if (InBuffer == 8)
        {
            Debug.Log("UDP Message : 친구 추가 요청에 대한 답변을 받았습니다. ");
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData(PROTOCOL.DEMAND_ANNOUNCEMENT);
        }
        else if (InBuffer == (int)UDP_PROTOCOL.CONFIRM_PORT)
        {
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().isServerRecvMyPort = true;
        }
    }

    public IEnumerator UDP_Receive()
    {
        //Debug.Log("ipAddr : " + ipAddr.GetAddressBytes().ToString());
        //IPHostEntry IPHost = Dns.GetHostEntry(Dns.GetHostName());// Dns.GetHostByName(Dns.GetHostName());

        // for External Ip Receive
        UnityWebRequest www = UnityWebRequest.Get("http://checkip.dyndns.org/"); // 나중에 GitPage로 바꾸기.
        yield return www.SendWebRequest();

        int index1 = www.downloadHandler.text.IndexOf("Current IP Address: ") + 20;
        int index2 = www.downloadHandler.text.IndexOf("<", index1);

        string external_IP = www.downloadHandler.text.Substring(index1, index2 - index1);

        Debug.Log("나의 외부 IP는 :" + external_IP.ToString());

        e = new IPEndPoint(/*IPAddress.Any*/ /*ipAddr*/ /*IPAddress.Loopback*/ /*"127.0.0.1"*/ System.Net.IPAddress.Parse(external_IP) /*System.Net.IPAddress.Parse(AWS_PUBLIC_IP)*/, 9002);
        u = new UdpClient(e);

        s = new UDP_StateObject()
        {
            e = e,
            u = u
        };

    }
    public void StartUDPCoroutine()
    {
        self.StartCoroutine(self.SendPortToServerCoroutine());
        self.StartCoroutine(self.UdpCoroutine());
    }

    public IEnumerator SendPortToServerCoroutine()
    {
        // for Hole Punching
        Debug.Log("홀펀칭 서버 어드레스는 :" + iP_ADDRESS);

        using (UdpClient c = new UdpClient(9002))
        {
            byte[] sendByte = new byte[50];

            byte[] idByte = Encoding.ASCII.GetBytes(ID);
            int idByteSize = idByte.Length;

            sendByte[0] = (byte)((UDP_PROTOCOL.SEND_PORT)); // UDP_PROTOCOL::SEND_PORT
            sendByte[1] = (byte)(idByteSize);

            for(int i = 0; i < idByteSize; ++i)
            {
                sendByte[2 + i] = idByte[i];
            }

            while (!isServerRecvMyPort)
            {
                c.Send(sendByte, idByteSize + 2, iP_ADDRESS, 9001);
                yield return new WaitForSeconds(1.0f);
            }
        }
    }

    public IEnumerator UdpCoroutine(/*IPAddress InIPAddress*/)
    {
        while (true)
        {
            u.BeginReceive(new AsyncCallback(UDP_ReceiveCallback), s);

            while (!messageReceived)
            {
                yield return new WaitForSeconds(1.0f);
                //Debug.Log("대기");
            }

            Debug.Log(" UDP Message를 받았습니다. : " + (int)(receiveData[0] ));
            if (self)
                ProcessRecvUDPData((int)(receiveData[0]));

            messageReceived = false;
        }
    }

    public void UDP_ReceiveCallback(IAsyncResult ar)
    {
        UdpClient u = (UdpClient)((UDP_StateObject)(ar.AsyncState)).u;
        IPEndPoint e = (IPEndPoint)((UDP_StateObject)(ar.AsyncState)).e;

        Byte[] receiveBytes = u.EndReceive(ar, ref e);
        receiveData = Encoding.ASCII.GetString(receiveBytes);

        Debug.Log(" UDP Message를 받았습니다. : " + (int)receiveData[0]);

        //Console.WriteLine("Received: {0}", receiveString);
        messageReceived = true;
    }
}
#endif
