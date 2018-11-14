using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BuildOnPlanet : MonoBehaviour
{
    private GameObject target_;
    public Transform objectSet;

    public void EulerRotCal(GameObject targetObj, GameObject buildingObj,float offset)
    {
        target_ = Instantiate(buildingObj);
        target_.transform.position = new Vector3(targetObj.transform.position.x, targetObj.transform.position.y + offset, targetObj.transform.position.z); ;
        target_.transform.parent = objectSet;

        target_.transform.LookAt(gameObject.transform.position);
        target_.transform.eulerAngles = new Vector3(target_.transform.eulerAngles.x + 180, target_.transform.eulerAngles.y, target_.transform.eulerAngles.z);
    }

    public void SetDone()
    {
        if (CameraController.ChangeableCount != 0)
            return;

        gameObject.GetComponent<FlowSystem>().FlowChange(FLOW.TO_PICKINGLOC);
    }
}
////// Mesh쪽으로 코드 옮겼어요! [181106]YSK