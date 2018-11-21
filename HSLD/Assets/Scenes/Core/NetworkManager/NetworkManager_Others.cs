//#define UDP

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

public partial class NetworkManager : MonoBehaviour {

    // Use this for initialization
    public void StartNetworkFunction()
    {
        isOnNetwork = true;

        // Socket Create
        socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.SendTimeout, 10000);
        socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReceiveTimeout, 10000);

        // Socket connect.
        IPAddress ipAddr;
        try
        {
            ipAddr = System.Net.IPAddress.Parse(iP_ADDRESS);
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

#if UDP
        UDP_Receive(ipAddr);
#endif

        //SendData(100);
        //m_scenenManager = GameObject.Find("SceneManager");
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
