using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum FLOW
{
    WAITING,
    TO_ROLLINGDICE,
    TO_PICKINGCARD,
    TO_PICKINGLOC,
    DISPLAYINGEVENT
}

public class FlowSystem : MonoBehaviour
{
    public FLOW currentFlow;

    public GameObject cardSetCanvas;
    public GameObject timerCanvas;

    public void FlowChange(FLOW doneFlow)
    {
        switch(doneFlow)
        {
            case FLOW.WAITING:
                currentFlow = FLOW.TO_ROLLINGDICE;
                break;
            case FLOW.TO_PICKINGCARD:
                currentFlow = FLOW.TO_PICKINGLOC;
                cardSetCanvas.SetActive(false);
                timerCanvas.SetActive(true);
                StartCoroutine(gameObject.GetComponent<TurnSystem>().BuildTimer());
                break;
            case FLOW.TO_ROLLINGDICE:
                currentFlow = FLOW.TO_PICKINGCARD;
                break;
            case FLOW.TO_PICKINGLOC:
                currentFlow = FLOW.DISPLAYINGEVENT;
                break;
            case FLOW.DISPLAYINGEVENT:
                currentFlow = FLOW.WAITING;
                break;
        }
    }
}
