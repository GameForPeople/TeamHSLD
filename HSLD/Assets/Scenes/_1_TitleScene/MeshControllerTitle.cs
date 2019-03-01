using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MeshControllerTitle : MonoBehaviour {

    public GameObject MyObject;

    public void EulerRotCal(GameObject targetObj, GameObject buildingObj, float offset)
    {
        if (MyObject == null)
        {
            Debug.Log("new");
            MyObject = Instantiate(buildingObj);
        }
        else
        {
            Destroy(MyObject);
            MyObject = Instantiate(buildingObj);
        }

        MyObject.transform.position = new Vector3(targetObj.transform.position.x, targetObj.transform.position.y, targetObj.transform.position.z) * offset;
        MyObject.transform.parent = GameObject.Find("ObjectSetCore").transform;

        MyObject.transform.LookAt(GameObject.Find("Sphere_Core").transform);

        if (MyObject.transform.childCount == 0)
            MyObject.transform.eulerAngles = new Vector3(MyObject.transform.eulerAngles.x - 90, MyObject.transform.eulerAngles.y, Random.Range(0, 360));
        else
        {
            MyObject.transform.eulerAngles = new Vector3(MyObject.transform.eulerAngles.x - 90, MyObject.transform.eulerAngles.y, MyObject.transform.eulerAngles.z);
            MyObject.transform.GetChild(0).transform.localEulerAngles = new Vector3(MyObject.transform.GetChild(0).transform.localEulerAngles.x, MyObject.transform.GetChild(0).transform.localEulerAngles.y, Random.Range(0, 360));
        }
    }
}