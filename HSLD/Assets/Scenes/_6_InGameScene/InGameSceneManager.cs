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

    //public void SendData(int InMsg, int InTerrainType = -1, int InChangeTerrainCount = -1, int[] InTerrainIndex = null, int InEventCardType = -1)
    //{
    //    //network_recvProtocolFlag = 0;
    //    // 이부분에서 일정확률로 동기화 안될 수도 있음. // 일부 공격자 메세지 생략될 수 있음.
    //
    //    // InMsg에 따라, 대입 처리 바꿀것인가 안바꿀것인가..
    //    network_terrainType = InTerrainType;
    //    network_changeTerrainCount = InChangeTerrainCount;
    //    network_terrainIndex = InTerrainIndex;
    //    network_eventCardType = InEventCardType;
    //
    //    network_sendProtocol = InMsg;
    //    //networkManager.SendData(InMsg);
    //}

    public void SendChangeTurn()
    {
        network_sendProtocol = (int)PROTOCOL.NOTIFY_CHANGE_TURN;
    }

    public void SendDiceValue(int InDiceValue)
    {
        network_changeTerrainCount = InDiceValue;
        network_sendProtocol = (int)PROTOCOL.NOTIFY_DICE_VALUE;
    }

    public void SendTerrainType(int InTerrainType)
    {
        network_terrainType = InTerrainType;
        network_sendProtocol = (int)PROTOCOL.NOTIFY_TERRAIN_TYPE;
    }

    public void SendTerrainType(int InDiceValue, int[] InTerrainIndex)
    {
        network_changeTerrainCount = InDiceValue;
        network_terrainIndex = InTerrainIndex;
        network_sendProtocol = (int)PROTOCOL.NOTIFY_TERRAIN_INDEXS;
    }

    public void SendEventcardIndex(int InEventcardIndex)
    {
        network_eventCardType = InEventcardIndex;
        network_sendProtocol = (int)PROTOCOL.NOTIFY_EVENTCARD_INDEX;
    }
    // 동기화가 된다는 가능성이 어느정도 되는가, 네트워크 지연에 따른 데이터 삭제시는 어쩔것인가. -> 몰러
    //public bool GetRecvData(ref int retRecvProtocol, ref int retTerrainType, ref int retChangeTerrainCount, ref int[] retTerrainIndex, ref int retEventCardType)
    //{
    //    if (network_recvProtocolFlag == 0)
    //    {
    //        return false;
    //    }
    //    else
    //    {
    //        retTerrainType = network_terrainType;
    //        retChangeTerrainCount = network_changeTerrainCount;
    //        retTerrainIndex = network_terrainIndex;
    //        retEventCardType = network_eventCardType;
    //
    //        retRecvProtocol = network_recvProtocolFlag;
    //        network_recvProtocolFlag = 0;
    //
    //        return true;
    //    }
    //}

    public void RecvChangeTurn()
    {
        gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN_NOTYETFLAG;
    }

    public void RecvDiceValue(int InDiceValue)
    {

    }

    public void RecvTerrainType(int InTerrainType)
    {

    }

    public void RecvTerrainType(int InDiceValue, int[] InTerrainIndex)
    {

    }

    public void RecvEventcardIndex(int InEventcardIndex)
    {

    }

    IEnumerator InGameNetworkCoroutine()
    {
        while (true)
        {
            yield return new WaitForSeconds(1.0f);

            if (network_sendProtocol == (int)PROTOCOL.VOID_GAME_STATE)
            {
                networkManager.SendData((int)PROTOCOL.VOID_GAME_STATE);
            }
            else
            {
                networkManager.SendData(network_sendProtocol); // RECV까지 자동.
                network_sendProtocol = (int)PROTOCOL.VOID_GAME_STATE; // 여기서, 쓰레드 개입일어나면, 데이터 날라간다... -> 수정
            }
        }
    }
}
