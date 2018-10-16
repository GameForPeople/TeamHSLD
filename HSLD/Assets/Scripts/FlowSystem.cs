using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum FLOW
{
    READY_MATCHINGCOMPLETE,
    READY_DISPLAYMISSION,
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
    public GameObject matchingCompleteCanvas;
    public GameObject spinCanvas;
    public GameObject displayMissionCanvas;
    
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
                displayMissionCanvas.SetActive(true);
                matchingCompleteCanvas.SetActive(false);
                currentFlow = FLOW.READY_DISPLAYMISSION;
                break;
            case FLOW.READY_DISPLAYMISSION:
                displayMissionCanvas.SetActive(false);
                cardSetCanvas.SetActive(true);
                currentFlow = FLOW.READY_SETCARD;
                break;
            case FLOW.READY_TURNORDER:
                
                turnSetCanvas.SetActive(false);
                readyCanvas.SetActive(false);

                //선공 / 후공 - 임시, 서버붙이면 변경
                gameObject.GetComponent<TurnSystem>().TurnSet();
                //currentFlow = FLOW.READY_DONE;
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

                if (SoundManager.instance_ != null)
                    SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[5], 1.0f);
                currentFlow = FLOW.TO_PICKINGCARD;
                break;
            case FLOW.TO_PICKINGLOC:
                currentFlow = FLOW.TO_PICKEVENTCARD;

                break;
            case FLOW.TO_PICKEVENTCARD:
                gameObject.GetComponent<TurnSystem>().TurnSet();
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
