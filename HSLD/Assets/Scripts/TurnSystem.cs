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
    public float selectOrder = 10;
    public float selectCard = 50;
    public GameObject turnSet;

    public Slider readySlider;
    private FLOW beforeFlow;
    public TURN currentTurn;            //최초 선공 정할시, enum 설정.

    public float rollingDiceTime = 10;
    public float pickingTerrainCardTime = 5;
    public float pickingEventCardTime = 15;
    public float selectPlanetTerrainTime = 20;

    private float warningTime;
    public GameObject warningPanel;
    private float warningRadio = 0;


    private void Start()
    {
        StartCoroutine(ReadySetOrderTimer());
    }

   IEnumerator ReadySetOrderTimer()
    {
        time_ = 0;
        readySlider.maxValue = selectOrder;
        readySlider.value = selectOrder;

        while (true)
        {
            time_ += Time.deltaTime;
            readySlider.value -= Time.deltaTime;

            yield return new WaitForEndOfFrame();
            if (SetTurn.isPicking)
                break;

            //랜덤으로 아무거나 픽킹하기
            if (time_ > selectOrder)
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
        StartCoroutine(ReadySetCardTimer());
    }
    IEnumerator ReadySetCardTimer()
    {
        time_ = 0;
        readySlider.maxValue = selectCard;
        readySlider.value = selectCard;
        while (true)
        {
            time_ += Time.deltaTime;
            readySlider.value -= Time.deltaTime;
            yield return new WaitForEndOfFrame();
            if (time_ > selectCard /*상대방과 내가 모두 선택했을때*/)
                break;
        }
        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.READY_SETCARD);
    }


    //게임이 시작하고 선후공을 정한 후, 컴포넌트 액티브 활성화 - 최초시
    public void TurnSet()
    {
        //내턴일때의 코루틴 진입
        if (currentTurn.Equals(TURN.MYTURN))
        {
            gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.TO_ROLLINGDICE;
            StartCoroutine(TurnCounting());
        }
        //내턴이아닐때의 코루틴 진입
        else
        {
            gameObject.GetComponent<FlowSystem>().currentFlow = FLOW.READY_DONE;
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
                Debug.Log("시간경과 어떻게 처리할것인지.");

                StartCoroutine(EndTurnAndWaiting());
            }
            else
            {
                beforeFlow = FLOW.TO_PICKEVENTCARD;
            }

            displayTurnTimerTxt.text = "(임시)이벤트 카드를 선택해주세요. ( ";
            displayTurnTimerTxt.text += currentMyTurnTimer.ToString();
            displayTurnTimerTxt.text += " / " + pickingEventCardTime + " )";
        }

        if (warningTime < currentMyTurnTimer + 1)
        {
            warningRadio += 0.05f;
            warningPanel.GetComponent<Image>().color = new Color(1, 0, 0, warningRadio);
        }

        StartCoroutine(TurnCounting());
    }

    //내턴이 아닐때 
    IEnumerator EndTurnAndWaiting()
    {
        while (true)
        {
            yield return new WaitForEndOfFrame();
            if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
                break;
        }
        StartCoroutine(TurnCounting());
    }
}
