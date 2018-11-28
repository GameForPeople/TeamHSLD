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

    private float warningTime;
    public GameObject warningPanel;

    private float warningRadio = 0;
    static public bool isSetTerrainDone = false;

    Coroutine myCoroutine;
    Coroutine enemyCoroutine;

    public void TurnChange(bool change)
    {
        if (change)
        {
            currentTurn = TURN.MYTURN;
        }
    }

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
        timerTxt.text = "";
        mainTxt.transform.localPosition = new Vector3(0, 200, 0);
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
        yield return new WaitForSeconds(3);
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_TURNORDER);
        Destroy(gameObject.GetComponent<SetTurn>());
        StartCoroutine(ReadySetCardTimer());
    }

    IEnumerator ReadySetCardTimer()
    {
        time_ = 0;
        mainTxt.text = "덱을 선택해 주세요.";
        mainTxt.transform.localPosition = new Vector3(211, 269, 0);
        timerTxt.transform.localPosition = new Vector3(463.3f, -190, 0);

        while (true)
        {
            time_ += Time.deltaTime;
            timerTxt.text = (selectCardTime - (int)time_).ToString() + " 초";
            yield return new WaitForEndOfFrame();
            if (time_ > selectCardTime || CardSet.isSelect)
                break;
        }

        gameObject.GetComponent<FlowSystem>().missionCanvas.transform.parent = gameObject.GetComponent<FlowSystem>().missionSetParentTransform;
        gameObject.GetComponent<FlowSystem>().missionCanvas.transform.localPosition = new Vector3(-318, gameObject.GetComponent<FlowSystem>().missionCanvas.transform.localPosition.y, 0);

        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_SETCARD);
    }

    IEnumerator ReadyMatchingComplete()
    {
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            timerTxt.text = (matchingCompleteTime - (int)time_).ToString() + "초 후 시작";
            yield return new WaitForEndOfFrame();
            if (time_ > matchingCompleteTime)
                break;
        }
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_MATCHINGCOMPLETE);
        StartCoroutine(ReadySetOrderTimer());
    }

    //게임이 시작하고 선후공을 정한 후, 컴포넌트 액티브 활성화 - 최초시
    public void TurnSet()
    {
        if (SoundManager.instance_ != null)
            SoundManager.instance_.BGMMixing(SoundManager.instance_.clips[0], 0.5f);

        //내턴일때의 코루틴 진입
        if (currentTurn.Equals(TURN.MYTURN))
        {
            gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_ROLLINGDICE;
            displayTurnTimerTxt.transform.localPosition = new Vector3(503, 246.1f, 0);
            gameObject.GetComponent<FlowSystem>().turnTimerImg.transform.localPosition = new Vector3(503.2f, 294.3f, 0);
            gameObject.GetComponent<FlowSystem>().enemyImage.SetActive(false);
            gameObject.GetComponent<FlowSystem>().diceCanvas.SetActive(true);

            if(myCoroutine != null)
                StopCoroutine(myCoroutine);
            myCoroutine = StartCoroutine(MyTurnCounting());
        }
        //내턴이아닐때의 코루틴 진입
        else
        {
            gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
            displayTurnTimerTxt.transform.localPosition = new Vector3(550, -308, 0);
            gameObject.GetComponent<FlowSystem>().turnTimerImg.transform.localPosition = new Vector3(549.3f, -260.3f, 0);
            gameObject.GetComponent<FlowSystem>().enemyImage.SetActive(true);
            gameObject.GetComponent<FlowSystem>().diceCanvas.SetActive(false);
            StartCoroutine(EndTurnAndWaiting());
        }
            
    }

    IEnumerator EnemyTurnCounting()
    {
        currentEnemyTurnTimer += 1;
        yield return new WaitForSeconds(1);

        if (beforeFlow != gameObject.GetComponent<FlowSystem>().currentFlow && !gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_PICKINGLOC))
            currentEnemyTurnTimer = 0;

        
        if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_ROLLINGDICE))
        {
            gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(true);
            displayTurnTimerTxt.text = (rollingDiceTime - currentEnemyTurnTimer).ToString() + " 초";
            beforeFlow = FLOW.ENEMYTURN_ROLLINGDICE;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_PICKINGCARD))
        {
            gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(true);
            displayTurnTimerTxt.text = (setTerrainOnPlanet - currentEnemyTurnTimer).ToString() + " 초";
            beforeFlow = FLOW.ENEMYTURN_PICKINGCARD;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_PICKINGLOC))
        {
            gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(true);
            displayTurnTimerTxt.text = (setTerrainOnPlanet - currentEnemyTurnTimer).ToString() + " 초";
            beforeFlow = FLOW.ENEMYTURN_PICKINGLOC;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.ENEMYTURN_PICKEVENTCARD))
        {
            gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(true);
            displayTurnTimerTxt.text = (pickingEventCardTime - currentEnemyTurnTimer).ToString() + " 초";
            beforeFlow = FLOW.ENEMYTURN_PICKEVENTCARD;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.DISPLAYANIMATION_WAITING))
        {
            beforeFlow = FLOW.DISPLAYANIMATION_WAITING;

            displayTurnTimerTxt.text = "";
            gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(false);
            currentEnemyTurnTimer = 0;
            enemyCoroutine = StartCoroutine(EnemyTurnCounting());
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
        {
            displayTurnTimerTxt.text = "";
            currentTurn = TURN.MYTURN;
        }
        else
        {
            Debug.Log("예외처리 : " + gameObject.GetComponent<FlowSystem>().currentFlow);
        }
    }

    //내턴일때 카운팅
    IEnumerator MyTurnCounting()
    {
        currentMyTurnTimer += 1;
        yield return new WaitForSeconds(1);
        //init

        if (gameObject.GetComponent <FlowSystem>().currentFlow.Equals(FLOW.DISPLAYANIMATION_WAITING))
        {
            gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(false);
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
            
            if (!gameObject.GetComponent<FlowSystem>().currentFlow.Equals(beforeFlow) && !gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGLOC))
            {
                currentMyTurnTimer = 0;
                warningRadio = 0;
                warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
            }

            if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            {
                warningTime = rollingDiceTime - 3;
                if (currentMyTurnTimer >= rollingDiceTime)
                {
                    Debug.Log("예외처리 : 주사위 클릭안했을때 랜덤으로 굴리기");
                    dice.GetComponent<DiceSystem>().RollingDice(Random.Range(1, 5));
                    gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_ROLLINGDICE);
                }

                gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(true);
                beforeFlow = FLOW.TO_ROLLINGDICE;
                displayTurnTimerTxt.text = (rollingDiceTime - currentMyTurnTimer).ToString() + " 초";
            }
            else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGCARD))
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
                    gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGLOC);
                    StopCoroutine(myCoroutine);
                }

                gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(true);
                beforeFlow = FLOW.TO_PICKINGCARD;
                displayTurnTimerTxt.text = (setTerrainOnPlanet - currentMyTurnTimer).ToString() + " 초"; 
            }
            else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGLOC))
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
                    gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGLOC);
                    StopCoroutine(myCoroutine);
                }

                gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(true);
                beforeFlow = FLOW.TO_PICKINGLOC;
                displayTurnTimerTxt.text = (setTerrainOnPlanet - currentMyTurnTimer).ToString() + " 초";
            }
            else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKEVENTCARD))
            {
                warningTime = pickingEventCardTime - 3;
                if (currentMyTurnTimer >= pickingEventCardTime)
                    gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKEVENTCARD);

                gameObject.GetComponent<FlowSystem>().turnTimerImg.SetActive(true);
                beforeFlow = FLOW.TO_PICKEVENTCARD;
                displayTurnTimerTxt.text = (pickingEventCardTime - currentMyTurnTimer).ToString() + " 초";
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
}
