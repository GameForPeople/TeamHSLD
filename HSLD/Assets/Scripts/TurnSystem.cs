using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum TURN
{
    NOTYETSET,
    MYTURN_NOTYETFLAG,
    MYTURN_GETFLAG,
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

    private FLOW beforeFlow;
    public TURN currentTurn;            //최초 선공 정할시, enum 설정.

    public float matchingCompleteTime = 10;
    public float displayMissionTime = 15;
    public float selectCardTime = 5;
    public float selectOrderTime = 5;


    public float rollingDiceTime = 10;
    public float pickingTerrainCardTime = 10;
    public float pickingEventCardTime = 5;
    public float selectPlanetTerrainTime = 20;

    private float warningTime;
    public GameObject warningPanel;

    private float warningRadio = 0;
    static public bool isSetTerrainDone = false;

    Coroutine myCoroutine;
    Coroutine enemyCoroutine;

    private void Start()
    {
        if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TSETVER))
            return;

        StartCoroutine(ReadyMatchingComplete());
    }

    public void DisplayTextMessage(string value, float time)
    {
        StartCoroutine(DisplayTextMessageCor(value, time));
    }

    IEnumerator DisplayTextMessageCor(string value, float time)
    {
        displayTurnTimerTxt.text = value;
        GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().tmpAnimationImage.SetActive(true);
        yield return new WaitForSeconds(time);
        displayTurnTimerTxt.text = "";
        GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().tmpAnimationImage.SetActive(false);
    }

    IEnumerator ReadySetOrderTimer()
    {
        time_ = 0;
        mainTxt.text = "카드를 선택해 주세요.";
        while (true)
        {
            time_ += Time.deltaTime;
            timerTxt.text = (selectOrderTime - (int)time_).ToString() + " 초";

            yield return new WaitForEndOfFrame();
            if (SetTurn.isPicking)
                break;

            //랜덤으로 아무거나 픽킹하기
            if (time_ > selectOrderTime)
            {
                //테스트용===================================================================
                if (GameObject.Find("NetworkManager") == null)
                {
                    if (gameObject.GetComponent<SetTurn>().RndNum() == 0)
                        gameObject.GetComponent<SetTurn>().PickCard(turnSet.transform.GetChild(0).gameObject);
                    else
                        gameObject.GetComponent<SetTurn>().PickCard(turnSet.transform.GetChild(1).gameObject);
                }
                //==========================================================================
                else
                {
                    if (GameObject.Find("NetworkManager").GetComponent<NetworkManager>().isAttackFirst)
                        gameObject.GetComponent<SetTurn>().PickCard(turnSet.transform.GetChild(0).gameObject);
                    else
                        gameObject.GetComponent<SetTurn>().PickCard(turnSet.transform.GetChild(1).gameObject);
                }

                break;
            }

            //if (상대방이 먼저 픽했을때)
            //    break;
        }

        yield return new WaitForSeconds(2);
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_TURNORDER);
        Destroy(gameObject.GetComponent<SetTurn>());
    }

    IEnumerator ReadySetCardTimer()
    {
        time_ = 0;
        mainTxt.text = "덱을 선택해 주세요.";
        while (true)
        {
            time_ += Time.deltaTime;
            timerTxt.text = (selectCardTime - (int)time_).ToString() + " 초";
            yield return new WaitForEndOfFrame();
            if (time_ > selectCardTime || CardSet.isSelect)
                break;
        }
        StartCoroutine(ReadySetOrderTimer());
    }

    IEnumerator ReadyMatchingComplete()
    {
        time_ = 0;
        mainTxt.text = "일반게임";
        while (true)
        {
            time_ += Time.deltaTime;
            timerTxt.text = (matchingCompleteTime - (int)time_).ToString() + "초 후 시작";
            yield return new WaitForEndOfFrame();
            if (time_ > matchingCompleteTime)
                break;
        }
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_MATCHINGCOMPLETE);
        StartCoroutine(ReadyDisplayMission());
    }

    IEnumerator ReadyDisplayMission()
    {
        time_ = 0;
        mainTxt.text = "미션을 확인하세요.";
        while (true)
        {
            time_ += Time.deltaTime;
            timerTxt.text = (displayMissionTime - (int)time_).ToString() + "초 후 시작";
            yield return new WaitForEndOfFrame();
            if (time_ > displayMissionTime)
                break;
        }
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_DISPLAYMISSION);
        StartCoroutine(ReadySetCardTimer());
    }

    //게임이 시작하고 선후공을 정한 후, 컴포넌트 액티브 활성화 - 최초시
    public void TurnSet()
    {
        if (SoundManager.instance_ != null)
            SoundManager.instance_.BGMMixing(SoundManager.instance_.clips[0], 0.5f);

        Debug.Log(currentTurn);

        //내턴일때의 코루틴 진입
        if (currentTurn.Equals(TURN.MYTURN_NOTYETFLAG) || currentTurn.Equals(TURN.MYTURN_GETFLAG))
        {
            gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_ROLLINGDICE;
            myCoroutine = StartCoroutine(MyTurnCounting());
        }
        //내턴이아닐때의 코루틴 진입
        else
            StartCoroutine(EndTurnAndWaiting());
    }

    IEnumerator EnemyTurnCounting()
    {
        currentEnemyTurnTimer += 1;
        yield return new WaitForSeconds(1);

        if (beforeFlow != gameObject.GetComponent<FlowSystem>().currentFlow)
            currentEnemyTurnTimer = 0;

        if (FlowSystem.isWaitingTime)
        {
            displayTurnTimerTxt.text = "";
            currentEnemyTurnTimer = 0;
            enemyCoroutine =  StartCoroutine(EnemyTurnCounting());
        }

        displayTurnTimerTxt.text = "( ";
        displayTurnTimerTxt.text += currentEnemyTurnTimer.ToString();


        if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_ROLLINGDICE))
        {
            displayTurnTimerTxt.text += " / " + rollingDiceTime + " )";
            beforeFlow = FLOW.ENEMYTURN_ROLLINGDICE;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_PICKINGCARD))
        {
            displayTurnTimerTxt.text += " / " + pickingTerrainCardTime + " )";
            beforeFlow = FLOW.ENEMYTURN_PICKINGCARD;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_PICKINGLOC))
        {
            displayTurnTimerTxt.text += " / " + selectPlanetTerrainTime + " )";
            beforeFlow = FLOW.ENEMYTURN_PICKINGLOC;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_PICKEVENTCARD))
        {
            displayTurnTimerTxt.text += " / " + pickingEventCardTime + " )";
            beforeFlow = FLOW.ENEMYTURN_PICKEVENTCARD;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
        {
            displayTurnTimerTxt.text = "";
            currentTurn = TURN.MYTURN_GETFLAG;
        }
    }

    //내턴일때 카운팅
    IEnumerator MyTurnCounting()
    {
        currentMyTurnTimer += 1;
        yield return new WaitForSeconds(1);
        //init

        if (FlowSystem.isWaitingTime)
        {
            displayTurnTimerTxt.text = "";
            currentMyTurnTimer = 0;
            warningRadio = 0;
            warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
            myCoroutine = StartCoroutine(MyTurnCounting());
        }
        
        else
        {
            if (!gameObject.GetComponent<FlowSystem>().currentFlow.Equals(beforeFlow))
            {
                currentMyTurnTimer = 0;
                warningRadio = 0;
                warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
            }

            displayTurnTimerTxt.text = "( ";
            displayTurnTimerTxt.text += currentMyTurnTimer.ToString();

            if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            {
                warningTime = rollingDiceTime - 3;
                if (currentMyTurnTimer >= rollingDiceTime)
                    gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_ROLLINGDICE);

                beforeFlow = FLOW.TO_ROLLINGDICE;
                displayTurnTimerTxt.text += " / " + rollingDiceTime + " )";
            }
            else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGCARD))
            {
                warningTime = pickingTerrainCardTime - 3;
                if (currentMyTurnTimer >= pickingTerrainCardTime)
                    gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGCARD);

                beforeFlow = FLOW.TO_PICKINGCARD;
                displayTurnTimerTxt.text += " / " + pickingTerrainCardTime + " )";
            }
            else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGLOC))
            {
                warningTime = selectPlanetTerrainTime - 3;
                if (currentMyTurnTimer >= selectPlanetTerrainTime)
                {
                    isSetTerrainDone = true;
                    gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGLOC);
                }

                beforeFlow = FLOW.TO_PICKINGLOC;
                displayTurnTimerTxt.text += " / " + selectPlanetTerrainTime + " )";
            }
            else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKEVENTCARD))
            {
                warningTime = pickingEventCardTime - 3;
                if (currentMyTurnTimer >= pickingEventCardTime)
                    gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKEVENTCARD);

                beforeFlow = FLOW.TO_PICKEVENTCARD;
                displayTurnTimerTxt.text += " / " + pickingEventCardTime + " )";
            }

            if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_ROLLINGDICE))
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
                myCoroutine = StartCoroutine(MyTurnCounting());

            if (warningTime < currentMyTurnTimer + 1)
            {
                warningRadio += 0.05f;
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
            if (currentTurn.Equals(TURN.MYTURN_NOTYETFLAG) || currentTurn.Equals(TURN.MYTURN_GETFLAG))
                break;
        }

        StopCoroutine(enemyCoroutine);
        gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_ROLLINGDICE;
        myCoroutine = StartCoroutine(MyTurnCounting());
    }
}
