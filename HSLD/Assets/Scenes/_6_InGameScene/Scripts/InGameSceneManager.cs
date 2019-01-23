using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public partial class InGameSceneManager : MonoBehaviour
{
    public int network_recvProtocolFlag;
    public int network_sendProtocol;

    // 지형 타입입니다.
    public int network_terrainType;

    // 바뀌어지는 테라리안의 개수입니다.
    public int network_changeTerrainCount;

    // 바뀌어진 테라리언들의 인덱스가 들어있는 컨테이너 입니다.
    public int[] network_terrainIndex = new int[12];
    public int[] recvTerrainIndex = new int[12]; // 여기는 받는 부분, 계속 쓸꺼, 자꾸만들면 손해여요.

    // 사용된 이벤트카드의 타입(인덱스) 입니다.
    public int network_eventCardType;

    // GameReady 여부에 따른 bool 변수.
    public bool isOnWaitGameReady;

    IEnumerator CoroutineHandle_Wait;
    IEnumerator CoroutineHandle_Play;

    private NetworkManager networkManager;
    
    // 변수 2개 추가했어요. - 181022 YSH
    [HideInInspector] public int recvDiceValue = 0;          //상대방 굴린 주사위 눈금
    [HideInInspector] public int recvTerrainType = 0;        //상대방이 뽑은 카드타입

    static bool isfirstSend = true;
    static bool isfirstRecv = true;

    void Start()
    {
        if (GameObject.Find("GameCores") == null)
            return;

        // -----
        GameObject.Find("GameCores").transform.Find("ClientBaseManager").GetComponent<ClientBaseManager>().OnOff_ClientBaseSpace(false);
        // -----
        StartForEmoji();

        for (int i = 0; i < 12; ++i)
        {
            recvTerrainIndex[i] = 0;
            network_terrainIndex[i] = 0;
        }

        networkManager = GameObject.Find("GameCores").transform.Find("NetworkManager").GetComponent<NetworkManager>();
        networkManager.inGameSceneManager = GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>(); // same , return this;

        network_recvProtocolFlag = 0;
        network_terrainType = 0;
        network_changeTerrainCount = 0;
        network_eventCardType = 0;
        network_sendProtocol = (int)PROTOCOL.VOID_GAME_STATE;

        CoroutineHandle_Wait = WaitCoroutine();
        CoroutineHandle_Play = InGameNetworkCoroutine();
        //StartCoroutine("InGameNetworkCoroutine");
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
        if (isfirstSend)
        {
            network_changeTerrainCount++;
            Debug.Log("network_changeTerrainCount : " + network_changeTerrainCount);
            isfirstSend = false;
        }
        network_sendProtocol = (int)PROTOCOL.NOTIFY_DICE_VALUE;
        //Debug.Log("주사위 보내는 값 : " + network_changeTerrainCount);
    }

    public void SendTerrainType(int InTerrainType)
    {
        network_terrainType = InTerrainType;
        network_sendProtocol = (int)PROTOCOL.NOTIFY_TERRAIN_TYPE;
    }

    //index만 ref으로 보냄. - 임시주석처리 181022.YSH 
    public void SendTerrainIndex(/*int InDiceValue, */int[] InTerrainIndex)
    {
        //network_changeTerrainCount = InDiceValue;
        //Debug.Log("!network_changeTerrainCount 의 값은 : " + AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer.Count + "입니다. ");
        Debug.Log("!InTerrainIndex.Length 의 값은 : " + InTerrainIndex.Length + "입니다. ");
        Debug.Log("!network_terrainIndex.Length 의 값은 : " + network_terrainIndex.Length + "입니다. ");

        for ( int i = 0; i < AllMeshController.myPlanet.GetComponent<AllMeshController>().PickContainer.Count; ++i)
        {
            network_terrainIndex[i] = InTerrainIndex[i];
            Debug.Log("InTerrainIndex 의 인덱스 " + i + " 값은 : " + InTerrainIndex[i] + "입니다. ");
        }
        // network_terrainIndex = InTerrainIndex;

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
        Debug.Log("1111");
        //상대방의 이벤트카드 갯수의 여부에따라 분기
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.ENEMYTURN_PICKINGLOC);
    }

    // 상대방이 굴린 주사위 눈금을 연출
    public void RecvDiceValue(int InDiceValue)
    {
        // 다이스 주사위 굴리는연출 / 결과 dsipaly        
        recvDiceValue = InDiceValue;

        //Debug.Log("주사위 왜 안뜨는거야 도대체 !! : "+recvDiceValue.ToString());
        //gameObject.GetComponent<TurnSystem>().DisplayTextMessage("상대의 주사위 눈금: "+ recvDiceValue.ToString() + " !!!", 5f);   //ref - 2f 수정.
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.ENEMYTURN_ROLLINGDICE);
    }

    // 상대방이 뽑은 카드를 연출
    public void RecvTerrainType(int InTerrainType)
    {
        // 카드 뽑는 연출 dsipaly
        recvTerrainType = InTerrainType;
        for (int i =0; i< gameObject.GetComponent<CardSystem>().cardSet.Length; i++)
        {
            if(gameObject.GetComponent<CardSystem>().cardSet[i].GetComponent<CardData>().data.cardIndex == InTerrainType)
            {
                Debug.Log("뽑은카드타입 왜 안뜨는거야 도대체 !! : " + gameObject.GetComponent<CardSystem>().cardSet[i].GetComponent<CardData>().data.cardName);
                //gameObject.GetComponent<TurnSystem>().DisplayTextMessage("상대가 뽑은 카드 타입 : " + gameObject.GetComponent<CardSystem>().cardSet[i].GetComponent<CardData>().data.cardName + " !!!", 5f);
                gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.ENEMYTURN_PICKINGCARD);
                break;
            }
        }
    }

    //ref으로 index만 받자 !
    public void RecvTerrainIndex(/*int InDiceValue, */ /*int[] InTerrainIndex*/)
    {
        if (recvTerrainType == 0)
        {
            Debug.Log("에러 : 1");
            return;
        }

        if(recvTerrainType == 0)
        {
            Debug.Log("에러 : 2");
            return;
        }
        //if (isfirstRecv)
        //{
        //    recvDiceValue++;
        //    Debug.Log("recvDiceValue : " + recvDiceValue);
        //    isfirstRecv = false;
        //}
        for (int i =0; i< recvDiceValue; i++)
        {
            if (AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]] == null)
            {
                Debug.Log("벗어나자.;;");
                break;
            }
            Debug.Log("recvDiceValue??" + recvDiceValue);
            Debug.Log("이름이뭐니??" + gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].name);
            if (AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFlag)
            {
                AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setFlag();
                AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;
                //AllMeshController.myPlanet.GetComponent<AllMeshController>().FlagContainer.Insert(1, gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]]);
                Debug.Log("상대방이 FLAG 획득" + recvTerrainIndex[i]);
                continue;
            }

            if (recvTerrainType == 1)
            {
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setModeration(Identify.ENEMY);
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;
            }
            else if (recvTerrainType == 2)
            {
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setBarren(Identify.ENEMY); // setModeration() -> setBarren()
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;
            }
            else if (recvTerrainType == 3)
            {
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setCold(Identify.ENEMY);
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;
            }
            else if (recvTerrainType == 4)
            {
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setSea(Identify.ENEMY);
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;
            }
            else if (recvTerrainType == 5)
            {
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setMountain(Identify.ENEMY);
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;
            }
            else
            {
                Debug.Log("에러 : 3");
            }
        }

        //gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.ENEMYTURN_PICKINGLOC);
        Camera.main.GetComponent<PCverPIcking>().FlagSetting();
        AllMeshController.myPlanet.GetComponent<AllMeshController>().AllPriorSetting(); // PriorSetting
        recvTerrainType = 0;
        recvDiceValue = 0;
    }

    //사용한 이벤트카드의 인덱스를 연출.
    public void RecvEventcardIndex(int InEventcardIndex)
    {
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.ENEMYTURN_PICKEVENTCARD);
    }

    //
    // 대기 상태를 시작할 때, 클라이언트에서 호출
    public void StartWaitCoroutine()
    {
        isOnWaitGameReady = false;

        StartCoroutine(CoroutineHandle_Wait);
    }

    // 대기 상태에서 게임 시작 상태가 시작될 때, 네트워크 매니저에서 호출.
    public void StartInGameCoroutine()
    {
        isOnWaitGameReady = true;

        StopCoroutine(CoroutineHandle_Wait);

        //while (networkManager.networkSyncLock == true)
        //{
        //    yield return new WaitForSeconds(0.03f); // True이면 이거 건들면 안 됌 감히 씬매니저주제에 디질라공.
        //}

        StartCoroutine(CoroutineHandle_Play);
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_DONE);
    }

    IEnumerator WaitCoroutine()
    {
        isOnWaitGameReady = false;

        while (true)
        {
            yield return new WaitForSeconds(1.0f);

            // 네트워크 순서를 무너뜨리지 않는 상태에서, SendExitMatching을 전송해야함.
            while (networkManager.networkSyncLock == true)
            {
                yield return new WaitForSeconds(0.03f); // True이면 이거 건들면 안 됌 감히 씬매니저주제에 디질라공.
            }
            // ---

            networkManager.SendData(PROTOCOL.NOTIFY_GAME_READY);
        }
    }

    IEnumerator InGameNetworkCoroutine()
    {
        while (true)
        {
            yield return new WaitForSeconds(1.0f);

            // 네트워크 순서를 무너뜨리지 않는 상태에서, SendExitMatching을 전송해야함.
            while (networkManager.networkSyncLock == true)
            {
                yield return new WaitForSeconds(0.03f); // True이면 이거 건들면 안 됌 감히 씬매니저주제에 디질라공.
            }
            // ---

            if (network_sendProtocol == (int)PROTOCOL.VOID_GAME_STATE)
            {
                if (selectedEmojiIndex > 0)
                    networkManager.SendData(PROTOCOL.NOTIFY_EMOJI);
                else
                    networkManager.SendData(PROTOCOL.VOID_GAME_STATE);
            }
            else
            {
                networkManager.SendData((PROTOCOL)network_sendProtocol); // RECV까지 자동.
                network_sendProtocol = (int)PROTOCOL.VOID_GAME_STATE; // 여기서, 쓰레드 개입일어나면, 데이터 날라간다... -> 수정
            }
        }
    }
}
