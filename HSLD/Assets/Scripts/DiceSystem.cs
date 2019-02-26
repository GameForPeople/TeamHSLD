using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DiceSystem : MonoBehaviour
{
    public Slider diceSlider;
    public static int getDiceNum;
    public GameObject pointer;
    private int diceValue;
    static public bool isDouble;                //주사위가 더블이 나왔을때
    private int rndTmpValue;
    private bool isTriggerEnter = false;
    private float gaze;
    private bool raiseValue = true;
    public float constValue;
    private int characterDiceRateUp = 5;        //tmp
    public GameObject doubleImg;

    static public DiceSystem instance_;

    private Color initColor = Color.yellow;
    private Color desColor = Color.red;

    static public bool isDiceDouble = false;            //이벤트카드로 인해 주사위를 더블할때
    private float time_;

    private FlowSystem flowSystem;
    private MissionManager missionManager;

    private void Start()
    {
        instance_ = this;
        flowSystem = GameObject.FindWithTag("GameManager").GetComponent<FlowSystem>();
        missionManager = GameObject.FindWithTag("GameManager").GetComponent<MissionManager>();
    }

    public void OnTrigger()
    {
        if (!flowSystem.currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            return;

        //init
        diceSlider.value = 0;
        gaze = 0;
        time_ = 0;
        isTriggerEnter = true;
    }
    public void OffTrigger()
    {
        if (!flowSystem.currentFlow.Equals(FLOW.TO_ROLLINGDICE))
            return;

        isTriggerEnter = false;
        //0123
        if ((int)(gaze*10) < 4)
            RollingDice(1);

        //456
        else if ((int)(gaze * 10) > 3 && (int)gaze < 7)
            RollingDice(2);

        //78
        else if ((int)(gaze * 10) > 6 && (int)gaze < 9)
            RollingDice(3);

        //9
        else if ((int)(gaze * 10) > 8)
            RollingDice(4);
    }

    //랜덤 넘버 추출
    int OutRndNum(int min, int max)
    {
        return Random.Range(min, max);
    }

    //슬라이드 값을 바탕으로 다이스 눈 설정.
    public void RollingDice(int index)
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
                        getDiceNum = 11;
                    }
                    //3일확률 2/3 
                    else
                    {
                        isDouble = false;
                        if (OutRndNum(0, 2) < 1)
                            getDiceNum = 12;
                        else
                            getDiceNum = 21;
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
                            // 1/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 22;
                            }
                            else if(rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 31;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 31;
                            }
                        }
                        //5일확률 4/12
                        else if (rndTmpValue > 2 && rndTmpValue < 7)
                        {
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 4);
                            if (rndTmpValue == 0)
                                getDiceNum = 14;
                            else if (rndTmpValue == 1)
                                getDiceNum = 23;
                            else if (rndTmpValue == 2)
                                getDiceNum = 32;
                            else if (rndTmpValue == 3)
                                getDiceNum = 41;
                        }
                        //6일확률 5/12
                        else
                        {
                            // 1/5확률로 더블
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 33;
                            }
                            else if (rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 15;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 24;
                            }
                            else if (rndTmpValue == 3)
                            {
                                isDouble = false;
                                getDiceNum = 42;
                            }
                            else if (rndTmpValue == 4)
                            {
                                isDouble = false;
                                getDiceNum = 51;
                            }
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
                            rndTmpValue = OutRndNum(0, 6);
                            if(rndTmpValue == 0)
                                getDiceNum = 16;
                            else if (rndTmpValue == 1)
                                getDiceNum = 25;
                            else if (rndTmpValue == 2)
                                getDiceNum = 34;
                            else if (rndTmpValue == 3)
                                getDiceNum = 43;
                            else if (rndTmpValue == 4)
                                getDiceNum = 52;
                            else if (rndTmpValue == 5)
                                getDiceNum = 61;
                        }
                        //8일확률 5/15
                        else if (rndTmpValue > 5 && rndTmpValue < 11)
                        {
                            //1/5확률로 더블
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 44;
                            }
                            else if(rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 26;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 35;
                            }
                            else if (rndTmpValue == 3)
                            {
                                isDouble = false;
                                getDiceNum = 53;
                            }
                            else if (rndTmpValue == 4)
                            {
                                isDouble = false;
                                getDiceNum = 62;
                            }
                        }
                        //9일확률 4/15
                        else
                        {
                            rndTmpValue = OutRndNum(0, 4);
                            isDouble = false;
                            if(rndTmpValue == 0)
                                getDiceNum = 36;
                            else if (rndTmpValue == 1)
                                getDiceNum = 45;
                            else if (rndTmpValue == 2)
                                getDiceNum = 54;
                            else if (rndTmpValue == 3)
                                getDiceNum = 63;
                        }
                    }
                    //10-12
                    else
                    {
                        rndTmpValue = OutRndNum(0, 6);
                        //10일확률 3/6
                        if (rndTmpValue < 3)
                        {
                            //1/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 55;
                            }
                            else if(rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 46;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 64;
                            }

                        }
                        //11일확률 2/6
                        else if (rndTmpValue > 2 && rndTmpValue < 5)
                        {
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 2);
                            if (rndTmpValue == 0)
                                getDiceNum = 56;
                            else
                                getDiceNum = 65;
                        }
                        //12일확률 1/6
                        else
                        {
                            //무조건 더블
                            isDouble = true;
                            getDiceNum = 66;
                        }
                    }
                }
                break;
            case 2:
                if (OutRndNum(0, 100) < 40 + characterDiceRateUp)
                {
                    rndTmpValue = OutRndNum(0, 12);
                    //4일확률 3/12
                    if (rndTmpValue < 3)
                    {
                        // 1/3확률로 더블
                        rndTmpValue = OutRndNum(0, 3);
                        if (rndTmpValue == 0)
                        {
                            //이때 더블
                            isDouble = true;
                            getDiceNum = 22;
                        }
                        else if (rndTmpValue == 1)
                        {
                            isDouble = false;
                            getDiceNum = 31;
                        }
                        else if (rndTmpValue == 2)
                        {
                            isDouble = false;
                            getDiceNum = 31;
                        }
                    }
                    //5일확률 4/12
                    else if (rndTmpValue > 2 && rndTmpValue < 7)
                    {
                        isDouble = false;
                        rndTmpValue = OutRndNum(0, 4);
                        if (rndTmpValue == 0)
                            getDiceNum = 14;
                        else if (rndTmpValue == 1)
                            getDiceNum = 23;
                        else if (rndTmpValue == 2)
                            getDiceNum = 32;
                        else if (rndTmpValue == 3)
                            getDiceNum = 41;
                    }
                    //6일확률 5/12
                    else
                    {
                        // 1/5확률로 더블
                        rndTmpValue = OutRndNum(0, 5);
                        if (rndTmpValue == 0)
                        {
                            //이때 더블
                            isDouble = true;
                            getDiceNum = 33;
                        }
                        else if (rndTmpValue == 1)
                        {
                            isDouble = false;
                            getDiceNum = 15;
                        }
                        else if (rndTmpValue == 2)
                        {
                            isDouble = false;
                            getDiceNum = 24;
                        }
                        else if (rndTmpValue == 3)
                        {
                            isDouble = false;
                            getDiceNum = 42;
                        }
                        else if (rndTmpValue == 4)
                        {
                            isDouble = false;
                            getDiceNum = 51;
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
                            getDiceNum = 11;
                        }
                        //3일확률 2/3 
                        else
                        {
                            isDouble = false;
                            if (OutRndNum(0, 2) < 1)
                                getDiceNum = 12;
                            else
                                getDiceNum = 21;
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
                            rndTmpValue = OutRndNum(0, 6);
                            if (rndTmpValue == 0)
                                getDiceNum = 16;
                            else if (rndTmpValue == 1)
                                getDiceNum = 25;
                            else if (rndTmpValue == 2)
                                getDiceNum = 34;
                            else if (rndTmpValue == 3)
                                getDiceNum = 43;
                            else if (rndTmpValue == 4)
                                getDiceNum = 52;
                            else if (rndTmpValue == 5)
                                getDiceNum = 61;
                        }
                        //8일확률 5/15
                        else if (rndTmpValue > 5 && rndTmpValue < 11)
                        {
                            //1/5확률로 더블
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 44;
                            }
                            else if (rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 26;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 35;
                            }
                            else if (rndTmpValue == 3)
                            {
                                isDouble = false;
                                getDiceNum = 53;
                            }
                            else if (rndTmpValue == 4)
                            {
                                isDouble = false;
                                getDiceNum = 62;
                            }
                        }
                        //9일확률 4/15
                        else
                        {
                            rndTmpValue = OutRndNum(0, 4);
                            isDouble = false;
                            if (rndTmpValue == 0)
                                getDiceNum = 36;
                            else if (rndTmpValue == 1)
                                getDiceNum = 45;
                            else if (rndTmpValue == 2)
                                getDiceNum = 54;
                            else if (rndTmpValue == 3)
                                getDiceNum = 63;
                        }
                    }
                    //10-12
                    else
                    {
                        rndTmpValue = OutRndNum(0, 6);
                        //10일확률 3/6
                        if (rndTmpValue < 3)
                        {
                            //1/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 55;
                            }
                            else if (rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 46;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 64;
                            }

                        }
                        //11일확률 2/6
                        else if (rndTmpValue > 2 && rndTmpValue < 5)
                        {
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 2);
                            if (rndTmpValue == 0)
                                getDiceNum = 56;
                            else
                                getDiceNum = 65;
                        }
                        //12일확률 1/6
                        else
                        {
                            //무조건 더블
                            isDouble = true;
                            getDiceNum = 66;
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
                        rndTmpValue = OutRndNum(0, 6);
                        if (rndTmpValue == 0)
                            getDiceNum = 16;
                        else if (rndTmpValue == 1)
                            getDiceNum = 25;
                        else if (rndTmpValue == 2)
                            getDiceNum = 34;
                        else if (rndTmpValue == 3)
                            getDiceNum = 43;
                        else if (rndTmpValue == 4)
                            getDiceNum = 52;
                        else if (rndTmpValue == 5)
                            getDiceNum = 61;
                    }
                    //8일확률 5/15
                    else if (rndTmpValue > 5 && rndTmpValue < 11)
                    {
                        //1/5확률로 더블
                        rndTmpValue = OutRndNum(0, 5);
                        if (rndTmpValue == 0)
                        {
                            //이때 더블
                            isDouble = true;
                            getDiceNum = 44;
                        }
                        else if (rndTmpValue == 1)
                        {
                            isDouble = false;
                            getDiceNum = 26;
                        }
                        else if (rndTmpValue == 2)
                        {
                            isDouble = false;
                            getDiceNum = 35;
                        }
                        else if (rndTmpValue == 3)
                        {
                            isDouble = false;
                            getDiceNum = 53;
                        }
                        else if (rndTmpValue == 4)
                        {
                            isDouble = false;
                            getDiceNum = 62;
                        }
                    }
                    //9일확률 4/15
                    else
                    {
                        rndTmpValue = OutRndNum(0, 4);
                        isDouble = false;
                        if (rndTmpValue == 0)
                            getDiceNum = 36;
                        else if (rndTmpValue == 1)
                            getDiceNum = 45;
                        else if (rndTmpValue == 2)
                            getDiceNum = 54;
                        else if (rndTmpValue == 3)
                            getDiceNum = 63;
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
                            // 1/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 22;
                            }
                            else if (rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 31;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 31;
                            }
                        }
                        //5일확률 4/12
                        else if (rndTmpValue > 2 && rndTmpValue < 7)
                        {
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 4);
                            if (rndTmpValue == 0)
                                getDiceNum = 14;
                            else if (rndTmpValue == 1)
                                getDiceNum = 23;
                            else if (rndTmpValue == 2)
                                getDiceNum = 32;
                            else if (rndTmpValue == 3)
                                getDiceNum = 41;
                        }
                        //6일확률 5/12
                        else
                        {
                            // 1/5확률로 더블
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 33;
                            }
                            else if (rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 15;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 24;
                            }
                            else if (rndTmpValue == 3)
                            {
                                isDouble = false;
                                getDiceNum = 42;
                            }
                            else if (rndTmpValue == 4)
                            {
                                isDouble = false;
                                getDiceNum = 51;
                            }
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
                            getDiceNum = 11;
                        }
                        //3일확률 2/3 
                        else
                        {
                            isDouble = false;
                            if (OutRndNum(0, 2) < 1)
                                getDiceNum = 12;
                            else
                                getDiceNum = 21;
                        }
                    }
                    //10-12
                    else
                    {
                        rndTmpValue = OutRndNum(0, 6);
                        //10일확률 3/6
                        if (rndTmpValue < 3)
                        {
                            //1/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 55;
                            }
                            else if (rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 46;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 64;
                            }

                        }
                        //11일확률 2/6
                        else if (rndTmpValue > 2 && rndTmpValue < 5)
                        {
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 2);
                            if (rndTmpValue == 0)
                                getDiceNum = 56;
                            else
                                getDiceNum = 65;
                        }
                        //12일확률 1/6
                        else
                        {
                            //무조건 더블
                            isDouble = true;
                            getDiceNum = 66;
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
                        rndTmpValue = OutRndNum(0, 3);
                        if (rndTmpValue == 0)
                        {
                            //이때 더블
                            isDouble = true;
                            getDiceNum = 55;
                        }
                        else if (rndTmpValue == 1)
                        {
                            isDouble = false;
                            getDiceNum = 46;
                        }
                        else if (rndTmpValue == 2)
                        {
                            isDouble = false;
                            getDiceNum = 64;
                        }

                    }
                    //11일확률 2/6
                    else if (rndTmpValue > 2 && rndTmpValue < 5)
                    {
                        isDouble = false;
                        rndTmpValue = OutRndNum(0, 2);
                        if (rndTmpValue == 0)
                            getDiceNum = 56;
                        else
                            getDiceNum = 65;
                    }
                    //12일확률 1/6
                    else
                    {
                        //무조건 더블
                        isDouble = true;
                        getDiceNum = 66;
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
                            // 1/3확률로 더블
                            rndTmpValue = OutRndNum(0, 3);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 22;
                            }
                            else if (rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 31;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 31;
                            }
                        }
                        //5일확률 4/12
                        else if (rndTmpValue > 2 && rndTmpValue < 7)
                        {
                            isDouble = false;
                            rndTmpValue = OutRndNum(0, 4);
                            if (rndTmpValue == 0)
                                getDiceNum = 14;
                            else if (rndTmpValue == 1)
                                getDiceNum = 23;
                            else if (rndTmpValue == 2)
                                getDiceNum = 32;
                            else if (rndTmpValue == 3)
                                getDiceNum = 41;
                        }
                        //6일확률 5/12
                        else
                        {
                            // 1/5확률로 더블
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 33;
                            }
                            else if (rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 15;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 24;
                            }
                            else if (rndTmpValue == 3)
                            {
                                isDouble = false;
                                getDiceNum = 42;
                            }
                            else if (rndTmpValue == 4)
                            {
                                isDouble = false;
                                getDiceNum = 51;
                            }
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
                            rndTmpValue = OutRndNum(0, 6);
                            if (rndTmpValue == 0)
                                getDiceNum = 16;
                            else if (rndTmpValue == 1)
                                getDiceNum = 25;
                            else if (rndTmpValue == 2)
                                getDiceNum = 34;
                            else if (rndTmpValue == 3)
                                getDiceNum = 43;
                            else if (rndTmpValue == 4)
                                getDiceNum = 52;
                            else if (rndTmpValue == 5)
                                getDiceNum = 61;
                        }
                        //8일확률 5/15
                        else if (rndTmpValue > 5 && rndTmpValue < 11)
                        {
                            //1/5확률로 더블
                            rndTmpValue = OutRndNum(0, 5);
                            if (rndTmpValue == 0)
                            {
                                //이때 더블
                                isDouble = true;
                                getDiceNum = 44;
                            }
                            else if (rndTmpValue == 1)
                            {
                                isDouble = false;
                                getDiceNum = 26;
                            }
                            else if (rndTmpValue == 2)
                            {
                                isDouble = false;
                                getDiceNum = 35;
                            }
                            else if (rndTmpValue == 3)
                            {
                                isDouble = false;
                                getDiceNum = 53;
                            }
                            else if (rndTmpValue == 4)
                            {
                                isDouble = false;
                                getDiceNum = 62;
                            }
                        }
                        //9일확률 4/15
                        else
                        {
                            rndTmpValue = OutRndNum(0, 4);
                            isDouble = false;
                            if (rndTmpValue == 0)
                                getDiceNum = 36;
                            else if (rndTmpValue == 1)
                                getDiceNum = 45;
                            else if (rndTmpValue == 2)
                                getDiceNum = 54;
                            else if (rndTmpValue == 3)
                                getDiceNum = 63;
                        }
                    }
                    //2-3
                    else
                    {
                        //2일확률 1/3
                        if (OutRndNum(0, 3) < 1)
                        {
                            //무조건더블 11
                            isDouble = true;
                            getDiceNum = 11;
                        }
                        //3일확률 2/3 
                        else
                        {
                            isDouble = false;
                            if (OutRndNum(0, 2) < 1)
                                getDiceNum = 12;
                            else
                                getDiceNum = 21;
                        }
                    }
                }
                break;
        }

        //getDiceNum = 11;
        //isDouble = false;
        CameraController.DiceCount = ((int)(getDiceNum / 10) + (int)(getDiceNum % 10));

        if (isDiceDouble)
        {
            CameraController.DiceCount *= 2;
            Destroy(doubleImg.GetComponent<IsDoubleImg>());
        }

        //isDouble = true;
        //Debug.Log("다이스눈금 : " + ((int)(getDiceNum/10) + (int)(getDiceNum % 10)) + "주사위 분리했을때 : "+ getDiceNum + " 더블여부 : " + isDouble);

        // 미션 - 300
        if (MissionManager.selectedSubMissionIndex == 1 && CameraController.DiceCount == 7)
            missionManager.SubMissionCounting(1, 0);


        GameObject.Find("DiceManager").GetComponent<DiceObject>().DiceSystem_Roll(getDiceNum / 10, getDiceNum % 10);
        if (GameObject.Find("GameCores") != null)
        {
            GameObject.FindWithTag("GameManager").GetComponent<InGameSceneManager>().SendDiceValue(getDiceNum);
            isDiceDouble = false;
        }
            
        //flow 변경
        flowSystem.FlowChange(FLOW.TO_ROLLINGDICE);
    }

    //슬라이드 값 설정
    void SetSliderValue()
    {
        if (diceSlider.value == diceSlider.maxValue)
            raiseValue = false;

        if (diceSlider.value == diceSlider.minValue)
            raiseValue = true;

        if (raiseValue)
            gaze += (Time.deltaTime * constValue);
        else
            gaze -= (Time.deltaTime * constValue);

        diceSlider.value = gaze;
        pointer.transform.localEulerAngles = Vector3.Lerp(new Vector3(0, 0, 92), new Vector3(0, 0, -95), gaze);
        //gameObject.GetComponent<Image>().color = Color.Lerp(initColor, desColor, gaze);
        //diceSlider.fillRect.GetComponent<Image>().color = Color.Lerp(initColor, desColor, gaze);
    }

    private void Update()
    {
        if (isTriggerEnter && flowSystem.currentFlow.Equals(FLOW.TO_ROLLINGDICE))
        {
            time_ += Time.deltaTime;
            if (time_ > 6f)
            {
                Debug.Log("예외처리 : 주사위 클릭했는데 6초가 지났을때");
                isTriggerEnter = false;
                RollingDice(Random.Range(1, 5));
            }
            if (Input.GetMouseButton(0))
            {
                SetSliderValue();
            }
        }
        else if (!isTriggerEnter)
            time_ = 0;
    }

    private void OnDisable()
    {
        gaze = 0;
        diceSlider.value = gaze;
    }
}
