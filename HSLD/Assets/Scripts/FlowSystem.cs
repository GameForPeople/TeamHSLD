﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum FLOW
{
    READY_MATCHINGCOMPLETE,
    READY_TURNORDER,
    READY_SETCARD,
    READY_DONE,
    READY_WAITING,
    DISPLAYANIMATION_WAITING,
    TO_ROLLINGDICE,
    TO_PICKINGCARD,
    TO_PICKINGLOC,
    TO_PICKEVENTCARD,
    ENEMYTURN_ROLLINGDICE,
    ENEMYTURN_PICKINGCARD,
    ENEMYTURN_PICKINGLOC,
    ENEMYTURN_PICKEVENTCARD,
    TSETVER
}


public class FlowSystem : MonoBehaviour
{
    public FLOW currentFlow;

    public GameObject cardSetCanvas;
    public GameObject turnSetCanvas;
    public GameObject readyCanvas;
    public GameObject matchingCompleteCanvas;
    public GameObject spinCanvas;
    public GameObject diceCanvas;
    public GameObject setTerrainCanvas;
    public GameObject enemyImage;
    public GameObject turnTimerImg;
    public GameObject tmpAnimationImage;
    public Transform missionSetParentTransform;
    public GameObject missionCanvas;

    private float time_;

    //이벤트 연출시간이 끝난다음에 다음 상태 진행.
    IEnumerator DisplayEventWaitingTime(FLOW beforeFlow, float time, bool animationImg)
    {
        if(animationImg)
            tmpAnimationImage.SetActive(true);
        currentFlow = FLOW.DISPLAYANIMATION_WAITING;
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            yield return new WaitForEndOfFrame();
            if (time_ > time)
                break;
        }

        switch (beforeFlow)
        {
            case FLOW.TO_ROLLINGDICE:
                currentFlow = FLOW.TO_PICKINGCARD;
                break;
            case FLOW.TO_PICKINGLOC:
                TurnSystem.isSetTerrainDone = false;
                
                //init
                gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.currentCnt -= 1;
                gameObject.GetComponent<CardSystem>().CardCntUpdate();
                gameObject.GetComponent<CardSystem>().pickedCard = null;
                gameObject.GetComponent<CardSystem>().CardPosInit();

                //이벤트카드로 갈지 말지 분기
                if(DiceSystem.isDouble /*|| 서브미션 달성시*/)
                {
                    currentFlow = FLOW.TO_PICKEVENTCARD;
                    gameObject.GetComponent<EventCardManager>().EventCardInstate();
                }
                else
                {
                    if (GameObject.Find("GameCores") != null)
                    {
                        Debug.Log("SEND : 턴종료");
                        gameObject.GetComponent<InGameSceneManager>().SendChangeTurn();
                    }

                    currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
                    gameObject.GetComponent<TurnSystem>().currentTurn = TURN.ENEMYTURN;
                    gameObject.GetComponent<TurnSystem>().TurnSet();
                }                
                break;
            case FLOW.TO_PICKEVENTCARD:
                if (GameObject.Find("GameCores") != null)
                {
                    Debug.Log("SEND : 턴종료");
                    gameObject.GetComponent<InGameSceneManager>().SendChangeTurn();
                }
                currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.ENEMYTURN;
                gameObject.GetComponent<TurnSystem>().TurnSet();
                break;
            case FLOW.ENEMYTURN_ROLLINGDICE:
                currentFlow = FLOW.ENEMYTURN_PICKINGCARD;
                break;
            //case FLOW.ENEMYTURN_PICKINGLOC:
            //    gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN;
            //    gameObject.GetComponent<TurnSystem>().TurnSet();
            //    break;
            
            //case FLOW.ENEMYTURN_PICKEVENTCARD:
            //    gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN;
            //    //gameObject.GetComponent<TurnSystem>().TurnSet();
            //    break;

        }
        if(animationImg)
            tmpAnimationImage.SetActive(false);
    }

    private void Start()
    {
        //사운드 임시
        if (SoundManager.instance_ != null)
            SoundManager.instance_.BGMMixing(SoundManager.instance_.clips[1], 2.0f);

        //테스트버전
        if (currentFlow.Equals(FLOW.TSETVER))
            FlowChange(currentFlow);
    }

    public void FlowChange(FLOW doneFlow)
    {
        switch(doneFlow)
        {
            case FLOW.READY_MATCHINGCOMPLETE:
                matchingCompleteCanvas.SetActive(false);
                turnSetCanvas.SetActive(true);
                currentFlow = FLOW.READY_TURNORDER;
                break;
            case FLOW.READY_TURNORDER:
                turnSetCanvas.SetActive(false);
                cardSetCanvas.SetActive(true);
                currentFlow = FLOW.READY_SETCARD;
                break;
            case FLOW.READY_SETCARD:
                cardSetCanvas.SetActive(false);
                tmpAnimationImage.SetActive(true);
                spinCanvas.SetActive(true);
                readyCanvas.SetActive(false);

                //init - card Cnt Update
                gameObject.GetComponent<CardSystem>().CardCntUpdate();

                //서버가 대기신호보내고 아무것도안함, 서버가 없으면 바로 시작
                if (GameObject.Find("GameCores") == null)
                {
                    currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
                    FlowChange(FLOW.READY_DONE);
                }

                else
                    gameObject.GetComponent<InGameSceneManager>().StartWaitCoroutine();
                    
                break;

            case FLOW.READY_DONE:
                tmpAnimationImage.SetActive(false);
                gameObject.GetComponent<TurnSystem>().TurnSet();
                break;
            case FLOW.READY_WAITING:
                currentFlow = FLOW.TO_ROLLINGDICE;
                break;
            case FLOW.TO_PICKINGCARD:
                GameObject.FindWithTag("MainCamera").GetComponent<PCverPIcking>().enabled = true;
                setTerrainCanvas.SetActive(true);
                if (GameObject.Find("GameCores") != null)
                {
                    GameObject picked = AllMeshController.IngameManager.GetComponent<CardSystem>().pickedCard; 
                    gameObject.GetComponent<InGameSceneManager>().SendTerrainType(picked.GetComponent<CardData>().data.cardIndex);
                }
                currentFlow = FLOW.TO_PICKINGLOC;
                break;
            case FLOW.TO_ROLLINGDICE:
                if (SoundManager.instance_ != null)
                    SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[5], 1.0f);

                //애니메이션 여기
                diceCanvas.SetActive(false);
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_ROLLINGDICE, 5, true));    // <<< 여기  5라는 숫자를 바꾸면댐
                break;

            //이벤트카드가 없다면 바로 대기상태로 변경
            case FLOW.TO_PICKINGLOC:
                //애니메이션 여기
                
                setTerrainCanvas.SetActive(false);
                TurnSystem.isSetTerrainDone = true;
                switch (gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.cardIndex)
                {
                    //비옥 - 병아리
                    case 1:
                        gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(GameObject.Find(AllMeshController.instance_.PickContainer[1].ToString()), AllMeshController.instance_.MovingObj[0], 1.03f, AllMeshController.instance_.PickContainer[1],0);
                        break;
                    //한랭 - 펭귄
                    case 3:
                        gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(GameObject.Find(AllMeshController.instance_.PickContainer[1].ToString()), AllMeshController.instance_.MovingObj[1], 1.03f, AllMeshController.instance_.PickContainer[1],1);
                        break;
                    //바다 - 고래
                    case 4:
                        gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(GameObject.Find(AllMeshController.instance_.PickContainer[1].ToString()), AllMeshController.instance_.MovingObj[2], 5f, AllMeshController.instance_.PickContainer[1],2);
                        break;
                }
                Camera.main.GetComponent<PCverPIcking>().TurnChangeLogic();                
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_PICKINGLOC, 5, false));    // <<< 여기  5라는 숫자를 바꾸면댐
                break;
            case FLOW.TO_PICKEVENTCARD:
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_PICKEVENTCARD, 2, true));    // <<< 여기  2라는 숫자를 바꾸면댐
                break;
            case FLOW.ENEMYTURN_ROLLINGDICE:
                StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_ROLLINGDICE, 5, true));
                break;
            case FLOW.ENEMYTURN_PICKINGCARD:
                currentFlow = FLOW.ENEMYTURN_PICKINGLOC;
                break;
            //거점을 정복했는지 여부에따라서 분기
            case FLOW.ENEMYTURN_PICKINGLOC:
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN;
                gameObject.GetComponent<TurnSystem>().TurnSet();
                //StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_PICKINGLOC, 5, false));
                break;
            case FLOW.ENEMYTURN_PICKEVENTCARD:
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN;
                gameObject.GetComponent<TurnSystem>().TurnSet();
                //StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_PICKEVENTCARD, 2, true));
                break;
            case FLOW.TSETVER:
                GameObject.FindWithTag("MainCamera").GetComponent<PCverPIcking>().enabled = true;
                cardSetCanvas.SetActive(false);
                readyCanvas.SetActive(false);
                spinCanvas.SetActive(true);
                currentFlow = FLOW.TO_PICKINGLOC;
                break;
        }
    }
}
