using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EditMesh : MonoBehaviour {
    public GameObject[] buildingObj;
    public GameObject[] MovingObj;

    public void Start()
    {
        //setMaterial(1, "COLD");
        //setMaterial(2, "COLD");
        //setMaterial(3, "COLD");
        //setMaterial(4, "COLD");
        //setMaterial(5, "COLD");
        //setMaterial(6, "COLD");
        //setMaterial(7, "COLD");
        //setMaterial(8, "COLD");
        //setMaterial(9, "COLD");
        //setMaterial(11, "COLD");
        //setMaterial(12, "COLD");
        //setMaterial(13, "COLD");
        //setMaterial(14, "COLD");
        //setMaterial(15, "COLD");
        //setMaterial(16, "COLD");
        //setMaterial(17, "COLD");
        //setMaterial(18, "COLD");
        //setMaterial(19, "COLD");
        //setMaterial(21, "MODERATION");
        //setMaterial(22, "MODERATION");
        //setMaterial(23, "MODERATION");
        //setMaterial(24, "MODERATION");
        //setMaterial(25, "MODERATION");
        //setMaterial(26, "MODERATION");
        //setMaterial(27, "MODERATION");
        //setMaterial(28, "MODERATION");
        //setMaterial(29, "MODERATION");
        //setMaterial(31, "MODERATION");
        //setMaterial(32, "MODERATION");
        //setMaterial(33, "MODERATION");
        //setMaterial(34, "MODERATION");
        //setMaterial(35, "MODERATION");
        //setMaterial(36, "MODERATION");
        //setMaterial(37, "MODERATION");
        //setMaterial(38, "MODERATION");
        //setMaterial(39, "MODERATION");


        //setObject(1, "COLD");
        //setObject(2, "COLD");
        //setObject(3, "COLD");
        //setObject(4, "COLD");
        //setObject(5, "COLD");
        //setObject(6, "COLD");
        //setObject(7, "COLD");
        //setObject(8, "COLD");
        //setObject(9, "COLD");
        //setObject(11, "COLD");
        //setObject(12, "COLD");
        //setObject(13, "COLD");
        //setObject(14, "COLD");
        //setObject(15, "COLD");
        //setObject(16, "COLD");
        //setObject(17, "COLD");
        //setObject(18, "COLD");
        //setObject(19, "COLD");
        //setMovingObj(21, "MODERATION");
        //setMovingObj(22, "MODERATION");
        //setMovingObj(23, "MODERATION");
        //setMovingObj(24, "MODERATION");
        //setMovingObj(25, "MODERATION");
        //setMovingObj(26, "MODERATION");
        //setMovingObj(27, "MODERATION");
        //setMovingObj(28, "MODERATION");
        //setMovingObj(29, "MODERATION");
        //setMovingObj(31, "MODERATION");
        //setMovingObj(32, "MODERATION");
        //setMovingObj(33, "MODERATION");
        //setMovingObj(34, "MODERATION");
        //setMovingObj(35, "MODERATION");
        //setMovingObj(36, "MODERATION");
        //setMovingObj(37, "MODERATION");
        //setMovingObj(38, "MODERATION");
        //setMovingObj(39, "MODERATION");
    }

    public void setMaterial(int meshNum, string material)
    {
        GameObject meshObj = GameObject.Find(meshNum.ToString());
        if (material == "DEFAULT")
        {
            meshObj.GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_Default");
        }
        if (material == "MODERATION")
        {
            meshObj.GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_Moderation");
        }
        else if (material == "BARREN")
        {
            meshObj.GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_BARREN");
        }
        else if (material == "COLD")
        {
            meshObj.GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_COLD");
        }
        else if (material == "SEA")
        {
            meshObj.GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_Sea");
        }
        else if (material == "MOUNTAIN")
        {
            meshObj.GetComponent<MeshRenderer>().material = Resources.Load<Material>("M_Mountain");
        }
    }

    public void setObject(int meshNum, string material)
    {
        GameObject meshObj = GameObject.Find(meshNum.ToString());
        if (material == "DEFAULT")
        {
            Destroy(meshObj.GetComponent<MeshControllerTitle>().MyObject);
        }
        if (material == "MODERATION")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, buildingObj[RandomValue(13, 16)], 1.00f);
        }
        else if (material == "BARREN")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, buildingObj[RandomValue(5, 12)], 1.00f);
        }
        else if (material == "COLD")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, buildingObj[RandomValue(1, 4)], 1.00f);
        }
        else if (material == "SEA")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, buildingObj[18], 1.00f);
        }
        else if (material == "MOUNTAIN")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, buildingObj[0], 1.00f);
        }
    }

    public void setMovingObj(int meshNum, string material)
    {
        GameObject meshObj = GameObject.Find(meshNum.ToString());
        if (material == "DEFAULT")
        {
            Destroy(meshObj.GetComponent<MeshControllerTitle>().MyObject);
        }
        if (material == "MODERATION")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, MovingObj[0], 1.03f);
        }
        else if (material == "BARREN")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, MovingObj[6], 1.03f);
        }
        else if (material == "COLD")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, MovingObj[RandomValue(1, 3)], 1.03f);
        }
        else if (material == "SEA")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, MovingObj[RandomValue(4, 5)], 1.03f);
        }
        else if (material == "MOUNTAIN")
        {
            meshObj.GetComponent<MeshControllerTitle>().EulerRotCal(meshObj, MovingObj[0], 1.03f);
        }
    }

    private int RandomValue(int min, int max)
    {
        return Random.Range(min, max + 1);
    }
}
