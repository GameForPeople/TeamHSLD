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
        if (GameObject.Find("GameCores") == null)
            return;
        networkManager = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();
        networkManager.inGameSceneManager = GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>(); // same , return this;

        network_recvProtocolFlag = 0;
        network_terrainType = 0;
        network_changeTerrainCount = 0;
        network_eventCardType = 0;
    }

    public void SendData (int InMsg, int InTerrainType, int InChangeTerrainCount, int[] InTerrainIndex, int InEventCardType)
    {
        network_recvProtocolFlag = 0;

        network_terrainType = InTerrainType;
        network_changeTerrainCount = InChangeTerrainCount;
        network_terrainIndex = InTerrainIndex;
        network_eventCardType = InEventCardType;

        networkManager.SendData(InMsg);
        // 이부분에서 일정확률로 동기화 안될 수도 있음. // 일부 공격자 메세지 생략될 수 있음.
    }

    // 동기화가 된다는 가능성이 어느정도 되는가, 네트워크 지연에 따른 데이터 삭제시는 어쩔것인가.
    public int GetRecvProtocol()
    {
        return network_recvProtocolFlag;
    }

    public void GetRecvData(ref int retTerrainType, ref int retChangeTerrainCount, ref int[] retTerrainIndex, ref int retEventCardType)
    {
        retTerrainType = network_terrainType;
        retChangeTerrainCount = network_changeTerrainCount;
        retTerrainIndex = network_terrainIndex;
        retEventCardType = network_eventCardType;

        network_recvProtocolFlag = 0;
    }

    //IEnumerator NetworkRecvSyncCoroutine(ref bool retIsRecv)
    //{
    //    while (network_recvProtocolFlag == 0)
    //    {
    //        // 네트워크 지연처리 필요함.
    //        // 1. 상대방 - 서버
    //        // 2. 서버 - 나...
    //    }
    //}
}
