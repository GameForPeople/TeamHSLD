using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DiceSystem : MonoBehaviour
{
    public Slider diceSlider;
    public static int getDiceNum;
    static public bool isDouble;
    private int rndTmpValue;
    private bool isTriggerEnter = false;
    private float gaze;
    private bool raiseValue = true;
    public float constValue;
    private int characterDiceRateUp = 5;        //tmp
    

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

        //0123
        if ((int)gaze < 4)
            RollingDice(1);

        //456
        else if ((int)gaze > 3 && (int)gaze < 7)
            RollingDice(2);

        //78
        else if ((int)gaze > 6 && (int)gaze < 9)
            RollingDice(3);

        //9
        else if ((int)gaze > 8)
            RollingDice(4);
    }

    //랜덤 넘버 추출
    int OutRndNum(int min, int max)
    {
        return Random.Range(min, max);
    }

    //슬라이드 값을 바탕으로 다이스 눈 설정.
    void RollingDice(int index)
    {
        switch(index)
        {
            case 1:
                if (OutRndNum(0, 100) < 40 + characterDiceRateUp)
                {
                    //2일확률 1/3
                    if (OutRndNum(0, 3) < 1)
                    {
                        //무조건더블 11
                        isDouble = true;
                        getDiceNum = 2;
                    }
                    //3일확률 2/3 
                    else
                    {
                        isDouble = false;
                        getDiceNum = 3;
                    }
                }

                else
                {
                    rndTmpValue = OutRndNum(0, 3);
                    //4-6
                    if (rndTmpValue == 0)
                    {
                        rndTmpValue = OutRndNum(0, 12);
                        //4일확률 3/12
                        if (rndTmpValue < 3)
                        {
                            isDouble = false;
                            // 2/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue < 2)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 4;

                        }
                        //5일확률 4/12
                        else if (rndTmpValue > 2 && rndTmpValue < 7)
                        {
                            isDouble = false;
                            getDiceNum = 5;
                        }
                        //6일확률 5/12
                        else
                        {
                            isDouble = false;
                            // 1/5확률로 더블
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 6;
                        }
                    }
                    //7-9
                    else if (rndTmpValue == 1)
                    {
                        rndTmpValue = OutRndNum(0, 15);
                        //7일확률 6/15
                        if (rndTmpValue < 6)
                        {
                            isDouble = false;
                            getDiceNum = 7;
                        }
                        //8일확률 5/15
                        else if (rndTmpValue > 5 && rndTmpValue < 11)
                        {
                            isDouble = false;
                            //1/5확률로 더블
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 8;
                        }
                        //9일확률 4/15
                        else
                        {
                            isDouble = false;
                            getDiceNum = 9;
                        }
                    }
                    //10-12
                    else if (rndTmpValue == 2)
                    {
                        rndTmpValue = OutRndNum(0, 6);
                        //10일확률 3/6
                        if (rndTmpValue < 3)
                        {
                            isDouble = false;
                            //1/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 10;
                        }
                        //11일확률 2/6
                        else if (rndTmpValue > 2 && rndTmpValue < 5)
                        {
                            isDouble = false;
                            getDiceNum = 11;
                        }
                        //12일확률 1/6
                        else
                        {
                            //무조건 더블
                            isDouble = true;
                            getDiceNum = 12;
                        }
                    }
                }
                break;
            case 2:
                if (OutRndNum(0, 100) < 40 + characterDiceRateUp)
                {
                    rndTmpValue = OutRndNum(0, 3);
                    //4-6
                    if (rndTmpValue == 0)
                    {
                        rndTmpValue = OutRndNum(0, 12);
                        //4일확률 3/12
                        if (rndTmpValue < 3)
                        {
                            isDouble = false;
                            // 2/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue < 2)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 4;

                        }
                        //5일확률 4/12
                        else if (rndTmpValue > 2 && rndTmpValue < 7)
                        {
                            isDouble = false;
                            getDiceNum = 5;
                        }
                        //6일확률 5/12
                        else
                        {
                            // 1/5확률로 더블
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 6;
                        }
                    }
                }

                else
                {
                    rndTmpValue = OutRndNum(0, 3);
                    if (rndTmpValue == 0)
                    {
                        //2일확률 1/3
                        if (OutRndNum(0, 3) < 1)
                        {
                            //무조건더블 11
                            isDouble = true;
                            getDiceNum = 2;
                        }
                        //3일확률 2/3 
                        else
                        {
                            isDouble = false;
                            getDiceNum = 3;
                        }
                    }
                    //7-9
                    else if (rndTmpValue == 1)
                    {
                        rndTmpValue = OutRndNum(0, 15);
                        //7일확률 6/15
                        if (rndTmpValue < 6)
                        {
                            isDouble = false;
                            getDiceNum = 7;
                        }
                        //8일확률 5/15
                        else if (rndTmpValue > 5 && rndTmpValue < 11)
                        {
                            //1/5확률로 더블
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 8;
                        }
                        //9일확률 4/15
                        else
                        {
                            isDouble = false;
                            getDiceNum = 9;
                        }
                    }
                    //10-12
                    else if (rndTmpValue == 2)
                    {
                        rndTmpValue = OutRndNum(0, 6);
                        //10일확률 3/6
                        if (rndTmpValue < 3)
                        {
                            isDouble = false;
                            //1/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 10;
                        }
                        //11일확률 2/6
                        else if (rndTmpValue > 2 && rndTmpValue < 5)
                        {
                            isDouble = false;
                            getDiceNum = 11;
                        }
                        //12일확률 1/6
                        else
                        {
                            //무조건 더블
                            isDouble = true;
                            getDiceNum = 12;
                        }
                    }
                }
                break;
            case 3:
                //7-9
                if (OutRndNum(0, 100) < 40 + characterDiceRateUp)
                {
                    rndTmpValue = OutRndNum(0, 15);
                    //7일확률 6/15
                    if (rndTmpValue < 6)
                    {
                        isDouble = false;
                        getDiceNum = 7;
                    }
                    //8일확률 5/15
                    else if (rndTmpValue > 5 && rndTmpValue < 11)
                    {
                        isDouble = false;
                        //1/5확률로 더블
                        rndTmpValue = OutRndNum(0, 5);
                        if (rndTmpValue < 1)
                        {
                            //이때 더블
                            isDouble = true;
                        }
                        getDiceNum = 8;
                    }
                    //9일확률 4/15
                    else
                    {
                        isDouble = false;
                        getDiceNum = 9;
                    }
                }

                else
                {
                    rndTmpValue = OutRndNum(0, 3);
                    //4-6
                    if (rndTmpValue == 0)
                    {
                        rndTmpValue = OutRndNum(0, 12);
                        //4일확률 3/12
                        if (rndTmpValue < 3)
                        {
                            // 2/3확률로 더블
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue < 2)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 4;

                        }
                        //5일확률 4/12
                        else if (rndTmpValue > 2 && rndTmpValue < 7)
                        {
                            isDouble = false;
                            getDiceNum = 5;
                        }
                        //6일확률 5/12
                        else
                        {
                            // 1/5확률로 더블
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 6;
                        }
                    }
                    //2-3
                    else if (rndTmpValue == 1)
                    {
                        //2일확률 1/3
                        if (OutRndNum(0, 3) < 1)
                        {
                            //무조건더블 11
                            isDouble = true;
                            getDiceNum = 2;
                        }
                        //3일확률 2/3 
                        else
                        {
                            isDouble = false;
                            getDiceNum = 3;
                        }
                    }
                    //10-12
                    else if (rndTmpValue == 2)
                    {
                        rndTmpValue = OutRndNum(0, 6);
                        //10일확률 3/6
                        if (rndTmpValue < 3)
                        {
                            //1/3확률로 더블
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 10;
                        }
                        //11일확률 2/6
                        else if (rndTmpValue > 2 && rndTmpValue < 5)
                        {
                            isDouble = false;
                            getDiceNum = 11;
                        }
                        //12일확률 1/6
                        else
                        {
                            //무조건 더블
                            isDouble = true;
                            getDiceNum = 12;
                        }
                    }
                }
                break;
            case 4:
                //10-12
                if (OutRndNum(0, 100) < 40 + characterDiceRateUp)
                {
                    rndTmpValue = OutRndNum(0, 6);
                    //10일확률 3/6
                    if (rndTmpValue < 3)
                    {
                        //1/3확률로 더블
                        isDouble = false;
                        rndTmpValue = OutRndNum(0, 3);
                        if (rndTmpValue < 1)
                        {
                            //이때 더블
                            isDouble = true;
                        }
                        getDiceNum = 10;
                    }
                    //11일확률 2/6
                    else if (rndTmpValue > 2 && rndTmpValue < 5)
                    {
                        isDouble = false;
                        getDiceNum = 11;
                    }
                    //12일확률 1/6
                    else
                    {
                        //무조건 더블
                        isDouble = true;
                        getDiceNum = 12;
                    }
                }

                else
                {
                    rndTmpValue = OutRndNum(0, 3);
                    //4-6
                    if (rndTmpValue == 0)
                    {
                        rndTmpValue = OutRndNum(0, 12);
                        //4일확률 3/12
                        if (rndTmpValue < 3)
                        {
                            // 2/3확률로 더블
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue < 2)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 4;

                        }
                        //5일확률 4/12
                        else if (rndTmpValue > 2 && rndTmpValue < 7)
                        {
                            isDouble = false;
                            getDiceNum = 5;
                        }
                        //6일확률 5/12
                        else
                        {
                            // 1/5확률로 더블
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 6;
                        }
                    }
                    //7-9
                    else if (rndTmpValue == 1)
                    {
                        rndTmpValue = OutRndNum(0, 15);
                        //7일확률 6/15
                        if (rndTmpValue < 6)
                        {
                            isDouble = false;
                            getDiceNum = 7;
                        }
                        //8일확률 5/15
                        else if (rndTmpValue > 5 && rndTmpValue < 11)
                        {
                            //1/5확률로 더블
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue < 1)
                            {
                                //이때 더블
                                isDouble = true;
                            }
                            getDiceNum = 8;
                        }
                        //9일확률 4/15
                        else
                        {
                            isDouble = false;
                            getDiceNum = 9;
                        }
                    }
                    //2-3
                    else if (rndTmpValue == 2)
                    {
                        //2일확률 1/3
                        if (OutRndNum(0, 3) < 1)
                        {
                            //무조건더블 11
                            isDouble = true;
                            getDiceNum = 2;
                        }
                        //3일확률 2/3 
                        else
                        {
                            isDouble = false;
                            getDiceNum = 3;
                        }
                    }
                }
                break;
        }
        Debug.Log("다이스눈금 : " + getDiceNum + " 더블여부 : " + isDouble);
        CameraController.DiceCount = getDiceNum;

        //flow 변경
        GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>().FlowChange(FLOW.TO_ROLLINGDICE);
        //getDiceNum = 0; // 잠시 주석처리할게요!
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
