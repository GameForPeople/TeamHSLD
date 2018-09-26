using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DiceSystem : MonoBehaviour
{
    public Slider diceSlider;
    private int getDiceNum;
    private bool isTriggerEnter = false;
    private float gaze;
    private bool raiseValue = true;
    public float constValue;
    private int rollingCnt = 0;

    public void OnTrigger()
    {
        if (!GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            return;

        //init
        diceSlider.value = 0;
        gaze = 0;

        isTriggerEnter = true;
    }
    public void OffTrigger()
    {
        if (!GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            return;

        isTriggerEnter = false;
        rollingCnt += 1;

        if ((int)gaze < 3)
            RollingDice(1);
        else if ((int)gaze > 2 && (int)gaze < 5)
            RollingDice(2);
        else if ((int)gaze > 4)
            RollingDice(3);
    }

    //랜덤 넘버 추출
    int OutRndNum(int min, int max)
    {
        getDiceNum += Random.Range(min, max);
        return getDiceNum;
    }

    //슬라이드 값을 바탕으로 다이스 눈 설정.
    void RollingDice(int index)
    {
        switch (index)
        {
            case 1:
                OutRndNum(1, 4);
                break;
            case 2:
                OutRndNum(4, 6);
                break;
            case 3:
                getDiceNum += 6;
                break;
            default:
                Debug.Log("error : 1");
                break;
        }

        if (rollingCnt == 2)
        {
            rollingCnt = 0;
            //flow 변경
            GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().FlowChange(FLOW.TO_ROLLINGDICE);
            Debug.Log("합 다이스 눈 : " + getDiceNum);
            getDiceNum = 0;
        }

    }

    //슬라이드 값 설정
    void SetSliderValue()
    {
        if (diceSlider.value == diceSlider.maxValue)
            raiseValue = false;

        if (diceSlider.value == diceSlider.minValue)
            raiseValue = true;

        if (raiseValue)
        {
            gaze += (Time.deltaTime * constValue);
        }
        else
        {
            gaze -= (Time.deltaTime * constValue);
        }

        diceSlider.value = gaze;
    }

    private void Update()
    {
        if (isTriggerEnter && GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().currentFlow.Equals(FLOW.TO_ROLLINGDICE))
        {
            if (Input.GetMouseButton(0))
            {
                SetSliderValue();
            }
        }
    }
}
