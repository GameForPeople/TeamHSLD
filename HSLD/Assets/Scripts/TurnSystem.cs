using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum TURN
{
    NOTYETSET,
    MYTURN,
    ENEMYTURN
}

public class TurnSystem : MonoBehaviour
{
    private int currentMyTurnTimer = 0;
    private int currentEnemyTurnTimer = 0;
    public Text displayTurnTimerTxt;
    private float time_;

    public Text mainTxt;
    public Text timerTxt;
    public GameObject turnSet;
    public GameObject dice;
    private FLOW beforeFlow;
    public TURN currentTurn;            //최초 선공 정할시, enum 설정.

    public float matchingCompleteTime = 5;
    public float selectOrderTime = 10;
    public float selectCardTime = 5;

    public float rollingDiceTime = 10;
    public float setTerrainOnPlanet = 50;
    public float pickingEventCardTime = 5;
    public float pickingEventCardLocTime = 15;

    private float warningTime;
    public GameObject warningPanel;

    private float warningRadio = 0;
    static public bool isSetTerrainDone = false;
    static public bool enemyEventCardDefense = false;           //이벤트 카드로 인해 상대방의 이벤트카드를 막는다.

    Coroutine myCoroutine;
    Coroutine enemyCoroutine;
    Coroutine TurnPassAndDisplayTextCor;

    //최적화 스크립트
    private FlowSystem flowSystem;
    private SetTurn setTurn;

    private void Start()
    {
        flowSystem = gameObject.GetComponent<FlowSystem>();
        setTurn = gameObject.GetComponent<SetTurn>();
        StartCoroutine(ReadyMatchingComplete());
    }

    public void TurnChange(bool change)
    {
        if (change)
            currentTurn = TURN.MYTURN;
    }

    public void DisplayTextMessage(string value, float time)
    {
        StartCoroutine(DisplayTextMessageCor(value, time));
    }

    IEnumerator DisplayTextMessageCor(string value, float time)
    {
        displayTurnTimerTxt.text = value;
        yield return new WaitForSeconds(time);
        displayTurnTimerTxt.text = "";
    }

    IEnumerator ReadySetOrderTimer()
    {
        time_ = 0;
        mainTxt.text = "카드를 선택해 주세요.";
        timerTxt.text = "";
        mainTxt.transform.localPosition = new Vector3(0, 263, 0);
        while (true)
        {
            time_ += Time.deltaTime;
            //timerTxt.text = (selectOrderTime - (int)time_).ToString() + " 초";

            yield return new WaitForEndOfFrame();
            if (SetTurn.isPicking)
                break;

            //랜덤으로 아무거나 픽킹하기
            if (time_ > selectOrderTime)
            {
                //테스트용===================================================================
                if (GameObject.Find("NetworkManager") == null)
                {
                    if (setTurn.RndNum() == 0)
                        setTurn.PickCard(turnSet.transform.GetChild(0).gameObject);
                    else
                        setTurn.PickCard(turnSet.transform.GetChild(1).gameObject);
                }
                //==========================================================================
                else
                {
                    if (GameObject.Find("NetworkManager").GetComponent<NetworkManager>().isAttackFirst)
                        setTurn.PickCard(turnSet.transform.GetChild(0).gameObject);
                    else
                        setTurn.PickCard(turnSet.transform.GetChild(1).gameObject);
                }
                break;
            }

            //if (상대방이 먼저 픽했을때)
            //    break;
        }
        yield return new WaitForSeconds(3);
        flowSystem.FlowChange(FLOW.READY_TURNORDER);
        Destroy(setTurn);
        StartCoroutine(ReadySetCardTimer());
    }

    IEnumerator ReadySetCardTimer()
    {
        time_ = 0;
        mainTxt.text = "덱을 선택해 주세요.";
        mainTxt.transform.localPosition = new Vector3(276, 269, 0);
        timerTxt.transform.localPosition = new Vector3(463.3f, -146, 0);

        while (true)
        {
            time_ += Time.deltaTime;
            timerTxt.text = (selectCardTime - (int)time_).ToString();
            yield return new WaitForEndOfFrame();
            if (time_ > selectCardTime || CardSet.isSelect)
                break;
        }

        flowSystem.missionCanvas.transform.parent = flowSystem.missionSetParentTransform;
        flowSystem.missionCanvas.transform.localPosition = new Vector3(-318, flowSystem.missionCanvas.transform.localPosition.y, 0);

        flowSystem.FlowChange(FLOW.READY_SETCARD);
    }

    IEnumerator ReadyMatchingComplete()
    {
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            timerTxt.text = (matchingCompleteTime - (int)time_).ToString() + "초후에 게임에 입장합니다.";
            yield return new WaitForEndOfFrame();
            if (time_ > matchingCompleteTime)
                break;
        }
        flowSystem.FlowChange(FLOW.READY_MATCHINGCOMPLETE);
        StartCoroutine(ReadySetOrderTimer());
    }

    //게임이 시작하고 선후공을 정한 후, 컴포넌트 액티브 활성화 - 최초시
    public void TurnSet()
    {
        gameObject.GetComponent<TerrainGainCounting>().CheckFlag();

        //내턴일때의 코루틴 진입
        if (currentTurn.Equals(TURN.MYTURN))
        {
            flowSystem.enemyTurnPassObj.SetActive(false);
            flowSystem.currentFlow = FLOW.TO_ROLLINGDICE;
            flowSystem.diceCanvas.SetActive(true);

            if(myCoroutine != null)
                StopCoroutine(myCoroutine);
            myCoroutine = StartCoroutine(MyTurnCounting());
        }
        //내턴이아닐때의 코루틴 진입
        else
        {
            flowSystem.enemyTurnPassObj.SetActive(true);
            flowSystem.currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
            flowSystem.diceCanvas.SetActive(false);
            StartCoroutine(EndTurnAndWaiting());
        }
    }

    IEnumerator EnemyTurnCounting()
    {
        currentEnemyTurnTimer += 1;
        yield return new WaitForSeconds(1);

        if (beforeFlow != flowSystem.currentFlow && !flowSystem.currentFlow.Equals(FLOW.ENEMYTURN_PICKINGLOC))
            currentEnemyTurnTimer = 0;

        
        if (flowSystem.currentFlow.Equals(FLOW.ENEMYTURN_ROLLINGDICE))
        {
            flowSystem.turnTimerImg.SetActive(true);
            displayTurnTimerTxt.text = (rollingDiceTime - currentEnemyTurnTimer).ToString();
            beforeFlow = FLOW.ENEMYTURN_ROLLINGDICE;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (flowSystem.currentFlow.Equals(FLOW.ENEMYTURN_PICKINGCARD))
        {
            flowSystem.turnTimerImg.SetActive(true);
            displayTurnTimerTxt.text = (setTerrainOnPlanet - currentEnemyTurnTimer).ToString();
            beforeFlow = FLOW.ENEMYTURN_PICKINGCARD;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (flowSystem.currentFlow.Equals(FLOW.ENEMYTURN_PICKINGLOC))
        {
            flowSystem.turnTimerImg.SetActive(true);
            displayTurnTimerTxt.text = (setTerrainOnPlanet - currentEnemyTurnTimer).ToString();
            beforeFlow = FLOW.ENEMYTURN_PICKINGLOC;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (flowSystem.currentFlow.Equals(FLOW.ENEMYTURN_PICKEVENTCARD))
        {
            flowSystem.turnTimerImg.SetActive(true);
            displayTurnTimerTxt.text = (pickingEventCardTime - currentEnemyTurnTimer).ToString();
            beforeFlow = FLOW.ENEMYTURN_PICKEVENTCARD;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }

        else if (flowSystem.currentFlow.Equals(FLOW.ENEMYTURN_PICKINGEVENTCARDLOC))
        {
            flowSystem.turnTimerImg.SetActive(true);
            displayTurnTimerTxt.text = (pickingEventCardLocTime - currentEnemyTurnTimer).ToString();
            beforeFlow = FLOW.ENEMYTURN_PICKINGEVENTCARDLOC;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }

        else if (flowSystem.currentFlow.Equals(FLOW.DISPLAYANIMATION_WAITING))
        {
            beforeFlow = FLOW.DISPLAYANIMATION_WAITING;

            displayTurnTimerTxt.text = "";
            flowSystem.turnTimerImg.SetActive(false);
            currentEnemyTurnTimer = 0;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (flowSystem.currentFlow.Equals(FLOW.TO_ROLLINGDICE))
        {
            displayTurnTimerTxt.text = "";
            currentTurn = TURN.MYTURN;
        }
        else
        {
            Debug.Log("예외처리 : " + flowSystem.currentFlow);
        }
    }

    //내턴일때 카운팅
    IEnumerator MyTurnCounting()
    {
        currentMyTurnTimer += 1;
        yield return new WaitForSeconds(1);
        //init

        if (flowSystem.currentFlow.Equals(FLOW.DISPLAYANIMATION_WAITING))
        {
            flowSystem.turnTimerImg.SetActive(false);
            displayTurnTimerTxt.text = "";
            currentMyTurnTimer = 0;
            warningRadio = 0;
            warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);

            if (myCoroutine != null)
                StopCoroutine(myCoroutine);
            myCoroutine = StartCoroutine(MyTurnCounting());
        }
        
        else
        {
            
            if (!flowSystem.currentFlow.Equals(beforeFlow) && !flowSystem.currentFlow.Equals(FLOW.TO_PICKINGLOC))
            {
                currentMyTurnTimer = 0;
                warningRadio = 0;
                warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
            }

            if (flowSystem.currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            {
                warningTime = rollingDiceTime - 3;
                if (currentMyTurnTimer >= rollingDiceTime)
                {
                    Debug.Log("예외처리 : 주사위 클릭안했을때 랜덤으로 굴리기");
                    dice.GetComponent<DiceSystem>().RollingDice(Random.Range(1, 5));
                    flowSystem.FlowChange(FLOW.TO_ROLLINGDICE);
                }

                flowSystem.turnTimerImg.SetActive(true);
                beforeFlow = FLOW.TO_ROLLINGDICE;
                displayTurnTimerTxt.text = (rollingDiceTime - currentMyTurnTimer).ToString();
            }
            else if (flowSystem.currentFlow.Equals(FLOW.TO_PICKINGCARD))
            {
                warningTime = setTerrainOnPlanet - 3;

                //예외처리
                if (currentMyTurnTimer >= setTerrainOnPlanet)
                {
                    if (GameObject.Find("GameCores") != null)
                    {
                        Debug.Log("SEND : 턴종료");
                        gameObject.GetComponent<InGameSceneManager>().SendChangeTurn();
                    }
                    
                    displayTurnTimerTxt.text = "";
                    warningRadio = 0;
                    warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
                    flowSystem.FlowChange(FLOW.TO_PICKINGLOC);
                    StopCoroutine(myCoroutine);
                }

                flowSystem.turnTimerImg.SetActive(true);
                beforeFlow = FLOW.TO_PICKINGCARD;
                displayTurnTimerTxt.text = (setTerrainOnPlanet - currentMyTurnTimer).ToString(); 
            }
            else if (flowSystem.currentFlow.Equals(FLOW.TO_PICKINGLOC))
            {
                warningTime = setTerrainOnPlanet - 3;

                //예외처리
                if (currentMyTurnTimer >= setTerrainOnPlanet)
                {
                    if (GameObject.Find("GameCores") != null)
                    {
                        Debug.Log("SEND : 턴종료");
                        gameObject.GetComponent<InGameSceneManager>().SendChangeTurn();
                    }
                    
                    displayTurnTimerTxt.text = "";
                    warningRadio = 0;
                    warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
                    flowSystem.FlowChange(FLOW.TO_PICKINGLOC);
                    StopCoroutine(myCoroutine);
                }

                flowSystem.turnTimerImg.SetActive(true);
                beforeFlow = FLOW.TO_PICKINGLOC;
                displayTurnTimerTxt.text = (setTerrainOnPlanet - currentMyTurnTimer).ToString();
            }
            else if (flowSystem.currentFlow.Equals(FLOW.TO_PICKEVENTCARD))
            {
                warningTime = pickingEventCardTime - 3;
                if (currentMyTurnTimer >= pickingEventCardTime)
                    flowSystem.FlowChange(FLOW.TO_PICKEVENTCARD);

                flowSystem.turnTimerImg.SetActive(true);
                beforeFlow = FLOW.TO_PICKEVENTCARD;
                displayTurnTimerTxt.text = (pickingEventCardTime - currentMyTurnTimer).ToString();
            }
            else if(flowSystem.currentFlow.Equals(FLOW.TO_PICKINGEVENTCARDLOC))
            {
                warningTime = pickingEventCardLocTime - 3;
                if (currentMyTurnTimer >= pickingEventCardLocTime)
                    flowSystem.FlowChange(FLOW.TO_PICKINGEVENTCARDLOC);

                flowSystem.turnTimerImg.SetActive(true);
                beforeFlow = FLOW.TO_PICKINGEVENTCARDLOC;
                displayTurnTimerTxt.text = (pickingEventCardLocTime - currentMyTurnTimer).ToString();
            }

            if (flowSystem.currentFlow.Equals(FLOW.ENEMYTURN_ROLLINGDICE))
            {
                yield return new WaitForSeconds(1.5f);
                gameObject.GetComponent<InGameSceneManager>().network_sendProtocol = (int)PROTOCOL.NOTIFY_CHANGE_TURN;

                //init
                displayTurnTimerTxt.text = "";
                warningRadio = 0;
                warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
                currentTurn = TURN.ENEMYTURN;
                StopCoroutine(myCoroutine);
            }
            else
            {
                if (myCoroutine != null)
                    StopCoroutine(myCoroutine);
                myCoroutine = StartCoroutine(MyTurnCounting());
            }

            if (warningTime < currentMyTurnTimer + 1)
            {
                warningRadio += 0.03f;
                warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
            }
        }
    }

    //내턴이 아닐때 
    IEnumerator EndTurnAndWaiting()
    {
        if (myCoroutine != null)
            StopCoroutine(myCoroutine);
        enemyCoroutine = StartCoroutine(EnemyTurnCounting());

        while (true)
        {
            yield return new WaitForEndOfFrame();
            if (currentTurn.Equals(TURN.MYTURN))
                break;
        }
        if (enemyCoroutine != null)
            StopCoroutine(enemyCoroutine);

        if(GameObject.Find("NetworkManager") == null)
            TurnSet();
    }

    public void EnemyTurnPass()
    {
        flowSystem.currentFlow = FLOW.DISPLAYANIMATION_WAITING;

        flowSystem.displayText.GetComponent<DisplayText>().text = "나의 턴";
        flowSystem.displayText.SetActive(true);

        if (TurnPassAndDisplayTextCor != null)
            StopCoroutine(TurnPassAndDisplayTextCor);
        TurnPassAndDisplayTextCor = StartCoroutine(TurnPassAndDisplayText());
    }

    IEnumerator TurnPassAndDisplayText()
    {
        yield return new WaitForSeconds(2.5f);
        currentTurn = TURN.MYTURN;
        TurnSet();
    }
}
