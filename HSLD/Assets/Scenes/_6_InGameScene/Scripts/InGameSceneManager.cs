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
    public int[] network_terrainIndex = new int[14];
    public int[] recvTerrainIndex = new int[14]; // 여기는 받는 부분, 계속 쓸꺼, 자꾸만들면 손해여요.

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


    //YSH 190204
    public int diceValueForLoop;

    //최적화 스크립트
    private MissionManager missionManager;

    private int maximumCnt = 0;
    private int[] currentCnt = new int[3];


    void Start()
    {
        if (GameObject.Find("GameCores") == null)
            return;

        missionManager = gameObject.GetComponent<MissionManager>();
        for (int i = 0; i < currentCnt.Length; i++)
            currentCnt[i] = 0;

        // -----
        GameObject.Find("GameCores").transform.Find("ClientBaseManager").GetComponent<ClientBaseManager>().OnOff_ClientBaseSpace(false);
        // -----

        StartForEmoji();
        StartForEvent();

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

        //YSH 190204
        diceValueForLoop = (InDiceValue % 10) + (InDiceValue / 10);
        Debug.Log("(InDiceValue % 10) : " + (InDiceValue % 10) + "(InDiceValue / 10)" + (InDiceValue / 10));

        GameObject.Find("Canvas_Debug").transform.Find("Text").GetComponent<Text>().text = "주사위값은 " + network_changeTerrainCount.ToString();

        if (isfirstSend)
        {
            //network_changeTerrainCount++;
            Debug.Log("network_changeTerrainCount : " + network_changeTerrainCount);
            isfirstSend = false;
        }
        network_sendProtocol = (int)PROTOCOL.NOTIFY_DICE_VALUE;
        //Debug.Log("주사위 보내는 값 : " + network_changeTerrainCount);
        Debug.Log("주사위 값을 보냅니다. 해당 값은 : " + InDiceValue.ToString());
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
        //Debug.Log("!InTerrainIndex.Length 의 값은 : " + InTerrainIndex.Length + "입니다. ");
        //Debug.Log("!network_terrainIndex.Length 의 값은 : " + network_terrainIndex.Length + "입니다. ");

        for ( int i = 0; i < diceValueForLoop; ++i)
        {
            network_terrainIndex[i] = InTerrainIndex[i];
            Debug.Log("InTerrainIndex 의 인덱스 " + i + " 값은 : " + InTerrainIndex[i] + "입니다. ");
        }
        // network_terrainIndex = InTerrainIndex;

        network_sendProtocol = (int)PROTOCOL.NOTIFY_TERRAIN_INDEXS;
    }

    public void SendGameEnd()
    {
        network_sendProtocol = (int)PROTOCOL.NOTIFY_GAME_END;
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
        //상대방의 이벤트카드 갯수의 여부에따라 분기
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.ENEMYTURN_PICKINGEVENTCARDLOC);
    }

    // 상대방이 굴린 주사위 눈금을 연출
    public void RecvDiceValue(int InDiceValue)
    {
        Debug.Log("주사위 값을 받았습니다. 해당 값은 : " + InDiceValue.ToString());
        GameObject.Find("Canvas_Debug").transform.Find("Text").GetComponent<Text>().text = "주사위값은 " + InDiceValue.ToString();

        // 다이스 주사위 굴리는연출 / 결과 dsipaly        
        recvDiceValue = InDiceValue;

        //YSH 190204
        diceValueForLoop = (InDiceValue % 10) + (InDiceValue / 10);

        GameObject.Find("DiceManager").GetComponent<DiceObject>().DiceSystem_Roll(InDiceValue / 10, InDiceValue % 10);
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
        //Debug.Log("상대지형선택끝남");
        //if (isfirstRecv)
        //{
        //    recvDiceValue++;
        //    Debug.Log("recvDiceValue : " + recvDiceValue);
        //    isfirstRecv = false;
        //}
        for (int i =0; i< diceValueForLoop; i++)
        {
            if (AllMeshController.myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]] == null)
            {
                Debug.Log("벗어나자.;;");
                break;
            }
            Debug.Log("recvDiceValue??" + diceValueForLoop);
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

            switch(recvTerrainType)
            {
                case 1:
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setModeration(Identify.ENEMY);
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;

                    //미션 - 500
                    if (MissionManager.selectedSubMissionIndex == 3 && diceValueForLoop > 6)
                        missionManager.SubMissionCounting(1, 0, Identify.ENEMY);

                    ////미션 - 420
                    //if (MissionManager.selectedSubMissionIndex == 2 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                    //    missionManager.SubMissionCounting(1, 4, Identify.ENEMY);

                    //미션 - 510
                    if (MissionManager.selectedSubMissionIndex == 3)
                        missionManager.SubMissionCounting(1, 2, Identify.ENEMY);

                    //미션 - 301
                    if (MissionManager.selectedSubMissionIndex == 1)
                    {
                        currentCnt[0] += CameraController.DiceCount;
                        if (currentCnt[0] > missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt)
                            currentCnt[0] = missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt;

                        maximumCnt = 0;

                        for (int j = 0; j < currentCnt.Length; j++)
                            if (maximumCnt < currentCnt[j])
                                maximumCnt = currentCnt[j];
                            else
                                continue;

                        if (currentCnt[0] == maximumCnt)
                            missionManager.SubMissionCounting(maximumCnt, 1, Identify.ENEMY);
                    }


                    break;
                case 2:
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setBarren(Identify.ENEMY); // setModeration() -> setBarren()
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;

                    //미션 - 210
                    if (MissionManager.selectedSubMissionIndex == 0)
                        missionManager.SubMissionCounting(1, 2, Identify.ENEMY);

                    //미션 - 400
                    if (MissionManager.selectedSubMissionIndex == 2 && diceValueForLoop > 6)
                        missionManager.SubMissionCounting(1, 0, Identify.ENEMY);

                    ////미션 - 420
                    //if (MissionManager.selectedSubMissionIndex == 2 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                    //    missionManager.SubMissionCounting(1, 4, Identify.ENEMY);

                    //미션 - 510
                    if (MissionManager.selectedSubMissionIndex == 3)
                        missionManager.SubMissionCounting(1, 2, Identify.ENEMY);

                    //미션 - 301
                    if (MissionManager.selectedSubMissionIndex == 1)
                    {
                        currentCnt[1] += CameraController.DiceCount;
                        if (currentCnt[1] > missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt)
                            currentCnt[1] = missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt;
                        maximumCnt = 0;

                        for (int j = 0; j < currentCnt.Length; i++)
                            if (maximumCnt < currentCnt[j])
                                maximumCnt = currentCnt[j];
                            else
                                continue;

                        if (currentCnt[1] == maximumCnt)
                            missionManager.SubMissionCounting(maximumCnt, 1, Identify.ENEMY);
                    }
                    break;
                case 3:
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setCold(Identify.ENEMY);
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;

                    //미션 - 200
                    if (MissionManager.selectedSubMissionIndex == 0 && diceValueForLoop > 6)
                        missionManager.SubMissionCounting(1, 0, Identify.ENEMY);

                    ////미션 - 420
                    //if (MissionManager.selectedSubMissionIndex == 2 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                    //    missionManager.SubMissionCounting(1, 4, Identify.ENEMY);

                    //미션 - 510
                    if (MissionManager.selectedSubMissionIndex == 3)
                        missionManager.SubMissionCounting(1, 2, Identify.ENEMY);

                    //미션 - 301
                    if (MissionManager.selectedSubMissionIndex == 1)
                    {
                        currentCnt[2] += CameraController.DiceCount;
                        if (currentCnt[2] > missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt)
                            currentCnt[2] = missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt;

                        maximumCnt = 0;

                        for (int j = 0; j < currentCnt.Length; j++)
                            if (maximumCnt < currentCnt[j])
                                maximumCnt = currentCnt[j];
                            else
                                continue;

                        if (currentCnt[2] == maximumCnt)
                            missionManager.SubMissionCounting(maximumCnt, 1, Identify.ENEMY);
                    }
                    break;
                case 4:
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setSea(Identify.ENEMY);
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;

                    ////미션 - 520
                    //if (MissionManager.selectedSubMissionIndex == 3 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                    //    missionManager.SubMissionCounting(1, 4, Identify.ENEMY);
                    break;
                case 5:
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().setMountain(Identify.ENEMY);
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isFixed = true;
                    gameObject.GetComponent<FlagSystem>().myPlanet.GetComponent<AllMeshController>().AllContainer[recvTerrainIndex[i]].GetComponent<MeshController>().isLandingSign = true;

                    //미션 - 401
                    if (MissionManager.selectedSubMissionIndex == 2 && diceValueForLoop > 6)
                        missionManager.SubMissionCounting(1, 1, Identify.ENEMY);

                    ////미션 - 520
                    //if (MissionManager.selectedSubMissionIndex == 3 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                    //    missionManager.SubMissionCounting(1, 4, Identify.ENEMY);
                    break;
            }
        }
        Camera.main.GetComponent<PCverPIcking>().FlagSetting();
        AllMeshController.myPlanet.GetComponent<AllMeshController>().AllPriorSetting(); // PriorSetting
        recvTerrainType = 0;
    }

    // 마 졋나 이겻나
    public void RecvGameEnd(bool InIsWin)
    {
        if(InIsWin)
        {
            gameObject.GetComponent<FlowSystem>().GameOver(true, 0);
        }
        else
        {
            gameObject.GetComponent<FlowSystem>().GameOver(false, 0);
        }
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

        // 이거 FlowSystem 종속으로 변환시킴.
        //gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_DONE);
    }

    IEnumerator WaitCoroutine()
    {
        isOnWaitGameReady = false;

        while (true)
        {
            yield return new WaitForSeconds(1.0f);

            Debug.Log("Send - Wait");

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

            Debug.Log("Send - InGame");

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
