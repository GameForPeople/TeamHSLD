using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class TutorialFlowSystem : MonoBehaviour
{
    public FLOW currentFlow;

    public GameObject cardSetCanvas;
    public GameObject turnSetCanvas;
    public GameObject readyCanvas;
    public GameObject matchingCompleteCanvas;
    //public GameObject spinCanvas;
    public GameObject diceCanvas;
    public GameObject setTerrainCanvas;
    public GameObject turnTimerImg;
    public Transform missionSetParentTransform;
    public GameObject missionCanvas;
    public GameObject displayText;
    public GameObject displayTextImg;
    //public GameObject enemyTurnPassObj;
    public GameObject resultUICanvas;
    public GameObject diceCntingUI;

    private float time_;
    private int randomVal;

    private int maximumCnt = 0;
    private int[] currentCnt = new int[3];

    //최적화 스크립트
    private TutorialCardSystem cardSystem;
    private EventCardManager eventCardManager;
    private TutorialTurnSystem turnSystem;
    private MissionManager missionManager;
    private BuildOnPlanet buildOnPlanet;
    private Transform planetTrans;
    private TerrainGainCounting terrainGainCounting;

    static public string finalTerrainName;

    private void Start()
    {
        SoundManager.instance_.BGMChange(SoundManager.instance_.clips[14]);

        for (int i = 0; i < currentCnt.Length; i++)
            currentCnt[i] = 0;
        
        cardSystem = gameObject.GetComponent<TutorialCardSystem>();
        eventCardManager = gameObject.GetComponent<EventCardManager>();
        turnSystem = gameObject.GetComponent<TutorialTurnSystem>();
        missionManager = gameObject.GetComponent<MissionManager>();
        buildOnPlanet = gameObject.GetComponent<BuildOnPlanet>();
        planetTrans = GameObject.FindWithTag("InGamePlanet").transform;
        terrainGainCounting = gameObject.GetComponent<TerrainGainCounting>();

        //gameObject.GetComponent<TutorialManager>().currentTutorial = TUTORIAL.INGAME_ROLLINGDICE;
        //currentFlow = FLOW.READY_SETCARD;
        //turnSystem.currentTurn = TURN.MYTURN;
        //TutorialTurnSystem.startTurn = TURN.MYTURN;
        //FlowChange(FLOW.READY_SETCARD);
    }

    //이벤트 연출시간이 끝난다음에 다음 상태 진행.
    IEnumerator DisplayEventWaitingTime(FLOW beforeFlow, float time)
    {
        currentFlow = FLOW.DISPLAYANIMATION_WAITING;
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            yield return new WaitForEndOfFrame();
            if (time_ > time)
                break;
        }

        switch (beforeFlow)
        {
            case FLOW.TO_ROLLINGDICE:
                currentFlow = FLOW.TO_PICKINGCARD;
                diceCntingUI.SetActive(true);
                if(gameObject.GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_ROLLINGDICE))
                    gameObject.GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.INGAME_SELECTTERRAINCARD);
                break;
            case FLOW.TO_PICKINGLOC:
                TurnSystem.isSetTerrainDone = false;
                diceCntingUI.SetActive(false);
                //init
                cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt -= 1;
                cardSystem.CardCntUpdate();
                cardSystem.pickedCard = null;
                cardSystem.CardPosInit();

                //이벤트카드로 갈지 말지 분기
                if (DiceSystem.isDouble /*|| 서브미션 달성시*/ && PCverPIcking.isDominatedConfirm)
                {
                    currentFlow = FLOW.TO_PICKEVENTCARD;
                    eventCardManager.EventCardInstate();

                }
                else
                {
                    if (!gameObject.GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.ENDGAME_CONDITIONTOWIN))
                    {
                        displayTextImg.SetActive(true);
                        displayTextImg.GetComponent<DisplayTextImg>().Performance(displayTextImg.GetComponent<DisplayTextImg>().sprs[2]);

                        yield return new WaitForSeconds(2.5f);
                        currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
                        turnSystem.currentTurn = TURN.ENEMYTURN;
                        turnSystem.TurnSet();
                    }
                }
                break;
            case FLOW.TO_PICKINGEVENTCARDLOC:
                currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
                turnSystem.currentTurn = TURN.ENEMYTURN;
                turnSystem.TurnSet();
                break;
            case FLOW.ENEMYTURN_ROLLINGDICE:
                currentFlow = FLOW.ENEMYTURN_PICKINGCARD;
                break;
            case FLOW.ENEMYTURN_PICKINGLOC:
                //상대방이 이벤트카드를 뽑았다. - 더블일경우
                if ((GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().recvDiceValue % 10) == (GameObject.Find("InGameSceneManager").GetComponent<InGameSceneManager>().recvDiceValue / 10))
                {
                    currentFlow = FLOW.ENEMYTURN_PICKEVENTCARD;
                }
                else
                {
                    displayTextImg.SetActive(true);
                    displayTextImg.GetComponent<DisplayTextImg>().Performance(displayTextImg.GetComponent<DisplayTextImg>().sprs[1]);
                    yield return new WaitForSeconds(2.5f);

                    currentFlow = FLOW.TO_ROLLINGDICE;
                    turnSystem.currentTurn = TURN.MYTURN;
                    turnSystem.TurnSet();
                }
                break;

            case FLOW.ENEMYTURN_PICKEVENTCARD:
                eventCardManager.EnemyEventCardOff();
                eventCardManager.DefenseCard();
                break;
            case FLOW.ENEMYTURN_PICKINGEVENTCARDLOC:
                currentFlow = FLOW.TO_ROLLINGDICE;
                turnSystem.currentTurn = TURN.MYTURN;
                turnSystem.TurnSet();
                break;
                //case FLOW.READY_DONE:
                //    //turnSystem.TurnSet();
                //    break;
        }
    }

    IEnumerator DisplayLoadingCor()
    {
        displayTextImg.SetActive(true);
        displayTextImg.GetComponent<DisplayTextImg>().Performance(displayTextImg.GetComponent<DisplayTextImg>().sprs[0]);
        yield return new WaitForSeconds(2.5f);

        displayTextImg.SetActive(true);
        if (turnSystem.currentTurn.Equals(TURN.MYTURN))
            displayTextImg.GetComponent<DisplayTextImg>().Performance(displayTextImg.GetComponent<DisplayTextImg>().sprs[1]);
        else
            displayTextImg.GetComponent<DisplayTextImg>().Performance(displayTextImg.GetComponent<DisplayTextImg>().sprs[2]);

        yield return new WaitForSeconds(2.5f);
        FlowChange(FLOW.READY_DONE);
    }

    IEnumerator DiceActiveOff()
    {
        yield return new WaitForSeconds(1f);
        diceCanvas.SetActive(false);
    }
    public void FlowChange(FLOW doneFlow)
    {
        switch (doneFlow)
        {
            case FLOW.READY_MATCHINGCOMPLETE:
                diceCntingUI.SetActive(false);
                matchingCompleteCanvas.SetActive(false);
                turnSetCanvas.SetActive(true);
                gameObject.GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.READY_SELECTORDERCARD);
                currentFlow = FLOW.READY_TURNORDER;
                break;
            case FLOW.READY_TURNORDER:
                turnSetCanvas.SetActive(false);
                cardSetCanvas.SetActive(true);
                gameObject.GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.READY_SELECTCARD);
                currentFlow = FLOW.READY_SETCARD;
                break;
            case FLOW.READY_SETCARD:
                readyCanvas.SetActive(false);
                cardSetCanvas.SetActive(false);
                //tmpAnimationImage.SetActive(true);
                //spinCanvas.SetActive(true);
                //readyCanvas.SetActive(false);

                //init - card Cnt Update
                cardSystem.CardCntUpdate();
                StartCoroutine(DisplayLoadingCor());
                break;
            case FLOW.READY_DONE:
                gameObject.GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.INGAME_ROLLINGDICE);
                turnSystem.TurnSet();
                //StartCoroutine(DisplayEventWaitingTime(FLOW.READY_DONE, 2.5f, false));    // <<< 여기  5라는 숫자를 바꾸면댐
                break;
            case FLOW.READY_WAITING:
                currentFlow = FLOW.TO_ROLLINGDICE;
                break;
            case FLOW.TO_PICKINGCARD:
                
                currentFlow = FLOW.TO_PICKINGLOC;
                break;
            case FLOW.TO_ROLLINGDICE:
                SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[8], 1.0f);

                //애니메이션 여기
                StartCoroutine(DiceActiveOff());
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_ROLLINGDICE, 2));    // <<< 여기  5라는 숫자를 바꾸면댐
                break;

            //이벤트카드가 없다면 바로 대기상태로 변경
            case FLOW.TO_PICKINGLOC:
                //애니메이션 여기
                TurnSystem.isSetTerrainDone = true;
                MeshController.isMakeIsland = false;

                //Debug.Log("카드 인덱스 : " + cardSystem.pickedCard.GetComponent<CardData>().data.cardIndex + " / 다이스 주사위 눈금 : " + CameraController.DiceCount);
                //Debug.Log(cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt);

                switch (cardSystem.pickedCard.GetComponent<CardData>().data.cardIndex)
                {
                    //비옥 - 병아리
                    case 1:
                        //미션 - 500
                        if (MissionManager.selectedSubMissionIndex == 3 && CameraController.DiceCount > 6)
                            missionManager.SubMissionCounting(1, 0);

                        //미션 - 420
                        if (MissionManager.selectedSubMissionIndex == 2 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                            missionManager.SubMissionCounting(1, 4);

                        //미션 - 310
                        if (MissionManager.selectedSubMissionIndex == 1)
                            missionManager.SubMissionCounting(1, 2);

                        //미션 - 301
                        if (MissionManager.selectedSubMissionIndex == 1)
                        {
                            currentCnt[0] += CameraController.DiceCount;
                            if (currentCnt[0] > missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt)
                                currentCnt[0] = missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt;

                            maximumCnt = 0;

                            for (int i = 0; i < currentCnt.Length; i++)
                                if (maximumCnt < currentCnt[i])
                                    maximumCnt = currentCnt[i];
                                else
                                    continue;

                            if (currentCnt[0] == maximumCnt)
                                missionManager.SubMissionCounting(maximumCnt, 1);
                        }
                        
                        buildOnPlanet.EulerRotCal(GameObject.Find(TutorialAllMeshController.instance_.PickContainer[0].ToString()), TutorialAllMeshController.instance_.MovingObj[0], 1.03f, TutorialAllMeshController.instance_.PickContainer[1], cardSystem.pickedCard.GetComponent<CardData>().data.cardIndex);
                        finalTerrainName = TutorialAllMeshController.instance_.PickContainer[0].ToString();
                        break;
                    //건조 - 뱀
                    case 2:
                        //미션 - 210
                        if (MissionManager.selectedSubMissionIndex == 0)
                            missionManager.SubMissionCounting(1, 2);

                        //미션 - 400
                        if (MissionManager.selectedSubMissionIndex == 2 && CameraController.DiceCount > 6)
                            missionManager.SubMissionCounting(1, 0);

                        //미션 - 420
                        if (MissionManager.selectedSubMissionIndex == 2 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                            missionManager.SubMissionCounting(1, 4);

                        //미션 - 310
                        if (MissionManager.selectedSubMissionIndex == 1)
                            missionManager.SubMissionCounting(1, 2);

                        //미션 - 301
                        if (MissionManager.selectedSubMissionIndex == 1)
                        {
                            currentCnt[1] += CameraController.DiceCount;
                            if (currentCnt[1] > missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt)
                                currentCnt[1] = missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt;
                            maximumCnt = 0;

                            for (int i = 0; i < currentCnt.Length; i++)
                                if (maximumCnt < currentCnt[i])
                                    maximumCnt = currentCnt[i];
                                else
                                    continue;

                            if (currentCnt[1] == maximumCnt)
                                missionManager.SubMissionCounting(maximumCnt, 1);
                        }

                        buildOnPlanet.EulerRotCal(GameObject.Find(TutorialAllMeshController.instance_.PickContainer[0].ToString()), TutorialAllMeshController.instance_.MovingObj[6], 1.03f, TutorialAllMeshController.instance_.PickContainer[1], cardSystem.pickedCard.GetComponent<CardData>().data.cardIndex);
                        finalTerrainName = TutorialAllMeshController.instance_.PickContainer[0].ToString();
                        break;
                    //한랭 - 펭귄
                    case 3:
                        //미션 - 200
                        if (MissionManager.selectedSubMissionIndex == 0 && CameraController.DiceCount > 6)
                            missionManager.SubMissionCounting(1, 0);

                        //미션 - 420
                        if (MissionManager.selectedSubMissionIndex == 2 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                            missionManager.SubMissionCounting(1, 4);

                        //미션 - 310
                        if (MissionManager.selectedSubMissionIndex == 1)
                            missionManager.SubMissionCounting(1, 2);

                        //미션 - 301
                        if (MissionManager.selectedSubMissionIndex == 1)
                        {
                            currentCnt[2] += CameraController.DiceCount;
                            if (currentCnt[2] > missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt)
                                currentCnt[2] = missionManager.missionSet[MissionManager.selectedSubMissionIndex].subMission[1].goalCnt;

                            maximumCnt = 0;

                            for (int i = 0; i < currentCnt.Length; i++)
                                if (maximumCnt < currentCnt[i])
                                    maximumCnt = currentCnt[i];
                                else
                                    continue;

                            if (currentCnt[2] == maximumCnt)
                                missionManager.SubMissionCounting(maximumCnt, 1);
                        }

                        randomVal = Random.Range(1, 4);
                        buildOnPlanet.EulerRotCal(GameObject.Find(TutorialAllMeshController.instance_.PickContainer[0].ToString()), TutorialAllMeshController.instance_.MovingObj[randomVal], 1.03f, TutorialAllMeshController.instance_.PickContainer[1], cardSystem.pickedCard.GetComponent<CardData>().data.cardIndex);
                        finalTerrainName = TutorialAllMeshController.instance_.PickContainer[0].ToString();
                        break;
                    //바다 - 고래
                    case 4:
                        //미션 - 520
                        if (MissionManager.selectedSubMissionIndex == 3 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                            missionManager.SubMissionCounting(1, 4);

                        randomVal = Random.Range(4, 6);
                        buildOnPlanet.EulerRotCal(GameObject.Find(TutorialAllMeshController.instance_.PickContainer[0].ToString()), TutorialAllMeshController.instance_.MovingObj[randomVal], 5f, TutorialAllMeshController.instance_.PickContainer[1], cardSystem.pickedCard.GetComponent<CardData>().data.cardIndex);
                        finalTerrainName = TutorialAllMeshController.instance_.PickContainer[0].ToString();
                        break;
                    //산 - 구름
                    case 5:
                        //미션 - 401
                        if (MissionManager.selectedSubMissionIndex == 2 && CameraController.DiceCount > 6)
                            missionManager.SubMissionCounting(1, 1);

                        //미션 - 520
                        if (MissionManager.selectedSubMissionIndex == 3 && cardSystem.pickedCard.GetComponent<CardData>().data.currentCnt == 1)
                            missionManager.SubMissionCounting(1, 4);

                        randomVal = Random.Range(7, 9);
                        buildOnPlanet.EulerRotCal(GameObject.Find(TutorialAllMeshController.instance_.PickContainer[0].ToString()), TutorialAllMeshController.instance_.MovingObj[randomVal], 15f, TutorialAllMeshController.instance_.PickContainer[1], cardSystem.pickedCard.GetComponent<CardData>().data.cardIndex);
                        finalTerrainName = TutorialAllMeshController.instance_.PickContainer[0].ToString();
                        break;
                }
                Camera.main.GetComponent<TutorialPcVerCamera>().TurnChangeLogic();
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_PICKINGLOC, 5));    // <<< 여기  5라는 숫자를 바꾸면댐
                break;
            case FLOW.TO_PICKEVENTCARD:
                break;
            case FLOW.TO_PICKINGEVENTSECLECTTERRAIN:
                break;
            case FLOW.TO_PICKINGEVENTCARDLOC:
                displayTextImg.SetActive(true);
                if (turnSystem.currentTurn.Equals(TURN.MYTURN))
                    displayTextImg.GetComponent<DisplayTextImg>().Performance(displayTextImg.GetComponent<DisplayTextImg>().sprs[2]);
                else
                    displayTextImg.GetComponent<DisplayTextImg>().Performance(displayTextImg.GetComponent<DisplayTextImg>().sprs[1]);
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_PICKINGEVENTCARDLOC, 2));    // <<< 여기  2라는 숫자를 바꾸면댐
                break;

            case FLOW.ENEMYTURN_ROLLINGDICE:
                //GameObject.Find("DiceManager").GetComponent<DiceObject>().DiceSystem_Roll(getDiceNum / 10, getDiceNum % 10);
                StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_ROLLINGDICE, 5));
                break;
            case FLOW.ENEMYTURN_PICKINGCARD:
                currentFlow = FLOW.ENEMYTURN_PICKINGLOC;
                break;
            //거점을 정복했는지 여부에따라서 분기
            case FLOW.ENEMYTURN_PICKINGLOC:
                StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_PICKINGLOC, 0f));
                break;
            case FLOW.ENEMYTURN_PICKINGEVENTCARDLOC:
                displayTextImg.SetActive(true);
                displayTextImg.GetComponent<DisplayTextImg>().Performance(displayTextImg.GetComponent<DisplayTextImg>().sprs[1]);
                StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_PICKINGEVENTCARDLOC, 2.5f));
                break;
            case FLOW.ENEMYTURN_PICKEVENTCARD:
                StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_PICKEVENTCARD, 2));
                break;
            case FLOW.ENEMYTURN_PICKINGEVENTSELECTTERRAIN:
                break;
            case FLOW.RESULT:
                break;
        }
    }

    /* Index - 어떻게 승리 / 패배 했는지 조건
        1 - 행성점유율로 승리/패배
        2 - 메인 미션 클리어로 인한 승리/패배
        3 - 공통 미션 클리어로 인한 승리/패배
         */
    public void GameOver(bool isWin, int index)
    {
        resultUICanvas.SetActive(true);
        if (isWin)
            resultUICanvas.transform.GetChild(0).transform.GetChild(2).GetComponent<Text>().text = "WIN";
        else
            resultUICanvas.transform.GetChild(0).transform.GetChild(2).GetComponent<Text>().text = "LOSE";

        switch (index)
        {
            case 0:
                resultUICanvas.transform.GetChild(0).transform.GetChild(3).GetComponent<Text>().text = "행성 점유 클리어";
                break;
            case 1:
                resultUICanvas.transform.GetChild(0).transform.GetChild(3).GetComponent<Text>().text = "메인 미션 클리어";
                break;
            case 2:
                resultUICanvas.transform.GetChild(0).transform.GetChild(3).GetComponent<Text>().text = "공통 미션 클리어";
                break;
        }
        
        StartCoroutine(GameOverCor());
    }

    IEnumerator GameOverCor()
    {
        yield return new WaitForSeconds(5f);
        SceneManager.LoadScene("MainUIScene");
    }
}
