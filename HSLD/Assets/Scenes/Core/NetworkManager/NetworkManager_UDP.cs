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
    //public IPAddress IPAddressBuffer;

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
            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>().OnUI_INVITE_FRIEND_UDP();
        }
        else if (InBuffer == 2)
        {
            GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().SendData((int)PROTOCOL.DEMAND_MAKE_FRIEND_INFO);
        }
        else if (InBuffer == 3)
        {
            GameObject.Find("GameCores").transform.Find("CoreUIManager").GetComponent<CoreUIManager>();
        }
    }

    public void UDP_Receive()
    {
        //IPAddressBuffer = InIPAddress;

        // Receive a message and write it to the console.
        IPEndPoint e = new IPEndPoint(/*IPAddress.Any*/ ipAddr, 9001);
        UdpClient u = new UdpClient(e);

        UDP_StateObject s = new UDP_StateObject()
        {
            e = e,
            u = u
        };

        Debug.Log("생성한 UDP 포트는 : " + s.e.ToString());
        
        u.BeginReceive(new AsyncCallback(UDP_ReceiveCallback), s);

        self.StartCoroutine(self.UdpCoroutine());
    }

    public IEnumerator UdpCoroutine(/*IPAddress InIPAddress*/)
    {
        while (true)
        {
            while (!messageReceived)
            {
                yield return new WaitForSeconds(1.0f);
                //Debug.Log("대기");
            }

            //Debug.Log(" UDP Message를 받았습니다. : ");
            //GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>().
            if (self)
                ProcessRecvUDPData((int)(receiveData[0]));
            //ProcessRecvUDPData((int)receiveData[0]);
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
