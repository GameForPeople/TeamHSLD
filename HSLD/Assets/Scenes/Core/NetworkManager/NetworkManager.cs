
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

// 클라이언트 현재 네트워크 동기방식 -> 턴제라 그냥 써도 될듯 한데, 프레임 드랍 등, 문제되면 비동기방식으로 변경 필요

// State object for receiving data from remote device.

public partial class NetworkManager : MonoBehaviour
{
    // 안녕하세요! 저는 고냥 몸뚱아리여요! 아무것도 없지요!

    // 1. 변수 및 Enum 등 선언
    // NetworkManager_Declaration

    // 2. Rece(TCP) 관련
    // NetworkManager_Recv

    // 3. Send(TCP) 관련
    // NetworkManager_Send

    // 4. UDP 관련
    // NetworkManager_UDP

    // 5. Parsing Server IP, 등등.
    // NetworkManager_Others

    void Awake()
    {
        DontDestroyOnLoad(gameObject);
    }
}




