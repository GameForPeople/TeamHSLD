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

    // 사용자가 턴을 종료하면 나의턴이 됩니다.
    public void RecvChangeTurn()
    {
        //거점을 정복했는지 여부에따라서 분기

        gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN_NOTYETFLAG;
        //gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN_GETFLAG;
    }

    // 상대방이 굴린 주사위 눈금을 연출
    public void RecvDiceValue(int InDiceValue)
    {
        // 다이스 주사위 굴리는연출 / 결과 dsipaly
    }

    // 상대방이 뽑은 카드를 연출
    public void RecvTerrainType(int InTerrainType)
    {
        // 카드 뽑는 연출 dsipaly
    }

    // 상대방이 변경하는 지형의 인덱스(0 - 320)와 눈금을 출력합니다. 
    // 이게 이해가 안가는데 .. InDiceValue를 ref으로 잡은것은 RecvDiceValue() 이함수하고 중복되는 일을 하는것같고. 지형의 인덱스와 terrainType을 받아야하는거 아냐 ?
    // 함수 역할이 뭔지 모르겠어 .. 상대방이 지형을 수정하면 그것을 적용하는것이 아닌지 ..?
    public void RecvTerrainIndex(int InDiceValue, int[] InTerrainIndex)
    {

    }

    //사용한 이벤트카드의 인덱스를 연출.
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
