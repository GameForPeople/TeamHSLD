﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TutorialTurnSystem : MonoBehaviour
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

    private float warningTime;
    public GameObject warningPanel;

    private float warningRadio = 0;
    static public bool isSetTerrainDone = false;
    static public bool enemyEventCardDefense = false;           //이벤트 카드로 인해 상대방의 이벤트카드를 막는다.

    Coroutine myCoroutine;
    Coroutine enemyCoroutine;
    Coroutine TurnPassAndDisplayTextCor;

    //최적화 스크립트
    private TutorialFlowSystem flowSystem;
    private TutorialSetTurn setTurn;
    private CardSystem cardsystem;

    private bool[] chk = new bool[5];

    private void Start()
    {
        flowSystem = gameObject.GetComponent<TutorialFlowSystem>();
        setTurn = gameObject.GetComponent<TutorialSetTurn>();
        cardsystem = gameObject.GetComponent<CardSystem>();
        StartCoroutine(ReadyMatchingComplete());
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

            if (myCoroutine != null)
                StopCoroutine(myCoroutine);
            //myCoroutine = StartCoroutine(MyTurnCounting());
        }
        //내턴이아닐때의 코루틴 진입
        else
        {
            flowSystem.enemyTurnPassObj.SetActive(true);
            flowSystem.currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
            flowSystem.diceCanvas.SetActive(false);
            //StartCoroutine(EndTurnAndWaiting());
        }
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
            yield return new WaitForEndOfFrame();
            if (TutorialSetTurn.isDone)
                break;
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
            for (int i = 0; i < chk.Length; i++)
                chk[i] = false;

            for (int i =0; i< cardsystem.cardSet.Length; i++)
            {
                if(i < 3)
                {
                    if(cardsystem.cardSet[i].GetComponent<CardData>().data.currentCnt == 8)
                    {
                        chk[i] = true;
                    }
                    
                }
                else
                {
                    if (cardsystem.cardSet[i].GetComponent<CardData>().data.currentCnt == 3)
                    {
                        chk[i] = true;
                    }
                }
            }

            int cnt = 0;
            for (int i =0;i< chk.Length;i++)
            {
                if (chk[i] == true)
                    cnt += 1;
            }
            if (cnt == 5)
            {
                gameObject.GetComponent<TutorialManager>().pointOff();
                break;
            }
                

            yield return new WaitForEndOfFrame();
            
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
    
    public void EnemyTurnPass()
    {
        flowSystem.currentFlow = FLOW.DISPLAYANIMATION_WAITING;
        flowSystem.displayTextImg.SetActive(true);
        flowSystem.displayTextImg.GetComponent<DisplayTextImg>().Performance(flowSystem.displayTextImg.GetComponent<DisplayTextImg>().sprs[1]);

        if (TurnPassAndDisplayTextCor != null)
            StopCoroutine(TurnPassAndDisplayTextCor);
        TurnPassAndDisplayTextCor = StartCoroutine(TurnPassAndDisplayText());
    }

    IEnumerator TurnPassAndDisplayText()
    {
        yield return new WaitForSeconds(2.5f);
        currentTurn = TURN.MYTURN;
    }
}
