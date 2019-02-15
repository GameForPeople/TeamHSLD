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
    }

    public void UDP_Receive()
    {
        e = new IPEndPoint(IPAddress.Any /*ipAddr*/ /*IPAddress.Loopback*/ /*"127.0.0.1"*/ /*System.Net.IPAddress.Parse("127.0.0.1"),*/ /*System.Net.IPAddress.Parse(AWS_PUBLIC_IP)*/, 9001);
        u = new UdpClient(e);

        s = new UDP_StateObject()
        {
            e = e,
            u = u
        };

        self.StartCoroutine(self.UdpCoroutine());
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
