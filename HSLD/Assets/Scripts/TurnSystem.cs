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
    public int turnTimer;
    public Text displayTurnTimerTxt;
    public Slider buildTimerSlider;
    public Text buildTimerText;
    private float time_;


    public TURN currentTurn;            //최초 선공 정할시, enum 설정.
    static public bool myTurn;          //최초 이후 턴바뀔때 bool 설정.

    //게임이 시작하고 선후공을 정한 후, 컴포넌트 액티브 활성화 - 최초시
    private void OnEnable()
    {
        //내턴일때의 코루틴 진입
        if (currentTurn.Equals(TURN.MYTURN))
        {
            myTurn = true;
            StartCoroutine(TurnCounting());
        }
        //내턴이아닐때의 코루틴 진입
        else
        {
            myTurn = false;
            StartCoroutine(EndTurnAndWaiting());
        }
    }

    //내턴일때 카운팅
    IEnumerator TurnCounting()
    {
        yield return new WaitForSeconds(1);
        currentMyTurnTimer += 1;
        displayTurnTimerTxt.text = currentMyTurnTimer.ToString();

        if (currentMyTurnTimer < turnTimer)
            StartCoroutine(TurnCounting());
        else
            StartCoroutine(EndTurnAndWaiting());
    }

    //내턴이 아닐때 
    IEnumerator EndTurnAndWaiting()
    {
        myTurn = false;
        displayTurnTimerTxt.text = "";
        while (true)
        {
            yield return new WaitForEndOfFrame();
            if (myTurn)
                break;
        }

        currentMyTurnTimer = 0;
        StartCoroutine(TurnCounting());
    }

    //행성지형설치에대한 타이머 가동
    public IEnumerator BuildTimer()
    {
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            buildTimerText.text = ((int)10 - time_).ToString();
            buildTimerSlider.maxValue = 10;
            buildTimerSlider.minValue = 0;
            buildTimerSlider.value = 10 - time_;

            yield return new WaitForEndOfFrame();
            if (buildTimerSlider.value == 0)
                break;
        }
        buildTimerText.text = "";
    }

}
