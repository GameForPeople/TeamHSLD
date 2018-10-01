using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum FLOW
{
    MATCHINGCOMPLETE,
    READY_TURNORDER,
    READY_SETCARD,
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
    public GameObject timerCanvas;
    public GameObject cardSetCanvas;
    public GameObject turnSetCanvas;
    public GameObject readyCanvas;

    public void FlowChange(FLOW doneFlow)
    {
        switch(doneFlow)
        {
            case FLOW.MATCHINGCOMPLETE:

                break;
            case FLOW.READY_TURNORDER:
                turnSetCanvas.SetActive(false);
                cardSetCanvas.SetActive(true);
                currentFlow = FLOW.READY_SETCARD;
                break;
            case FLOW.READY_SETCARD:
                cardSetCanvas.SetActive(false);
                readyCanvas.SetActive(false);
                //선공 / 후공
                gameObject.GetComponent<TurnSystem>().TurnSet();
                break;
            case FLOW.WAITING:
                currentFlow = FLOW.TO_ROLLINGDICE;
                break;
            case FLOW.TO_PICKINGCARD:
                currentFlow = FLOW.TO_PICKINGLOC;
                cardCanvas.SetActive(false);
                timerCanvas.SetActive(true);
                StartCoroutine(gameObject.GetComponent<TurnSystem>().BuildTimer());
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
