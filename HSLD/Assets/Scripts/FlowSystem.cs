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
    TO_PICKINGEVENTCARDLOC,
    TO_PICKINGEVENTSECLECTTERRAIN,
    ENEMYTURN_ROLLINGDICE,
    ENEMYTURN_PICKINGCARD,
    ENEMYTURN_PICKINGLOC,
    ENEMYTURN_PICKEVENTCARD,
    ENEMYTURN_PICKINGEVENTCARDLOC,
    ENEMYTURN_PICKINGEVENTSELECTTERRAIN,
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
    public GameObject turnTimerImg;
    public GameObject tmpAnimationImage;
    public Transform missionSetParentTransform;
    public GameObject missionCanvas;
    public GameObject displayText;

    private float time_;
    private int randomVal;
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
                    if (gameObject.GetComponent<TurnSystem>().currentTurn.Equals(TURN.MYTURN))
                    {
                        displayText.GetComponent<DisplayText>().text = "상대 턴";
                        displayText.SetActive(true);
                    }
                    else
                    {
                        displayText.GetComponent<DisplayText>().text = "나의 턴";
                        displayText.SetActive(true);
                    }
                    yield return new WaitForSeconds(time);
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
            case FLOW.TO_PICKINGEVENTCARDLOC:
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
            case FLOW.READY_DONE:
                gameObject.GetComponent<TurnSystem>().TurnSet();
                break;
        }
        if(animationImg)
            tmpAnimationImage.SetActive(false);
    }

    private void Start()
    {
        SoundManager.instance_.BGMChange(SoundManager.instance_.clips[14]);

        //테스트버전
        if (currentFlow.Equals(FLOW.TSETVER))
            FlowChange(currentFlow);
    }


    IEnumerator DisplayLoadingCor()
    {
        //서버가 대기신호보내고 아무것도안함, 서버가 없으면 바로 시작
        if (GameObject.Find("GameCores") == null)
        {
            Debug.Log("11");
            displayText.GetComponent<DisplayText>().text = "GAME START!";
            displayText.SetActive(true);
            yield return new WaitForSeconds(4.5f);
            currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
            FlowChange(FLOW.READY_DONE);
        }

        else
        {
            StartCoroutine(GameObject.Find("SceneControlManager").GetComponent<SceneControlManager>().DrawOnlyLoadUI());
            yield return new WaitForSeconds(2f);
            displayText.GetComponent<DisplayText>().text = "GAME START!";
            displayText.SetActive(true);
            yield return new WaitForSeconds(4.5f);
            displayText.SetActive(true);
            gameObject.GetComponent<InGameSceneManager>().StartWaitCoroutine();

        }
            

    }

    IEnumerator DiceActiveOff()
    {
        yield return new WaitForSeconds(1f);
        diceCanvas.SetActive(false);
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
                //tmpAnimationImage.SetActive(true);
                spinCanvas.SetActive(true);
                readyCanvas.SetActive(false);

                //init - card Cnt Update
                gameObject.GetComponent<CardSystem>().CardCntUpdate();
                StartCoroutine(DisplayLoadingCor());
                break;

            case FLOW.READY_DONE:
                tmpAnimationImage.SetActive(false);
                Debug.Log("22");
                if(gameObject.GetComponent<TurnSystem>().currentTurn.Equals(TURN.MYTURN))
                {
                    displayText.GetComponent<DisplayText>().text = "나의 턴";
                    displayText.SetActive(true);
                }
                else
                {
                    displayText.GetComponent<DisplayText>().text = "상대 턴";
                    displayText.SetActive(true);
                }
                StartCoroutine(DisplayEventWaitingTime(FLOW.READY_DONE, 4, false));    // <<< 여기  5라는 숫자를 바꾸면댐
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
                SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[8], 1.0f);

                //애니메이션 여기
                StartCoroutine(DiceActiveOff());
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
                        gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(GameObject.Find(AllMeshController.instance_.PickContainer[1].ToString()), AllMeshController.instance_.MovingObj[0], 1.03f, AllMeshController.instance_.PickContainer[1], gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.cardIndex);
                        break;
                    //건조 - 뱀
                    case 2:
                        gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(GameObject.Find(AllMeshController.instance_.PickContainer[1].ToString()), AllMeshController.instance_.MovingObj[6], 1.03f, AllMeshController.instance_.PickContainer[1], gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.cardIndex);
                        break;
                    //한랭 - 펭귄
                    case 3:
                        randomVal = Random.Range(1, 4);
                        gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(GameObject.Find(AllMeshController.instance_.PickContainer[1].ToString()), AllMeshController.instance_.MovingObj[randomVal], 1.03f, AllMeshController.instance_.PickContainer[1], gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.cardIndex);
                        break;
                    //바다 - 고래
                    case 4:
                        randomVal = Random.Range(4, 6);
                        gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(GameObject.Find(AllMeshController.instance_.PickContainer[1].ToString()), AllMeshController.instance_.MovingObj[randomVal], 5f, AllMeshController.instance_.PickContainer[1], gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.cardIndex);
                        break;
                    //산 - 구름
                    case 5:
                        randomVal = Random.Range(7, 9);
                        gameObject.GetComponent<BuildOnPlanet>().EulerRotCal(GameObject.Find(AllMeshController.instance_.PickContainer[1].ToString()), AllMeshController.instance_.MovingObj[randomVal], 15f, AllMeshController.instance_.PickContainer[1], gameObject.GetComponent<CardSystem>().pickedCard.GetComponent<CardData>().data.cardIndex);
                        break;
                }
                Camera.main.GetComponent<PCverPIcking>().TurnChangeLogic();                
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_PICKINGLOC, 5, false));    // <<< 여기  5라는 숫자를 바꾸면댐
                break;
            case FLOW.TO_PICKEVENTCARD:
                break;
            case FLOW.TO_PICKINGEVENTSECLECTTERRAIN:
                break;
            case FLOW.TO_PICKINGEVENTCARDLOC:
                if (gameObject.GetComponent<TurnSystem>().currentTurn.Equals(TURN.MYTURN))
                {
                    displayText.GetComponent<DisplayText>().text = "상대 턴";
                    displayText.SetActive(true);
                }
                else
                {
                    displayText.GetComponent<DisplayText>().text = "나의 턴";
                    displayText.SetActive(true);
                }
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_PICKINGEVENTCARDLOC, 2, true));    // <<< 여기  2라는 숫자를 바꾸면댐
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
                //if (/*상대방카드 index = 101, 111,202 && TurnSystem.enemyEventCardDefense*/)
                //{
                //    //상대 턴 종료.
                //    //애니메이션 후, 내턴 시작
                //}
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN;
                gameObject.GetComponent<TurnSystem>().TurnSet();
                //StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_PICKEVENTCARD, 2, true));
                break;
            case FLOW.ENEMYTURN_PICKINGEVENTCARDLOC:
                if (gameObject.GetComponent<TurnSystem>().currentTurn.Equals(TURN.MYTURN))
                {
                    displayText.GetComponent<DisplayText>().text = "상대 턴";
                    displayText.SetActive(true);
                }
                else
                {
                    displayText.GetComponent<DisplayText>().text = "나의 턴";
                    displayText.SetActive(true);
                }
                break;
            case FLOW.ENEMYTURN_PICKINGEVENTSELECTTERRAIN:
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
