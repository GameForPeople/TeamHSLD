using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class InGameSceneManager : MonoBehaviour
{

    public int network_recvProtocolFlag;
    public int network_sendProtocol;

    // 지형 타입입니다.
    public int network_terrainType;

    // 바뀌어지는 테라리안의 개수입니다.
    public int network_changeTerrainCount;

    // 바뀌어진 테라리언들의 인덱스가 들어있는 컨테이너 입니다.
    public int[] network_terrainIndex = new int[12];

    // 사용된 이벤트카드의 타입(인덱스) 입니다.
    public int network_eventCardType;



    private NetworkManager networkManager;
    // Use this for initialization
    void Start()
    {
        if (GameObject.Find("GameCores") == null)
            return;

        networkManager = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();
        networkManager.inGameSceneManager = GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>(); // same , return this;

        network_recvProtocolFlag = 0;
        network_terrainType = 0;
        network_changeTerrainCount = 0;
        network_eventCardType = 0;

        StartCoroutine("InGameNetworkCoroutine");
    }

    public void SendData(int InMsg, int InTerrainType = -1, int InChangeTerrainCount = -1, int[] InTerrainIndex = null, int InEventCardType = -1)
    {
        network_recvProtocolFlag = 0;
        // 이부분에서 일정확률로 동기화 안될 수도 있음. // 일부 공격자 메세지 생략될 수 있음.

        // InMsg에 따라, 대입 처리 바꿀것인가 안바꿀것인가..
        network_terrainType = InTerrainType;
        network_changeTerrainCount = InChangeTerrainCount;
        network_terrainIndex = InTerrainIndex;
        network_eventCardType = InEventCardType;


        network_sendProtocol = InMsg;
        //networkManager.SendData(InMsg);
    }

    // 동기화가 된다는 가능성이 어느정도 되는가, 네트워크 지연에 따른 데이터 삭제시는 어쩔것인가. -> 몰러
    public bool GetRecvData(ref int retRecvProtocol, ref int retTerrainType, ref int retChangeTerrainCount, ref int[] retTerrainIndex, ref int retEventCardType)
    {
        if (network_recvProtocolFlag == 0)
        {
            return false;
        }
        else
        {
            retTerrainType = network_terrainType;
            retChangeTerrainCount = network_changeTerrainCount;
            retTerrainIndex = network_terrainIndex;
            retEventCardType = network_eventCardType;

            retRecvProtocol = network_recvProtocolFlag;
            network_recvProtocolFlag = 0;

            return true;
        }
    }

    public void ChangeTurn()
    {
        gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN_NOTYETFLAG;
    }

    IEnumerator InGameNetworkCoroutine()
    {
        while (true)
        {
            network_sendProtocol = (int)PROTOCOL.DEMAND_GAME_STATE;

            yield return new WaitForSeconds(1.0f);

            networkManager.SendData(network_sendProtocol); // REcv까지 자동.
        }
    }
}
