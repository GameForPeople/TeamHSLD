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
    DISPLAY_EVENT         //이벤트연출 - 나중에
}

public class FlowSystem : MonoBehaviour
{
    public FLOW currentFlow;

    public GameObject cardCanvas;
    public GameObject cardSetCanvas;
    public GameObject turnSetCanvas;
    public GameObject readyCanvas;
    public GameObject spinCanvas;

    public void FlowChange(FLOW doneFlow)
    {
        switch(doneFlow)
        {
            case FLOW.MATCHINGCOMPLETE:
                break;
            case FLOW.READY_TURNORDER:
                turnSetCanvas.SetActive(false);
                readyCanvas.SetActive(false);
                //선공 / 후공
                gameObject.GetComponent<TurnSystem>().TurnSet();
                spinCanvas.SetActive(true);
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
                currentFlow = FLOW.TO_PICKINGCARD;
                break;
            case FLOW.TO_PICKINGLOC:
                currentFlow = FLOW.TO_PICKEVENTCARD;
                break;
            case FLOW.TO_PICKEVENTCARD:
                currentFlow = FLOW.WAITING;
                break;
        }
    }
}
