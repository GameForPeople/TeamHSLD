using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameSceneManager : MonoBehaviour {

    public int          network_recvProtocolFlag;

    // 지형 타입입니다.
    public int          network_terrainType;

    // 바뀌어지는 테라리안의 개수입니다.
    public int          network_changeTerrainCount;

    // 바뀌어진 테라리언들의 인덱스가 들어있는 컨테이너 입니다.
    public int[]        network_terrainIndex = new int[12];

    // 사용된 이벤트카드의 타입(인덱스) 입니다.
    public int          network_eventCardType;

    private NetworkManager networkManager;
    // Use this for initialization
    void Start ()
    {
        networkManager = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();
        networkManager.inGameSceneManager = GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>(); // same , return this;

        network_recvProtocolFlag = 0;
        network_terrainType = 0;
        network_changeTerrainCount = 0;
        network_eventCardType = 0;
    }

    void SendData (int InMsg)
    {
        // 이부분에서 일정확률로 동기화 안될 수도 있음. // 일부 공격자 메세지 생략될 수 있음.
        network_recvProtocolFlag = 0;

        networkManager.SendData(InMsg);
    }

    // 동기화가 된다는 가능성
    int GetRecvProtocol()
    {
        if (network_recvProtocolFlag == 0)
        {
            // 네트워크 지연처리 필요함.
            // 1. 상대방 - 서버
            // 2. 서버 - 나...
        }

        return network_recvProtocolFlag;
    }
}
