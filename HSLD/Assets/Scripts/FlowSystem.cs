using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum FLOW
{
    MATCHINGCOMPLETE,
    READY_TURNORDER,
    READY_SETCARD,
    READY_DONE,
    WAITING,
    TO_ROLLINGDICE,
    TO_PICKINGCARD,
    TO_PICKINGLOC,
    TO_PICKEVENTCARD,
    DISPLAY_EVENT,         //이벤트연출 - 나중에
    TSETVER
}


public class FlowSystem : MonoBehaviour
{
    public FLOW currentFlow;

    public GameObject cardCanvas;
    public GameObject cardSetCanvas;
    public GameObject turnSetCanvas;
    public GameObject readyCanvas;
    public GameObject spinCanvas;

    private void Start()
    {
        //테스트버전
        if (currentFlow.Equals(FLOW.TSETVER))
            FlowChange(currentFlow);
        else
            FlowChange(FLOW.MATCHINGCOMPLETE);
    }

    public void FlowChange(FLOW doneFlow)
    {
        switch(doneFlow)
        {
            case FLOW.MATCHINGCOMPLETE:
                SoundManager.instance_.BGMMixing(SoundManager.instance_.clips[1], 2.0f);
                currentFlow = FLOW.READY_SETCARD;
                break;
            case FLOW.READY_TURNORDER:
                turnSetCanvas.SetActive(false);
                readyCanvas.SetActive(false);
                //선공 / 후공
                gameObject.GetComponent<TurnSystem>().TurnSet();
                spinCanvas.SetActive(true);
                SoundManager.instance_.BGMMixing(SoundManager.instance_.clips[0], 0.5f);
                break;
            case FLOW.READY_SETCARD:
                cardSetCanvas.SetActive(false);
                turnSetCanvas.SetActive(true);
                currentFlow = FLOW.READY_TURNORDER;
                break;
            case FLOW.READY_DONE:
                break;
            case FLOW.WAITING:
                currentFlow = FLOW.TO_ROLLINGDICE;
                break;
            case FLOW.TO_PICKINGCARD:
                GameObject.FindWithTag("MainCamera").GetComponent<PCverPIcking>().enabled = true;
                currentFlow = FLOW.TO_PICKINGLOC;
                break;
            case FLOW.TO_ROLLINGDICE:
                SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[5], 1.0f);
                currentFlow = FLOW.TO_PICKINGCARD;
                break;
            case FLOW.TO_PICKINGLOC:
                currentFlow = FLOW.TO_PICKEVENTCARD;
                break;
            case FLOW.TO_PICKEVENTCARD:
                currentFlow = FLOW.WAITING;
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
