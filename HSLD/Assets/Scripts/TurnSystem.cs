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
    public float pickingEventCardTime = 15;
    public float selectPlanetTerrainTime = 20;

    private float warningTime;
    public GameObject warningPanel;
    private float warningRadio = 0;

    Coroutine coroutine;

    private void Start()
    {

        if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TSETVER))
            return;

        StartCoroutine(ReadyMatchingComplete());
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
            //if (SetTurn.isPicking)
            //    break;

            //랜덤으로 아무거나 픽킹하기
            if (time_ > selectOrderTime)
            {
                //테스트용===================================================================
                if(GameObject.Find("NetworkManager") == null)
                {
                    if(gameObject.GetComponent<SetTurn>().RndNum() == 0)
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
            if (time_ > selectCardTime /*|| CardSet.isSelect*/)
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

        //내턴일때의 코루틴 진입
        if (currentTurn.Equals(TURN.MYTURN))
        {
            gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_ROLLINGDICE;
            coroutine = StartCoroutine(TurnCounting());
        }
        //내턴이아닐때의 코루틴 진입
        else
        {
            gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.WAITING;
            StartCoroutine(EndTurnAndWaiting());
        }
    }

    //내턴일때 카운팅
    IEnumerator TurnCounting()
    {
        currentMyTurnTimer += 1;
        yield return new WaitForSeconds(1);
        //init
        if (!gameObject.GetComponent<FlowSystem>().currentFlow.Equals(beforeFlow))
        {
            displayTurnTimerTxt.text = "";
            currentMyTurnTimer = 0;
            warningRadio = 0;
            warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
            
        }


        if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
        {
            warningTime = rollingDiceTime - 3;
            if (currentMyTurnTimer >= rollingDiceTime)
            {
                gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_ROLLINGDICE);
                Debug.Log("시간경과 어떻게 처리할것인지.");
            }
            else
            {
                beforeFlow = FLOW.TO_ROLLINGDICE;
            }

            displayTurnTimerTxt.text = "(임시)다이스를 굴려주세요.( ";
            displayTurnTimerTxt.text += currentMyTurnTimer.ToString();
            displayTurnTimerTxt.text += " / " + rollingDiceTime + " )";
        }
        else if(gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGCARD))
        {
            warningTime = pickingTerrainCardTime - 3;
            if (currentMyTurnTimer >= pickingTerrainCardTime)
            {
                gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGCARD);
                Debug.Log("시간경과 어떻게 처리할것인지.");
            }
            else
            {
                beforeFlow = FLOW.TO_PICKINGCARD;
            }

            displayTurnTimerTxt.text = "(임시)지형 카드를 선택해주세요. ( ";
            displayTurnTimerTxt.text += currentMyTurnTimer.ToString();
            displayTurnTimerTxt.text += " / " + pickingTerrainCardTime + " )";
        }
        else if(gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGLOC))
        {
            warningTime = selectPlanetTerrainTime - 3;
            if (currentMyTurnTimer >= selectPlanetTerrainTime)
            {
                gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGLOC);
                Debug.Log("시간경과 어떻게 처리할것인지.");
            }
            else
            {
                beforeFlow = FLOW.TO_PICKINGLOC;
            }

            displayTurnTimerTxt.text = "(임시)지형을 설치해주세요. ( ";
            displayTurnTimerTxt.text += currentMyTurnTimer.ToString();
            displayTurnTimerTxt.text += " / " + selectPlanetTerrainTime + " )";
        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKEVENTCARD))
        {
            warningTime = pickingEventCardTime - 3;
            if (currentMyTurnTimer >= pickingEventCardTime)
            {
                gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKEVENTCARD);
                Debug.Log("턴종료");
            }
            else
            {
                beforeFlow = FLOW.TO_PICKEVENTCARD;
            }

            displayTurnTimerTxt.text = "(임시)이벤트 카드를 선택해주세요. ( ";
            displayTurnTimerTxt.text += currentMyTurnTimer.ToString();
            displayTurnTimerTxt.text += " / " + pickingEventCardTime + " )";
        }

        if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.WAITING))
        {

            yield return new WaitForSeconds(1.5f);
            gameObject.GetComponent<InGameSceneManager>().network_sendProtocol = (int)PROTOCOL.NOTIFY_END_OF_TURN;
            //init
            displayTurnTimerTxt.text = "";
            currentMyTurnTimer = 0;
            warningRadio = 0;
            warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);

            currentTurn = TURN.ENEMYTURN;
            StopCoroutine(coroutine);
            TurnSet();
        }
        else
            coroutine = StartCoroutine(TurnCounting());

        if (warningTime < currentMyTurnTimer + 1)
        {
            warningRadio += 0.05f;
            warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
        }
    }

    //내턴이 아닐때 
    IEnumerator EndTurnAndWaiting()
    {
        while (true)
        {
            yield return new WaitForEndOfFrame();
            if (currentTurn.Equals(TURN.MYTURN))
                break;
        }

        gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_ROLLINGDICE;
        coroutine = StartCoroutine(TurnCounting());
    }
}
