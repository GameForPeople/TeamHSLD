using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public enum TURN
{
    MYTURN,
    ENEMYTURN
}

public class TurnSystem : MonoBehaviour
{
    private int currentMyTurnTimer = 0;
    public Text displayTurnTimerTxt;
    public Slider buildTimerSlider;
    public Text buildTimerText;
    private float time_;

    private FLOW beforeFlow;

    public TURN currentTurn;            //최초 선공 정할시, enum 설정.

    //게임이 시작하고 선후공을 정한 후, 컴포넌트 액티브 활성화 - 최초시
    private void OnEnable()
    {
        //내턴일때의 코루틴 진입
        if (currentTurn.Equals(TURN.MYTURN))
        {
            StartCoroutine(TurnCounting());
        }
        //내턴이아닐때의 코루틴 진입
        else
        {
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
            currentMyTurnTimer = 0;

        if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
        {
            if (currentMyTurnTimer > 10)
            {
                Debug.Log("error:2");
                displayTurnTimerTxt.text = "시간만료. 어떻게처리할까.";
            }
            else
            {
                beforeFlow = FLOW.TO_ROLLINGDICE;
                displayTurnTimerTxt.text = "(임시)다이스를 굴려주세요.( ";
                displayTurnTimerTxt.text += currentMyTurnTimer.ToString();
                displayTurnTimerTxt.text += "/ 10 )";
            }
        }
        else if(gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGCARD))
        {
            if (currentMyTurnTimer > 5)
            {
                Debug.Log("error:3");
                displayTurnTimerTxt.text = "시간만료. 어떻게처리할까.";
            }
            else
            {
                beforeFlow = FLOW.TO_PICKINGCARD;
                displayTurnTimerTxt.text = "(임시)지형 카드를 선택해주세요. ( ";
                displayTurnTimerTxt.text += currentMyTurnTimer.ToString();
                displayTurnTimerTxt.text += "/ 5 )";
            }

        }
        else if(gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKINGLOC))
        {
            if (currentMyTurnTimer > 20)
            {
                Debug.Log("error:4");
                displayTurnTimerTxt.text = "시간만료. 어떻게처리할까.";
            }
            else
            {
                beforeFlow = FLOW.TO_PICKINGLOC;
                displayTurnTimerTxt.text = "(임시)지형을 설치해주세요. ( ";
                displayTurnTimerTxt.text += currentMyTurnTimer.ToString();
                displayTurnTimerTxt.text += "/ 20 )";
            }

        }
        else if (gameObject.GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_PICKEVENTCARD))
        {
            if (currentMyTurnTimer > 15)
            {
                Debug.Log("error:5");
                displayTurnTimerTxt.text = "시간만료. 상대턴진행";

                StartCoroutine(EndTurnAndWaiting());
            }
            else
            {
                beforeFlow = FLOW.TO_PICKEVENTCARD;
                displayTurnTimerTxt.text = "(임시)이벤트 카드를 선택해주세요. ( ";
                displayTurnTimerTxt.text += currentMyTurnTimer.ToString();
                displayTurnTimerTxt.text += "/ 15 )";
            }
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

    //행성지형설치에대한 타이머 가동
    public IEnumerator BuildTimer()
    {
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            buildTimerText.text = ((int)20 - time_).ToString();
            buildTimerSlider.maxValue = 20;
            buildTimerSlider.minValue = 0;
            buildTimerSlider.value = 20 - time_;

            yield return new WaitForEndOfFrame();
            if (buildTimerSlider.value == 0)
                break;
        }
        buildTimerText.text = "";
    }

}
