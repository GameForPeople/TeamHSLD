using System.Collections;
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

    public GameObject tmpAnimationImage;

    private float time_;
    static public bool isWaitingTime = false;

    //이벤트 연출시간이 끝난다음에 다음 상태 진행.
    IEnumerator DisplayEventWaitingTime(FLOW beforeFlow, float time, bool animationImg)
    {
        if(animationImg)
            tmpAnimationImage.SetActive(true);
        isWaitingTime = true;
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
                if (gameObject.GetComponent<CardSystem>().cardSet.Length > 5)
                {
                    currentFlow = FLOW.TO_PICKEVENTCARD;
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
                    GameObject picked = AllMeshController.IngameManager.GetComponent<CardSystem>().pickedCard;  //이게 될까 .. 안되면 이거문제일듯.
                    gameObject.GetComponent<InGameSceneManager>().SendTerrainType(picked.GetComponent<CardData>().data.cardIndex);

                    //1.5초 여유 ..!!
                    gameObject.GetComponent<InGameSceneManager>().SendChangeTurn();
                }
                currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.ENEMYTURN;
                gameObject.GetComponent<TurnSystem>().TurnSet();
                break;
            case FLOW.ENEMYTURN_ROLLINGDICE:
                currentFlow = FLOW.ENEMYTURN_PICKINGCARD;
                break;
            case FLOW.ENEMYTURN_PICKINGLOC:
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN_NOTYETFLAG;
                //gameObject.GetComponent<TurnSystem>().TurnSet();
                break;
            
            case FLOW.ENEMYTURN_PICKEVENTCARD:
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN_NOTYETFLAG;
                //gameObject.GetComponent<TurnSystem>().TurnSet();
                break;

        }
        if(animationImg)
            tmpAnimationImage.SetActive(false);
        isWaitingTime = false;
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
                //서버가 대기신호보내고 아무것도안함, 서버가 없으면 바로 시작
                if (GameObject.Find("GameCores") == null)
                {
                    currentFlow = FLOW.ENEMYTURN_ROLLINGDICE;
                    FlowChange(FLOW.READY_DONE);
                }

                else
                {
                    gameObject.GetComponent<InGameSceneManager>().StartWaitCoroutine();
                }
                    
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
                diceCanvas.SetActive(false);
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
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_ROLLINGDICE, 5, true));    // <<< 여기  5라는 숫자를 바꾸면댐
                break;

            //이벤트카드가 없다면 바로 대기상태로 변경
            case FLOW.TO_PICKINGLOC:
                //애니메이션 여기
                diceCanvas.SetActive(true);
                setTerrainCanvas.SetActive(false);
                TurnSystem.isSetTerrainDone = true;
                Camera.main.GetComponent<PCverPIcking>().TurnChangeLogic();
                StartCoroutine(DisplayEventWaitingTime(FLOW.TO_PICKINGLOC, 5, false));    // <<< 여기  5라는 숫자를 바꾸면댐
                break;
            case FLOW.TO_PICKEVENTCARD:
                //애니메이션 여기
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
                StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_PICKINGLOC, 5, false));
                break;
            case FLOW.ENEMYTURN_PICKEVENTCARD:
                StartCoroutine(DisplayEventWaitingTime(FLOW.ENEMYTURN_PICKEVENTCARD, 2, true));
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
