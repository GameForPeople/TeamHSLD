using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BuildOnPlanet : MonoBehaviour
{
    private GameObject target_;
    public Transform objectSet;

    public void EulerRotCal(GameObject targetObj, GameObject buildingObj,float offset, int index, int buildIndex)
    {
        target_ = Instantiate(buildingObj);
        target_.transform.position = new Vector3(targetObj.transform.position.x, targetObj.transform.position.y, targetObj.transform.position.z) * offset;
        target_.transform.parent = objectSet;
        StartCoroutine(ComponentOn(index, buildIndex));
        //target_.transform.LookAt(gameObject.transform.position);
        //target_.transform.eulerAngles = new Vector3(target_.transform.eulerAngles.x - 90, target_.transform.eulerAngles.y, target_.transform.eulerAngles.z);
    }

    IEnumerator ComponentOn(int index, int buildIndex)
    {
        yield return new WaitForSeconds(1f);
        target_.AddComponent<ObjectMoving>();
        target_.GetComponent<ObjectMoving>().startingIndex = index;
        target_.GetComponent<ObjectMoving>().thisObject = (MOVINGOBJECT)(buildIndex - 1);
    }

    public void SetDone()
    {
        if (CameraController.ChangeableCount != 0)
            return;
        
        if(gameObject.GetComponent<TutorialManager>() == null)
        {
            gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGLOC);
            gameObject.GetComponent<FlowSystem>().diceCntingUI.SetActive(false);
            gameObject.GetComponent<FlowSystem>().setTerrainCanvas.SetActive(false);
        }
            
        if (gameObject.GetComponent<TutorialManager>() != null)
        {
            if(gameObject.GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_SELECTLOCDONE))
            {
                gameObject.GetComponent<TutorialFlowSystem>().FlowChange(FLOW.TO_PICKINGLOC);
                gameObject.GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.INGAME_TURNEND);
                gameObject.GetComponent<TutorialManager>().pointOff();
            }
            else if(gameObject.GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_SELECTTERRAINLOC_2))
            {
                gameObject.GetComponent<TutorialFlowSystem>().FlowChange(FLOW.TO_PICKINGLOC);
                gameObject.GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.INGAME_ENEMYTURN_ROLLINGDICE_2);
                gameObject.GetComponent<TutorialManager>().pointOff();
            }
            else if(gameObject.GetComponent<TutorialManager>().currentTutorial.Equals(TUTORIAL.INGAME_ATTACK))
            {
                //gameObject.GetComponent<TutorialFlowSystem>().FlowChange(FLOW.TO_PICKINGLOC);
                gameObject.GetComponent<TutorialFlowSystem>().diceCntingUI.SetActive(false);
                gameObject.GetComponent<TutorialFlowSystem>().setTerrainCanvas.SetActive(false);
                gameObject.GetComponent<TutorialManager>().DoingTutorial(TUTORIAL.ENDGAME_CONDITIONTOWIN);
                return;
            }
            gameObject.GetComponent<TutorialFlowSystem>().diceCntingUI.SetActive(false);
            gameObject.GetComponent<TutorialFlowSystem>().setTerrainCanvas.SetActive(false);
        }
    }
}
////// Mesh쪽으로 코드 옮겼어요! [181106]YSK